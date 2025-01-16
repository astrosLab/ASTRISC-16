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
		call_stack_pointer = 0xFBEF;

		memory.fill(0);
		
		// Fibonacci program, very messy
		memory[0] = 0b0000101000000000;
		memory[1] = 0b1111111111110010;
		memory[2] = 0b0000100000000000;	
		memory[3] = 0b0000000000000001;	
		memory[4] = 0b0000101100000000;	
		memory[5] = 0b0000000000000001;
		memory[6] = 0b0000110000000000;	
		memory[7] = 0b0000000000001010;
		memory[8] = 0b0001110000000010;
		memory[9] = 0b0100100000100100;	
		memory[10] = 0b0001110010000010;
		memory[11] = 0b0100100000100000;
		memory[12] = 0b0111001100001100;
		memory[13] = 0b1011110001100000;
		memory[14] = 0b1101011111111010;
		memory[15] = 0b1111100000000000;

		running = false;
		debug = false;
		program_counter = 0;
	};

	void debug_mode() { debug = true; }

	void run() {
		running = true;

		while (running == true) {
			memory[0xFFF0] = program_counter;
			decode();
			if (memory[0xFFF2] != 0) {
				std::cout << std::to_string(memory[0xFFF2]) << std::endl;
				memory[0xFFF2] = 0;
			}
		}
	}

	void decode() {
		int instruction = memory[program_counter];
		int opcode = (instruction & 63488) >> 11;

		if (debug) {
			std::cout << std::endl;
			std::cout << opcode << std::endl;
			std::cout << "PC: " << program_counter << std::endl;
		}

		(this->*opcodes[opcode])();
	}
	
	void op_nop() {
		program_counter++;	
	}

	void op_ldi() {
		int reg = (memory[program_counter] & 1792) >> 8;
		int imm = memory[program_counter + 1];
	
		if (debug)
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
		
		if (debug)
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

		if (debug)
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

		if (call_stack_pointer != 0xF7EF){
			memory[call_stack_pointer] = program_counter + 1;
			call_stack_pointer--;
		} else {
			if (debug) std::cout << "Call Stack Overflow\n";
			memory[0xFFF1] = 3;
		}

		program_counter += 2;
	}

	void op_ret() {
		if (call_stack_pointer != 0xFBEF){
			call_stack_pointer++;
		} else {
			if (debug) std::cout << "Call Stack Underflow\n";
			memory[0xFFF1] = 4;
		}

		program_counter++;
	}

	void op_push() {
		int reg = (memory[program_counter] & 1792) >> 8;

		if (stack_pointer != 0xFBEF) { 
			memory[stack_pointer] = registers[reg];
			stack_pointer--;
			if (debug) std::cout << "Register " << reg << " pushed to stack" << std::endl;
		} else {
			std::cout << "Stack Overflow\n";
			memory[0xFFF1] = 1;
		}

		program_counter++;
	}

	void op_pop() {
		int reg = (memory[program_counter] & 1792) >> 8;

		if (stack_pointer != 0xFFEF) {
			stack_pointer++;
			registers[reg] = memory[stack_pointer];
			if (debug) std::cout << "Stack popped to register " << reg << std::endl;
		} else {
			if (debug) std::cout << "Stack Underflow\n";
			memory[0xFFF1] = 2;
		}

		program_counter++;
	}

	void op_mov() {
		int instruction = memory[program_counter];
		int reg_from = (instruction & 1792) >> 8;
		int reg_to = (instruction & 224) >> 5;
	
		registers[reg_to] = registers[reg_from];

		if (debug) std::cout << "Register " << reg_from << " copied to register " << reg_to << std::endl;
			
		program_counter++;
	}
	
	void op_add() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] + registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_sub() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] - registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;

	}

	void op_mul() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] * registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;
	
		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;

	}

	void op_div() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;
		
		if (registers[reg_in2] != 0) {
			int result = (int)(registers[reg_in1] / registers[reg_in2]) & 65535;

			registers[reg_out] = result;

			if (debug) 
				std::cout
					<< "Reg IN 1: " << reg_in1
					<< " Reg IN 2: " << reg_in2
					<< " Reg OUT: " << reg_out
					<< " Result: " << result
					<< std::endl;
		} else {
			memory[65521] = 5;
			if (debug) std::cout << "Division by zero\n";
		}

		program_counter++;
	}

	void op_mod() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		if (registers[reg_in2] != 0) {
			int result = (registers[reg_in1] % registers[reg_in2]) & 65535;

			registers[reg_out] = result;
			
			if (debug) 
				std::cout
					<< "Reg IN 1: " << reg_in1
					<< " Reg IN 2: " << reg_in2
					<< " Reg OUT: " << reg_out
					<< " Result: " << result
					<< std::endl;
		} else {
			memory[65521] = 5;
			if (debug) std::cout << "Division by zero\n";
		}
		
		program_counter++;
	}

	void op_inc() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] + 1) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN: " << reg_in1
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_dec() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] - 1) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN: " << reg_in1
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_shl() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] << 1) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN: " << reg_in1
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_shr() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] >> 1) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN: " << reg_in1
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_and() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] & registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_nand() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = ~(registers[reg_in1] & registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_or() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] | registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_nor() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = ~(registers[reg_in1] | registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Reg OUT: " << reg_out
				<< " Result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_xor() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;
		int reg_out = (instruction & 28) >> 2;

		int result = (registers[reg_in1] ^ registers[reg_in2]) & 65535;
		
		registers[reg_out] = result;

		if (debug) 
			std::cout
				<< "reg in 1: " << reg_in1
				<< " reg in 2: " << reg_in2
				<< " reg out: " << reg_out
				<< " result: " << result
				<< std::endl;

		program_counter++;
	}

	void op_cmp() {
		int instruction = memory[program_counter];
		int reg_in1 = (instruction & 1792) >> 8;
		int reg_in2 = (instruction & 224) >> 5;

		int result = (registers[reg_in1] - registers[reg_in2]) & 65535;

		if (result == 0)
			flags["zero"] = true;
		else
			flags["zero"] = false;

		if (result != 0)
			flags["not zero"] = true;
		else
			flags["not zero"] = false;

		if ((result & 32768) >> 15 == 1)
			flags["signed"] = true;
		else
			flags["signed"] = false;

		if ((result & 32768) >> 15 == 0)
			flags["not signed"] = true;
		else
			flags["not signed"] = false;

		if (debug) 
			std::cout
				<< "Reg IN 1: " << reg_in1
				<< " Reg IN 2: " << reg_in2
				<< " Result: " << result
				<< "\nZero Flag: " << flags["zero"]
				<< " Not Zero Flag: " << flags["not zero"]
				<< " Signed Flag: " << flags["signed"]
				<< " Not Signed Flag: " << flags["not signed"]
				<< std::endl;

		program_counter++;
	}

	int toTwosComplement(int number) {
		if (number & 1024) {
			return number - 2048;
		}
		return number;
	}

	void op_jmp() {
		int instruction = memory[program_counter];
		int offset = ~(instruction & 2047) + 1;

		if (debug) std::cout << "Jumped by: " << std::to_string(offset) << std::endl;

		program_counter += offset;
	}

	void op_brh_z() {
		int instruction = memory[program_counter];
		int offset = toTwosComplement(instruction & 2047);

		if (flags["zero"] == true) {
			program_counter += offset;
			if (debug) std::cout << "Jumped by: " << std::to_string(offset) << std::endl;
		} else {
			program_counter++;
			if (debug) std::cout << "Flag ZERO is false, didn't jump\n";
		}
	}

	void op_brh_nz() {
		int instruction = memory[program_counter];
		int offset = toTwosComplement(instruction & 2047);

		if (flags["not zero"] == true) {
			program_counter += offset;
			if (debug) std::cout << "Jumped by: " << std::to_string(offset) << std::endl;
		} else {
			program_counter++;
			if (debug) std::cout << "Flag NOT ZERO is false, didn't jump\n";
		}
	}

	void op_brh_s() {
		int instruction = memory[program_counter];
		int offset = toTwosComplement(instruction & 2047);

		if (flags["signed"] == true) {
			program_counter += offset;
			if (debug) std::cout << "Jumped by: " << std::to_string(offset) << std::endl;
		} else {
			program_counter++;
			if (debug) std::cout << "Flag SIGNED is false, didn't jump\n";
		}
	}

	void op_brh_ns() {
		int instruction = memory[program_counter];
		int offset = toTwosComplement(instruction & 2047);

		if (flags["not signed"] == true) {
			program_counter += offset;
			if (debug) std::cout << "Jumped by: " << std::to_string(offset) << std::endl;
		} else {
			program_counter++;
			if (debug) std::cout << "Flag NOT SIGNED is false, didn't jump\n";
		}
	}

	void op_halt() {
		running = false;
		if (debug) std::cout << "Halted\n";
	}
private:
	std::array<int, 65536> memory;
	std::array<int, 8> registers;
	std::map<std::string, bool> flags;
	int program_counter;
	int stack_pointer;
	int call_stack_pointer;
	using OpcodeFunction = void (ASTRISC::*)();
	std::array<OpcodeFunction, 32> opcodes;
	bool running;
	bool debug;
};

int main() {
	ASTRISC cpu;
	// cpu.debug_mode();
	cpu.run();
	return 0;
}

