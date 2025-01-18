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

	for (int i = 0; i < 8; ++i) {
		m_registers[i].setCharacterSize(24);
		m_registers[i].setFont(m_sevensegment);
		m_registers[i].setFillColor(Color::Black);
		m_registers[i].setPosition(i * 100 + 500, 200);
	}
}

void CpuWindow::update(int registers[8]) {
	Event event;

	while (m_window.pollEvent(event)) {
		if (event.type == Event::Closed)
			m_window.close();
	}

	m_window.clear(sf::Color::White);

	for (int i = 0; i < 8; ++i) {
		m_registers[i].setString(std::to_string(registers[i]));
		m_window.draw(m_registers[i]);
		std::string reg = m_registers[i].getString();
	}

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
	numbers.setString(std::to_string(registers[0]));
	numbers.setFont(m_sevensegment);
	numbers.setCharacterSize(55);
	numbers.setFillColor(Color::White);

	m_window.draw(rect);
	m_window.draw(screen);
	m_window.draw(segment);
	m_window.draw(numbers);

	m_window.display();
}

