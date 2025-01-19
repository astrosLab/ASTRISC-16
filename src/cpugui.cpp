#include "cpugui.h"

CpuWindow::CpuWindow() {
	m_window.create(
			VideoMode(1920, 1080),
			"ASTRISC-16",
			Style::Close
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
}

void CpuWindow::update(int registers[8]) {
	Event event;

	while (m_window.pollEvent(event)) {
		if (event.type == Event::Closed)
			m_window.close();
	}

	m_window.clear(sf::Color::White);

	std::stringstream ss;
	ss 
		<< "R0: " << addPadding(std::to_string(registers[0]), 5, ' ')
		<< "\nR1: " << addPadding(std::to_string(registers[1]), 5, ' ')
		<< "\nR2: " << addPadding(std::to_string(registers[2]), 5, ' ')
		<< "\nR3: " << addPadding(std::to_string(registers[3]), 5, ' ')
		<< "\nR4: " << addPadding(std::to_string(registers[4]), 5, ' ')
		<< "\nR5: " << addPadding(std::to_string(registers[5]), 5, ' ')
		<< "\nR6: " << addPadding(std::to_string(registers[6]), 5, ' ')
		<< "\nR7: " << addPadding(std::to_string(registers[7]), 5, ' ');
	m_registers.setString(ss.str());
	m_window.draw(m_registers);

	RectangleShape rect({940, 1045});
	rect.setPosition(20, 20);
	rect.setFillColor({39, 39, 40});
	rect.setOutlineColor(Color::Black);
	rect.setOutlineThickness(5);

	RectangleShape screen({900, 920});
	screen.setPosition(35, 125);
	screen.setFillColor(Color::Black);
	screen.setOutlineColor(Color::White);
	screen.setOutlineThickness(2);

	RectangleShape segment({200, 80});
	segment.setPosition({35, 30});
	segment.setFillColor(Color::Red);
	segment.setOutlineColor(Color::White);
	segment.setOutlineThickness(2);

	Text numbers;
	numbers.setPosition({45, 40});
	
	numbers.setString(addPadding(std::to_string(registers[0]), 5, '0'));
	numbers.setFont(m_sevensegment);
	numbers.setCharacterSize(55);
	numbers.setFillColor(Color::White);

	m_window.draw(rect);
	m_window.draw(screen);
	m_window.draw(segment);
	m_window.draw(numbers);

	m_window.display();
}

std::string CpuWindow::addPadding(std::string inital, int width, char paddingChar) {
	return inital.insert(0, width > inital.length() ? width - inital.length() : 0, paddingChar);
}

