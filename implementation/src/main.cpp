#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <array>
#include <ncurses.h>

// Returns the value of a given control character
#define CTRL_KEY(key) ((key) & 0x1F)

namespace TextEditor {

static int setTerminalMode() {
	if (atexit(TerminalSettings::reset) != 0) {
		syscallError("atexit");
		return ExitCode::ERROR;
	}
	if (TerminalSettings::saveCurrent() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return TerminalSettings::setRawMode();
}

static int run() {
	if (!isatty(STDIN_FILENO)) {
		syscallError("isatty");
		return ExitCode::ERROR;
	}
	if (setTerminalMode() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return ExitCode::SUCCESS;
}

}

int main() {
	return TextEditor::run();
}
