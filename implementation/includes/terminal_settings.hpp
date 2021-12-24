#pragma once

#include <termios.h>

namespace TextEditor {

class TerminalSettings {
public:
	static int set();
	static void reset();
private:
	struct TermInfo {
		TermInfo();
		struct termios orig;
		bool set;
	};
	// Contains necessary to reset the terminal settings on exit
	static struct TermInfo terminfo;
};

}
