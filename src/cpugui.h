#ifndef CPUGUI_H
#define CPUGUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <string>

using namespace sf;

class CpuWindow {
public:
	CpuWindow();

	void update(int registers[8]);
	bool isWindowOpen() { return m_window.isOpen(); }
private:
	RenderWindow m_window;
	Font m_sevensegment;
	std::array<Text, 8> m_registers;
};

#endif // CPUGUI_H

