#pragma once

#include <termios.h>

namespace TextEditor {

class TerminalSettings {
public:
	static int setRawMode();
	static int saveCurrent();
	static void reset();

private:
	struct TermInfo {
		TermInfo();
		struct termios orig;
		bool set;
	};
	// Contains necessary to reset the terminal settings on exit
	static bool isOriginalSet();
	static struct TermInfo terminfo;
	static struct termios getRawModeSettings();
};

}
