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

p_program = []

# Parse
position = 0
for index, line in enumerate(program):
    parsed = line.split(" ")  
    if parsed[0] == "#define":
        definitions[parsed[1]] = to_decimal(parsed[2])
        continue
    elif parsed[0].startswith("."):
        if position < len(p_program):
            p_program[position]["label"] = parsed[0]
        else:
            p_program.append({"label": parsed[0]})
        continue
    else:
        if position < len(p_program):
            p_program[position]["opcode"] = parsed[0]
            p_program[position]["operands"] = parsed[1:]
        else:
            p_program.append({"opcode": parsed[0], "operands": parsed[1:]})

        position += 1

# Get correct instruction lengths
address = 0
for index, line in enumerate(p_program):
        p_program[index]["address"] = address
        if line["opcode"] in ["str", "lod"]:
            if line["operands"][1].isdigit() or line["operands"][1] in definitions.keys():
                address += 2
            else:
                address += 1
        elif line["opcode"] in ["ldi", "cal"]:
            address += 2
        else:
            address += 1

# Define labels
for index, line in enumerate(p_program):
    if "label" in line.keys():
        definitions[line["label"]] = line["address"]

# Replace definitions with value
for index, line in enumerate(p_program):
    for opindex, operand in enumerate(line["operands"]):
        if operand in definitions.keys():
            if line["opcode"] in ["jmp", "brh-z", "brh-nz", "brh-s", "brh-ns"]:
                line["operands"][opindex] = definitions[operand] - line["address"]
            else:
                line["operands"][opindex] = definitions[operand]
        else:
            line["operands"][opindex] = to_decimal(line["operands"][opindex])

for line in p_program:
    print(line["opcode"], line["operands"])

for definition in definitions:
    print(definition, definitions[definition])

# Convert to numbers and create .bin file
