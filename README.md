# ASTRISC-16 CPU

A 16-bit RISC-like CPU project that I designed for fun.

The name comes from combining Astro (My main username) and RISC (Reduced Instruction Set Computer)

## Specs / Features

- 16 bit data/addr. bus
- 64KiB RAM
- Von Neumann Architecture
- 16 Memory-Mapped I/O (PC, Error, 14 General)
- Flags (Zero, Not Zero, Signed, Not Signed)
- 32 Opcodes
- 8 General Registers
- Instruction Register, Program Counter
- Pointer Support
- Stack Pointer Register
- Call Stack Pointer Register
- Relative Jumping

## Opcodes

("//" means next word/address)
- 0x00 NOP - Does nothing
- 0x01 LDI REG // IMMEDIATE - Load IMMEDIATE into REG
- 0x02 LOD IMM/REG REG USE-OFFSET OFFSET REG-PTR // IMMEDIATE - Load into REG, using IMMEDIATE+OFFSET(IF USE-OFFSET) if IMM/REG=IMM and REG-PTR+OFFSET if IMM/REG=REG
- 0x03 STR IMM/REG REG USE-OFFSET OTFFSET REG-PTR // IMMEDIATE - Store into REG, using IMMEDIATE+OFFSET(IF USE-OFFSET) if IMM/REG=IMM and REG-PTR+OFFSET if IMM/REG=REG
- 0x04 CALL // ADDRESS - Push PC+1 to call stack, jump to ADDRESS
- 0x05 RET - Pop call stack, jump to value from call stack output
- 0x06 PUSH REG - Pushes the value in REG into the stack
- 0x07 POP REG - Pop the stack and load into REG
- 0x08 MOV REG-FROM REG-TO - Copy value from REG-FROM into REG-TO
- 0x09 ADD REG-IN REG-IN REG-OUT - Add both REG-INs, outputs to REG-OUT
- 0x0A SUB REG-IN REG-IN REG-OUT - Subtract both REG-INs, outputs to REG-OUT
- 0x0B MUL REG-IN REG-IN REG-OUT - Multiply both REG-INs, outputs to REG-OUT
- 0x0C DIV REG-IN REG-IN REG-OUT - Divide both REG-INs, outputs to REG-OUT
- 0x0D MOD REG-IN REG-IN REG-OUT - Get remainder of both REG-INs, outputs to REG-OUT
- 0x0E INC REG-IN REG-OUT - Increment REG-IN by 1, outputs to REG-OUT
- 0x0F DEC REG-IN REG-OUT - Decrement REG-IN by 1, outputs to REG-OUT
- 0x10 SHL REG-IN REG-OUT - Shift REG-IN left by 1, outputs to REG-OUT
- 0x11 SHR REG-IN REG-OUT - Shift REG-IN right by 1, outputs to REG-OUT
- 0x12 AND REG-IN REG-IN REG-OUT - Get AND of both REG-INs, outputs to REG-OUT
- 0x13 NAND REG-IN REG-IN REG-OUT - Get NAND of both REG-INs, outputs to REG-OUT
- 0x14 OR REG-IN REG-IN REG-OUT - Get OR of both REG-INs, outputs to REG-OUT
- 0x15 NOR REG-IN REG-IN REG-OUT - Get NOR of both REG-INs, outputs to REG-OUT
- 0x16 XOR REG-IN REG-IN REG-OUT - Get XOR of both REG-INs, outputs to REG-OUT
- 0x17 CMP REG-IN REG-IN - Compare both REG-INs, update flags
- 0x18 JMP IMMEDIATE - Offsets the PC by IMMEDIATE
- 0x19 BRH-Z IMMEDIATE - Offsets the PC by IMMEDIATE if ZERO flag is set
- 0x1A BRH-NZ IMMEDIATE - Offsets the PC by IMMEDIATE if NOT ZERO flag is set
- 0x1B BRH-S IMMEDIATE - Offsets the PC by IMMEDIATE if SIGNED flag is set
- 0x1C BRH-NS IMMEDIATE - Offsets the PC by IMMEDIATE if NOT SIGNED flag is set
- 0x1D RESERVED
- 0x1E RESERVED
- 0x1F HALT - Stops the clock

## Error Handling

The ASTRISC-16 doesn't halt on errors such as division by zero, stack overflow, and stack underflow, instead it's stored to a memory-mapped error I/O port.

Stack Overflow = 1
Stack Underflow = 2
Call Stack Overflow = 3
Call Stack Underflow = 4
Division by 0 = 5

