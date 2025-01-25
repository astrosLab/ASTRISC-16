import sys

file_path = sys.argv[1]

def to_decimal(value: str) -> int:
    if value.startswith("0b"):
        return int(value, 2)
    elif value.startswith("0x"):
        return int (value, 16)
    else:
        return int(value)

def twos_complement(number, width):
    if number < 0:
        number = (1 << width) + number
    return number

program = None
with open(file_path, "r") as assembly:
    program = [i.strip() for i in assembly.readlines() if i.strip() != '']

opcodes = [
    "nop", "ldi", "lod", "str", "cal", "ret",
    "push", "pop", "mov", "add", "sub", "mul",
    "div", "mod", "inc", "dec", "shl", "shr",
    "and", "nand", "or", "nor", "xor",
    "cmp", "jmp", "brh-z", "brh-nz", "brh-s",
    "brh-ns", "res", "res", "halt"
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
    line["opcode"] = opcodes.index(line["opcode"])

# Convert to binary
program_bytes = []
for index, line in enumerate(p_program):
    instruction = 0
    opcode = line["opcode"] << 11
    instruction += opcode
    if line["opcode"] == 1:
        instruction += line["operands"][0] << 8
        program_bytes.append(instruction)
        program_bytes.append(twos_complement(line["operands"][1], 16))
        continue
    elif line["opcode"] in [6, 7]:
        line["operands"][0] << 8
        program_bytes.append(instruction)
        continue
    elif line["opcode"] in [2, 3]:
        instruction += line["operands"][0] << 7            
        
        if len(line["operands"]) > 2:
                instruction += 64
                instruction += line["operands"][2] << 3

        if p_program[index + 1]["address"] - line["address"] == 2:
            program_bytes.append(instruction)
            program_bytes.append(line["operands"][1])
            continue
        else:
            instruction += 1024
            instruction += line["operands"][1]
            program_bytes.append(instruction)
            continue
    elif line["opcode"] in [4]:
        program_bytes.append(instruction)
        program_bytes.append(line["operands"][0])
        continue
    elif line["opcode"] in [8]:
        instruction += line["operands"][0] << 8
        instruction += line["operands"][1] << 5
        program_bytes.append(instruction)
        continue
    elif line["opcode"] in [9, 10, 11, 12, 13, 19, 20, 21, 22]:
        instruction += line["operands"][0] << 8
        instruction += line["operands"][1] << 5
        instruction += line["operands"][2] << 2
        program_bytes.append(instruction)
        continue
    elif line["opcode"] in [14, 15, 16, 17, 23]:
        instruction += line["operands"][0] << 8
        instruction += line["operands"][1] << 2
        program_bytes.append(instruction)
        continue
    elif line["opcode"] in [24, 25, 26, 27, 28, 29]:
        instruction += twos_complement(line["operands"][0], 11)
        program_bytes.append(instruction)
        continue
    else:
        program_bytes.append(instruction)

# Create .bin file
raw_bytes = []

for word in program_bytes:
    raw_bytes.append((word >> 8) & 255)
    raw_bytes.append(word & 255)

data = bytearray(raw_bytes)
program_name = sys.argv[1].replace(".asm", ".bin")
with open(program_name, "wb") as binary_file:
    binary_file.write(data)
 
