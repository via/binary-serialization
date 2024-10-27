#include "utils.h"

@[for enum in enums]@
enum @(enum.name) {
@[ for name, value in enum.values.items()]@
  @(enum.name)_@name = @value,
@[ end for]@
};

@[end for]@

@[for struct in structs.values()]@
struct @(struct.name)_bytes {
  uint8_t *bytes;
};

@[def add_entry_size(var, entry)]@
@[    match entry.render()]@
@[      case EntrySize(count=Fixed(1), size=Fixed(s))]@
  /* @entry.name -> sizeof(@(entry.get_ctype())))) */
  @var += @(s);
@[      case EntrySize(count=Fixed(c), size=Fixed(s))]@
  /* @entry.name -> sizeof(@(entry.get_ctype())[@c]))) */
  @var += (@c * @s);
@[      case EntrySize(count=Lookup(c), size=Fixed(s))]@
  /* @entry.name -> sizeof(@(entry.get_ctype())[@c]))) */
  @var += (@(struct.name)_get_@(c)(b) * @s);
@[      case EntrySize(count=Fixed(1), size=Lookup(s))]@
  @var += @(s)_size((@(entry.get_ctype())){&b.bytes[@var]}); 
@[      case EntrySize(count=Fixed(c), size=Lookup(s))]@
  for (size_t i = 0; i < @c; i++) {
    @(entry.get_ctype()) element = {&b.bytes[@var] };
    @var += @(s)_size(element);
  }
@[      case EntrySize(count=Lookup(c), size=Lookup(s))]@
  {
    const size_t count = @(struct.name)_get_@(c)(b);
    for (size_t i = 0; i < count; i++) {
      @(entry.get_ctype()) element = {&b.bytes[@var] };
      @var += @(s)_size(element);
    }
  }
@[    end match]@
@[end def]@

@[  for entry in struct.entries]@
@[    if entry.is_array()]@   
static inline @(entry.get_ctype()) @(struct.name)_get_@(entry.name)(const struct @(struct.name)_bytes b, size_t idx) {
@[    else]@
static inline @(entry.get_ctype()) @(struct.name)_get_@(entry.name)(const struct @(struct.name)_bytes b) {
@[    end if]@
  size_t offset = 0;
@[    for prior_entry in struct.entries]@
@[    if prior_entry == entry]@
@[        break]@
@[    end if]@
@[    if prior_entry.union]@
  if (@(struct.name)_has_@(prior_entry.name)(b)) {
@[    end if]@
@[    if prior_entry.render().aligned]@
  offset = align(offset);
@[    end if]@
@add_entry_size('offset', prior_entry)@
@[    if prior_entry.union]@
  }
@[    end if]@

@[    end for]@
@[    if entry.render().aligned]@
  offset = align(offset);
@[    end if]@
@[    if entry.is_array()]@
@[      match entry.render()]@
@[        case EntrySize(size=Fixed(s))]@
  offset += (@s * idx);
@[        case EntrySize(size=Lookup(s))]@
  for (size_t i = 0; i < idx; i++) {
    @(entry.get_ctype()) element = {&b.bytes[offset] };
    offset += @(s)_size(element);
  }
@[      end match]@
@[    end if]@
@[    if entry.is_builtin()]@   
  return decode_@(entry.typ)(&b.bytes[offset]);
@[    elif entry.is_enum()]@
  return (@(entry.get_ctype()))decode_@(entry.container())(&b.bytes[offset]);
@[    else]@
  return (@(entry.get_ctype())){ .bytes = &b.bytes[offset] };
@[    end if]@
}
@[  for later_entry in struct.entries]@
@[    if later_entry.union and later_entry.union.selector == entry.name]@

static inline bool @(struct.name)_has_@(later_entry.name)(const struct @(struct.name)_bytes b) {
  if (@(struct.name)_get_@(later_entry.union.selector)(b) == @(later_entry.union.match)) {
    return true;
  } else {
    return false;
  }
}
@[    end if]@
@[  end for]@

@[  end for]@

static inline size_t @(struct.name)_size(const struct @(struct.name)_bytes b) {
  size_t size = 0;
@[  for entry in struct.entries]@
@[    if entry.union]@
  if (@(struct.name)_has_@(entry.name)(b)) {
@[    end if]@
@[    if entry.render().aligned]@
  size = align(size);
@[    end if]@
@add_entry_size('size', entry)@
@[    if entry.union]@
  }
@[    end if]@

@[  end for]@
  size = align(size);

  return size;
}


@[end for]@


