import sys

file_path = sys.argv[1]

def to_decimal(value: str) -> int:
    if value.startswith("0b"):
        return int(value, 2)
    elif value.startswith("0x"):
        return int (value, 16)
    else:
        return int(value)

program = None
with open(file_path, "r") as assembly:
    program = [i.strip() for i in assembly.readlines() if i.strip() != '']

opcodes = [
    "nop", "ldi", "lod", "str", "call", "ret",
    "push", "pop", "mov", "add", "sub", "mul",
    "div", "mod", "inc", "dec", "shl", "shr",
    "and", "nand", "or", "nor", "xor", "xnor",
    "cmp", "jmp", "brh-z", "brh-nz", "brh-s",
    "brh-nz", "halt"
]

definitions = {
    "r0": 0,
    "r1": 1,
    "r2": 2,
    "r3": 3,
    "r4": 4,
    "r5": 5,
    "r6": 6,
    "r7": 7,
}

formatted_program = []

for num, line in enumerate(program):
    separated = line.split(" ")
    if separated[0] == "#define":
        definitions[separated[1]] = to_decimal(separated[2])
    elif separated[0][0] == '.':
        formatted_program.append({
            "label": separated[0]
        })
    else:
        formatted_program.append({
            "opcode": separated[0],
            "operands": separated[1:]
        })

address = 0
for line in formatted_program:
    if "label" in line:
        print(line["label"])
        definitions[line["label"]] = address
        formatted_program.pop(address)

    address += 1

for line in formatted_program:
    print(line)

for j in definitions:
    print(j, definitions[j])
