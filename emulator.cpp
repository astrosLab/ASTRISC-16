#include <iostream>
#include <string>
#include <map>
#include <array>

class ASTRISC {
public:
	ASTRISC() {
		flags = {
			{"zero", false},
			{"not zero", false},
			{"signed", false},
			{"not signed", false}
		};

		opcodes = {
			&ASTRISC::op_nop, &ASTRISC::op_ldi,
			&ASTRISC::op_lod, &ASTRISC::op_str,
			&ASTRISC::op_call, &ASTRISC::op_ret,
			&ASTRISC::op_push, &ASTRISC::op_pop,
			&ASTRISC::op_mov, &ASTRISC::op_add,
			&ASTRISC::op_sub, &ASTRISC::op_mul,
			&ASTRISC::op_div, &ASTRISC::op_mod,
			&ASTRISC::op_inc, &ASTRISC::op_dec,
			&ASTRISC::op_shl, &ASTRISC::op_shr,
			&ASTRISC::op_and, &ASTRISC::op_nand,
			&ASTRISC::op_or, &ASTRISC::op_nor,
			&ASTRISC::op_xor, &ASTRISC::op_cmp,
			&ASTRISC::op_jmp, &ASTRISC::op_brh_z,
			&ASTRISC::op_brh_nz, &ASTRISC::op_brh_s,
			&ASTRISC::op_brh_ns, &ASTRISC::op_nop,
			&ASTRISC::op_nop, &ASTRISC::op_halt,
		};

		stack_pointer = 0xFFEF;
		call_stack_pointer = 0xF7EF;

		memory[0] = 0b0100000101000000;	
		program_counter = 0;
	};

	void decode() {
		int instruction = memory[program_counter];
		int opcode = (instruction & 63488) >> 11;
		std::cout << opcode << std::endl;
		std::cout << "PC: " << program_counter << std::endl;
		(this->*opcodes[opcode])();
	}
	
	void op_nop() {
		program_counter++;	
	}

	void op_ldi() {
		int reg = (memory[program_counter] & 1792) >> 8;
		int imm = memory[program_counter + 1];
		
		std::cout << "Register " << reg << " = " << imm << std::endl;

		registers[reg] = imm;
		program_counter += 2;	
	}

	void op_lod() {
		int instruction = memory[program_counter];
		int imm_reg = (instruction & 1024) >> 10;
		int reg = (instruction & 896) >> 7;
		int use_offset = (instruction & 64) >> 6;
		int offset_reg = (instruction & 56) >> 3;
		int ptr_reg = (instruction & 15);
		int addr;

		if (imm_reg == 0) {
			addr = memory[program_counter + 1];
			program_counter += 2;
		} else {
			addr = registers[ptr_reg];
			program_counter++;
		}

		if (use_offset == 1)
			addr += registers[offset_reg];

		std::cout << registers[offset_reg] << std::endl;

		std::cout
			<< "IMM/REG: " << imm_reg
			<< ", REG: " << reg
			<< ", OFFSET?: " << use_offset
			<< ", OFFSET REG: " << offset_reg
			<< ", POINTER REG: " << ptr_reg
			<< ", ADDRESS: " << addr
			<< std::endl;

		registers[reg] = memory[addr];
	}

	void op_str() {
		int instruction = memory[program_counter];
		int imm_reg = (instruction & 1024) >> 10;
		int reg = (instruction & 896) >> 7;
		int use_offset = (instruction & 64) >> 6;
		int offset_reg = (instruction & 56) >> 3;
		int ptr_reg = (instruction & 15);
		int addr;

		if (imm_reg == 0) {
			addr = memory[program_counter + 1];
			program_counter += 2;
		} else {
			addr = registers[ptr_reg];
			program_counter++;
		}

		if (use_offset == 1)
			addr += registers[offset_reg];

		std::cout << registers[offset_reg] << std::endl;

		std::cout
			<< "IMM/REG: " << imm_reg
			<< ", REG: " << reg
			<< ", OFFSET?: " << use_offset
			<< ", OFFSET REG: " << offset_reg
			<< ", POINTER REG: " << ptr_reg
			<< ", ADDRESS: " << addr
			<< std::endl;

		memory[addr] = registers[reg];
	}

	void op_call() {
		int address = memory[program_counter + 1];

		if (call_stack_pointer != 0xF7FF){
			memory[call_stack_pointer] = program_counter + 1;
			call_stack_pointer--;
		} else {
			std::cout << "Call Stack Overflow\n";
			// Add call stack overflow error code
		}

		program_counter += 2;
	}

	void op_ret() {
		if (call_stack_pointer != 0xFFEF){
			call_stack_pointer++;
		} else {
			std::cout << "Call Stack Underflow\n";
			// Add call stack underflow error code
		}

		program_counter++;
	}

	void op_push() {
		int reg = (memory[program_counter] & 1792) >> 8;

		if (stack_pointer != 0xFFFF) { // Fix overflow position
			memory[stack_pointer] = registers[reg];
			stack_pointer--;
			std::cout << "Register " << reg << " pushed to stack" << std::endl;
		} else {
			std::cout << "Stack Overflow\n";
			// Add stack overflow error code
		}

		program_counter++;
	}

	void op_pop() {
		int reg = (memory[program_counter] & 1792) >> 8;

		if (stack_pointer != 0xFFEF) {
			stack_pointer++;
			registers[reg] = memory[stack_pointer];
			std::cout << "Stack popped to register " << reg << std::endl;
		} else {
			std::cout << "Stack Underflow\n";
			// Add stack overflow error code
		}

		program_counter++;
	}

	void op_mov() {
		int instruction = memory[program_counter];
		int reg_from = (instruction & 1792) >> 8;
		int reg_to = (instruction & 112) >> 8;
	
		registers[reg_to] = registers[reg_from];

		std::cout << "Register " << reg_from << " copied to register " << reg_to << std::endl;
			
		program_counter++;
	}
	
	void op_add() {}
	void op_sub() {}
	void op_mul() {}
	void op_div() {}
	void op_mod() {}
	void op_inc() {}
	void op_dec() {}
	void op_shl() {}
	void op_shr() {}
	void op_and() {}
	void op_nand() {}
	void op_or() {}
	void op_nor() {}
	void op_xor() {}
	void op_cmp() {}
	void op_jmp() {}
	void op_brh_z() {}
	void op_brh_nz() {}
	void op_brh_s() {}
	void op_brh_ns() {}
	void op_halt() {}
private:
	std::array<int, 65536> memory;
	std::array<int, 8> registers;
	std::map<std::string, bool> flags;
	int program_counter;
	int stack_pointer;
	int call_stack_pointer;
	using OpcodeFunction = void (ASTRISC::*)();
	std::array<OpcodeFunction, 32> opcodes;
};

int main() {
	ASTRISC cpu;
	cpu.decode();
	return 0;
}

