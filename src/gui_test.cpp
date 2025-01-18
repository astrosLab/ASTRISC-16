#include "cpugui.h"

int main() {
	CpuWindow gui;

	int registers[8] = {
		1, 2, 3, 4,	5, 6, 7, 8
	};

	while (gui.isWindowOpen()) {
		gui.update(registers);
		registers[0]++;
	}

	return 0;
}

