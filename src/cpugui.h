#ifndef CPUGUI_H
#define CPUGUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <string>
#include <sstream>

using namespace sf;

class CpuWindow {
public:
	CpuWindow();

	void update(int registers[8]);
	std::string addPadding(std::string inital, int width, char paddingChar);
	bool isWindowOpen() { return m_window.isOpen(); }
private:
	RenderWindow m_window;
	Font m_sevensegment;
	Font m_jetbrains;
	Text m_registers;
};

#endif // CPUGUI_H

