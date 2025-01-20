#include "cpugui.h"

int main() {
	CpuWindow gui;

	int registers[8] = {
		1, 2, 3, 4,	5, 6, 7, 8
	};

	bool flags[4] = {
		false, true, true, false
	};

	while (gui.isWindowOpen()) {
		// gui.update(registers, flags);
	}

	return 0;
}

