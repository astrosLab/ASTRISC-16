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

labels = {}

formatted_program = []

for num, line in enumerate(program):
    separated = line.split(" ")
    if separated[0] == "#define":
        definitions[separated[1]] = to_decimal(separated[2])
    elif separated[0][0] == '.':
        labels[separated[0]] = {"position": num}
    else:
        formatted_program.append([separated[0], separated[1:]])

address = 0;
for num, line in enumerate(formatted_program):
    formatted_program[num].append(address)

    if line[0] == "str" or line[0] == "lod":
        if line[1][1].isdigit() or line[1][1] in definitions.keys():
            address += 2
        else:
            address += 1
    elif line[0] == "ldi" or line[0] == "cal":
        address += 2
    else:
        address + 1
 
    print(line)

for d in definitions:
    print(d, definitions[d])

for l in labels:
    print(l, labels[l])

