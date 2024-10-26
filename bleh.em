#include "utils.h"

@[for struct in structs.values()]@
struct @(struct.name)_bytes {
  uint8_t *bytes;
};

size_t @(struct.name)_size(const struct @(struct.name)_bytes b) {
  size_t size = 
@[for e in struct.entries]@
@[    if e.is_scalar() and (e.is_builtin() or e.is_enum())]@
                      + @(e.size()) /* @e.name -> sizeof(@(e.get_ctype())) (@(e.padded ? 'padded' ! 'packed' )) */
@[    if e.is_array() and not e.is_varsize() and (e.is_builtin() or e.is_enum())]@
                      + (@(e.count) * @(e.size())) /* @e.name -> [sizeof(@(e.get_ctype()))] (@(e.padded ? 'padded' ! 'packed' )) */
@# Scalar of a struct
@[    elif e.is_scalar() and e.is_struct()]@
@[        if e.is_array()]@
@[        else]@
                      + @(e.typ)_size(@(struct.name)_get_@(e.name)(b))
@[        end if]@
@[    elif e.is_array() and not e.is_varsize()]@
                      + @(e.size
@[    end if]@
@[end for]@
}

@[  for entry in struct.entries]@
@[    if entry.is_array()]@   
@(entry.get_ctype()) @(struct.name)_get_@(entry.name)(const struct @(struct.name)_bytes b, size_t idx) {
@[    else]@
@(entry.get_ctype()) @(struct.name)_get_@(entry.name)(const struct @(struct.name)_bytes b) {
@[    end if]@
  const size_t offset = 0
@[    for prior_entry in struct.entries]@
@[    if prior_entry == entry]@
@[        break]@
@[    end if]@
@# There are several cases we will handle individually
@# Scalar of fixed-size built-in or enum, directly produce the size:
@[    if prior_entry.is_scalar() and (prior_entry.is_builtin() or prior_entry.is_enum())]@
                      + @(prior_entry.size()) /* @prior_entry.name -> sizeof(@(prior_entry.get_ctype())) (@(prior_entry.padded ? 'padded' ! 'packed' )) */
@# Scalar of a struct
@[    elif prior_entry.is_scalar() and prior_entry.is_struct()]@
                      + @(prior_entry.typ)_size(@(struct.name)_get_@(prior_entry.name)(b))
@[    end if]@
@[    end for]@
                       ;
@[    if entry.is_builtin()]@   
  return decode_@(entry.typ)(&b.bytes[offset]);
@[    else]@
  return (@(entry.get_ctype())){ .bytes = &b.bytes[offset] };
@[    end if]@
}
@[  end for]@

@[end for]@


