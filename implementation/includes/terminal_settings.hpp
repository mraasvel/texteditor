#pragma once

#include <termios.h>

namespace TextEditor {

class TerminalSettings {
public:
	static int setRawMode();
	static int saveCurrent();
	static void reset();

private:
	static bool isOriginalSet();
	static struct termios getRawModeSettings();

private:
	// Contains necessary to reset the terminal settings on exit
	struct TermInfo {
		TermInfo();
		struct termios orig;
		bool set;
	};

	static struct TermInfo terminfo;
};

}
