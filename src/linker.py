import sys

file_paths = sys.argv[1:]
linked_program = []

for file in file_paths:
    with open(file, "r") as program:
        linked_program.append(program.read())

linked_program_string = "\n".join(linked_program)

print(linked_program_string)

