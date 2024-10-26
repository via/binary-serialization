from dataclasses import dataclass
import lark
import em
import pprint

@dataclass
class BuiltinType:
    size: int
    ctype: str

types = {
        "u8": BuiltinType(1, "uint8_t"),
        "u16": BuiltinType(2, "uint16_t"),
        "u32": BuiltinType(4, "uint32_t"),
        "u64": BuiltinType(8, "uint64_t"),

        "i8": BuiltinType(1, "int8_t"),
        "i16": BuiltinType(2, "int16_t"),
        "i32": BuiltinType(4, "int32_t"),
        "i64": BuiltinType(8, "int64_t"),

        "f32": BuiltinType(4, "float"),
        "f64": BuiltinType(8, "double"),
}

def pad4(size: int):
    overflow = size % 4
    if overflow == 0:
        return size
    else:
        return size + (4 - overflow)

class UnionEntry:
    pass

class Enum:
    def __init__(self, tree):
        self.name = tree.children[0].value
        self.container = tree.children[1].value
        self.values = {}
        for elem in tree.children[2:]:
            key = elem.children[0].value
            value = elem.children[1].value
            self.values[key] = value


@dataclass
class Entry:
    typ: str
    name: str
    padded: bool

    def get_ctype(self):
        t = types[self.typ]
        if isinstance(t, BuiltinType):
            return t.ctype
        elif isinstance(t, Structure):
            return f"struct {t.name}_bytes"
        elif isinstance(t, Enum):
            return f"enum {t.name}"
        else:
            return "void"

    def is_varsize(self):
        if isinstance(self, VariableArray) and self.is_packed:
            return True
        t = types[self.typ]
        if isinstance(t, BuiltinType):
            return False
        if isinstance(t, Enum):
            return False
        if isinstance(t, Structure):
            return any([x.is_varsize for x in t.entries])
        return True

    def size(self):
        if self.is_varsize():
            raise Exception("not fixed size")
        t = types[self.typ]
        size = None
        if isinstance(t, BuiltinType):
            size = t.size
        else:
            raise Exception("not implemented")
        if self.padded:
            return pad4(size)
        else:
            return size

    def is_array(self):
        return False

    def is_scalar(self):
        return False

    def is_enum(self):
        t = types[self.typ]
        if isinstance(t, BuiltinType):
            return True
        else:
            return False
    
    def is_struct(self):
        t = types[self.typ]
        if isinstance(t, Structure):
            return True
        else:
            return False

    def is_builtin(self):
        t = types[self.typ]
        if isinstance(t, BuiltinType):
            return True
        else:
            return False

# Size/offset rendering paths:
# - (array of) fixed size scalar, built-in or enum:
#   capacity/count * sizeof()
# - scalar struct
#   Struct_get()
# - array of structs
#   loop over capacity/count Struct_get()



@dataclass
class Scalar(Entry):
    def is_scalar(self):
        return True


@dataclass
class FixedArray(Entry):
    count: int

    def is_array(self):
        return True

@dataclass
class VariableArray(Entry):
    capacity: int
    count_spec: str
    is_packed: bool

    def is_variable_array(self):
        return True

class Structure:
    name: str

    def __init__(self, name):
        self.name = name
        self.entries = []

    def add_entry(self, entry):
        if entry.data == "single":
            self.entries.append(
                    Scalar(
                        typ=entry.children[0].value,
                        name=entry.children[1].value,
                        padded=True)
                    )
        elif entry.data == "fixedarray":
            self.entries.append(
                    FixedArray(
                        typ=entry.children[0].value,
                        name=entry.children[1].value,
                        padded=True,
                        count=entry.children[2].value)
                    )
        elif entry.data == "vararray":
            packed = True if entry.children[0] is not None else False
            self.entries.append(
                    VariableArray(
                        typ=entry.children[1].value,
                        name=entry.children[2].value,
                        padded=True,
                        capacity=entry.children[3].value,
                        count_spec=entry.children[4].value,
                        is_packed=packed)
                    )

        elif entry.data == "union":
            pass
        elif entry.data == "pack":
            pass



parser = lark.Lark(open("syntax.lark", "r"), start="file")
text = open("t1.expr", "r").read()
decoded = parser.parse(text)
structs = {}

for toplevel_entry in decoded.children:
    if toplevel_entry.data == "struct":
        struct = Structure(toplevel_entry.children[0].value)
        for entry in toplevel_entry.children[1:]:
            struct.add_entry(entry)
        structs[struct.name] = struct
        types[struct.name] = struct
    elif toplevel_entry.data == "enum":
        e = Enum(toplevel_entry)
        types[e.name] = e

template = open("bleh.em", "r").read()
expanded = em.expand(template, locals={
    "structs": structs
    })

print(expanded)

        
        
