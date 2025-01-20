#ifndef CPUGUI_H
#define CPUGUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <map>
#include <string>
#include <sstream>

using namespace sf;

class CpuWindow {
public:
	CpuWindow();

	void update(
			std::array <int, 8> registers, 
			std::map<std::string, bool> flags,
			std::array <int, 65536> &memory,
			int stack_pointer,
			int call_stack_pointer);
	std::string addPadding(std::string inital, int width, char paddingChar);
	bool isWindowOpen() { return m_window.isOpen(); }
	void closeWindow();
	void placePixel(std::array <int, 65536> &memory);
private:
	RenderWindow m_window;
	
	// Fonts
	Font m_sevensegment;
	Font m_jetbrains;

	// Output
	Texture m_panelpixels;
	RectangleShape m_panel;
	RectangleShape m_screen;
	RectangleShape m_segmentdisplay;
	Text m_segmentnumber;
	
	// Debug
	Text m_registers;
	Text m_flags;
	Text m_memory;
	Text m_memorylabels;
	Text m_programcounter;
	Text m_stack;
	Text m_callstack;
	Image m_panelpixelsimage;
};

#endif // CPUGUI_H

