#include "utils.h"

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
@[    if prior_entry.render().aligned]@
  offset = align(offset);
@[    end if]@
@add_entry_size('offset', prior_entry)@
@[    end for]@
@[    if entry.render().aligned]@
  offset = align(offset);
@[    end if]@
@[    if entry.is_builtin()]@   
  return decode_@(entry.typ)(&b.bytes[offset]);
@[    else]@
  return (@(entry.get_ctype())){ .bytes = &b.bytes[offset] };
@[    end if]@
}
@[  end for]@

static inline size_t @(struct.name)_size(const struct @(struct.name)_bytes b) {
  size_t size = 0;
@[  for entry in struct.entries]@
@[    if entry.render().aligned]@
  size = align(size);
@[    end if]@
@add_entry_size('size', entry)@
@[  end for]@
  size = align(size);

  return size;
}


@[end for]@


