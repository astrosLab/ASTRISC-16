#include "cpugui.h"

CpuWindow::CpuWindow() {
	m_window.create(
			VideoMode(1920, 1080),
			"ASTRISC-16",
			Style::Close | Style::Fullscreen
			);

	if (!m_sevensegment.loadFromFile("./fonts/seven_segment.ttf")) {
		std::cout << "Unable to load font.\n";
	}

	if (!m_jetbrains.loadFromFile("./fonts/jetbrains.ttf")) {
		std::cout << "Unable to load font.\n";
	}

	m_registers.setFont(m_jetbrains);
	m_registers.setCharacterSize(24);
	m_registers.setFillColor(Color::Black);
	m_registers.setPosition({1000, 20});

	m_flags.setFont(m_jetbrains);
	m_flags.setCharacterSize(24);
	m_flags.setFillColor(Color::Black);
	m_flags.setPosition({1250, 70});

	m_memory.setFont(m_jetbrains);
	m_memory.setCharacterSize(15);
	m_memory.setFillColor(Color::Black);
	m_memory.setPosition({1050, 350});

	m_panel.setSize({940, 1045});
	m_panel.setPosition(20, 20);
	m_panel.setFillColor({39, 39, 40});
	m_panel.setOutlineColor(Color::Black);
	m_panel.setOutlineThickness(5);

	m_screen.setSize({900, 920});
	m_screen.setPosition(35, 125);
	m_screen.setOutlineColor(Color::White);
	m_screen.setOutlineThickness(2);

	m_segmentdisplay.setSize({200, 80});
	m_segmentdisplay.setPosition({35, 30});
	m_segmentdisplay.setFillColor(Color::Red);
	m_segmentdisplay.setOutlineColor(Color::White);
	m_segmentdisplay.setOutlineThickness(2);

	m_memorylabels.setFont(m_jetbrains);
	m_memorylabels.setCharacterSize(15);
	m_memorylabels.setFillColor(Color::Black);
	m_memorylabels.setPosition({1005, 350});

	m_segmentnumber.setPosition({45, 40});
	m_segmentnumber.setFont(m_sevensegment);
	m_segmentnumber.setCharacterSize(55);
	m_segmentnumber.setFillColor(Color::White);

	m_programcounter.setPosition({1250, 20});
	m_programcounter.setCharacterSize(24);
	m_programcounter.setFont(m_jetbrains);
	m_programcounter.setFillColor(Color::Black);

	m_stack.setPosition({1450, 20});
	m_stack.setCharacterSize(24);
	m_stack.setFont(m_jetbrains);
	m_stack.setFillColor(Color::Black);

	m_callstack.setPosition({1650, 20});
	m_callstack.setCharacterSize(24);
	m_callstack.setFont(m_jetbrains);
	m_callstack.setFillColor(Color::Black);

	m_panelpixelsimage.create(32, 32, Color::Black);
}

void CpuWindow::update(
		std::array <int, 8> registers, 
		std::map<std::string, bool> flags,
		std::array <int, 65536> &memory,
		int stack_pointer,
		int call_stack_pointer) {
	Event event;

	while (m_window.pollEvent(event)) {
		if (event.type == Event::Closed)
			m_window.close();
		if (event.key.code == Keyboard::Q)
			m_window.close();
	}

	m_window.clear(sf::Color::White);

	std::string reg_ss;
	for (int i = 0; i < 8; ++i) {
		char reghex[4];
		std::sprintf(reghex, "%X", registers[i]);
		std::string hex = addPadding(reghex, 4, '0');
		reg_ss.append(
				"R" + std::to_string(i) + ": " + hex + " (" +
				addPadding(std::to_string(registers[i]), 5, ' ') + ")\n"); 
	}
	m_registers.setString(reg_ss);
	
	std::stringstream flag_ss;
	flag_ss
		<< "Zero: " << flags["zero"]
		<< "\nNot Zero: " << flags["not zero"]
		<< "\nSigned: " << flags["signed"]
		<< "\nNot Signed: " << flags["not singed"];
	m_flags.setString(flag_ss.str());

	std::string memory_ss;

	for (int i = 0; i < 512; ++i) {
		if (i % 16 == 0) {
			memory_ss.append("\n");
		}
		char formatted[4];
		std::sprintf(formatted, "%X", memory[i]);
		std::string padded = addPadding(formatted, 4, '0');
		memory_ss.append(padded);
		memory_ss.append(" ");
	}
	m_memory.setString(memory_ss);
	m_window.draw(m_memory);

	std::string grid = "      ";

	char hex[4];

	for (int i = 0; i < 16; ++i) {
		std::sprintf(hex, "%X", i);
		grid += addPadding(hex, 2, '0');
		grid += "   ";
	}
	grid += '\n';

	for (int row = 0; row < 32; ++row) {
		std::sprintf(hex, "%X", row * 16);
		grid += addPadding(hex, 4, '0') + '\n';
	}	

	m_memorylabels.setString(grid);

	m_segmentnumber.setString(addPadding(std::to_string(memory[0xFFF2]), 5, '0'));
	m_programcounter.setString(
			"PC: " + addPadding(std::to_string(memory[0xFFF0]), 5, '0')
			);

	std::string stack_ss;
	stack_ss.append("Stack:\n");
	for (int i = 0xFFEF; i > stack_pointer; --i) {
		stack_ss.append(std::to_string(memory[i]) + "\n");
	}
	
	std::string callstack_ss;
	callstack_ss.append("Call Stack:\n");
	for (int i = 0xFBEF; i > call_stack_pointer; --i) {
		callstack_ss.append(std::to_string(memory[i]) + "\n");
	}

	m_stack.setString(stack_ss);
	m_callstack.setString(callstack_ss);

	placePixel(memory);
	m_screen.setTexture(&m_panelpixels);

	// Output
	m_window.draw(m_panel);
	m_window.draw(m_screen);
	m_window.draw(m_segmentdisplay);
	m_window.draw(m_segmentnumber);

	// Debug
	m_window.draw(m_memorylabels);
	m_window.draw(m_registers);
	m_window.draw(m_flags);
	m_window.draw(m_programcounter);
	m_window.draw(m_stack);
	m_window.draw(m_callstack);

	m_window.display();
}

std::string CpuWindow::addPadding(std::string inital, int width, char paddingChar) {
	return inital.insert(0, width > inital.length() ? width - inital.length() : 0, paddingChar);
}

void CpuWindow::placePixel(std::array <int, 65536> &memory) {
	int color = memory[0xFFF3];
	int function = memory[0xFFF4];
	int parameters = memory[0xFFF5];

	if (function == 1) {
		int b = (color & 31774) >> 10;
		int g = (color & 992) >> 5;
		int r = (color & 31);
		int r8 = static_cast<int>((r / 31.0) * 255);
		int g8 = static_cast<int>((g / 31.0) * 255);
		int b8 = static_cast<int>((b / 31.0) * 255);
		Color converted = Color(r8, g8, b8);
		int x = parameters & 31;
		int y = (parameters & 992) >> 5;

		//std::cout
		//	<< "Drawn pixel to: " << x << " " << y
		//	<< " Color is: " << r8 << " " << g8 << " " << b8
		//	<< std::endl;

		m_panelpixelsimage.setPixel(x, y, converted);
		memory[0xFFF4] = 0;
	}

	m_panelpixels.loadFromImage(m_panelpixelsimage);
}

void CpuWindow::closeWindow() {
	m_window.close();
}

