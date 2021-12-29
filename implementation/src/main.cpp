#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "termapi.hpp"
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
		return syscallError("atexit");
	}
	if (TerminalSettings::saveCurrent() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return TerminalSettings::setRawMode();
}

static int run() {
	mrlog::clear();
	if (!isatty(STDIN_FILENO)) {
		return syscallError("isatty");
	}
	TermApi api;
	if (api.init() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}

	while (true) {
		int n = getch();
		if (n == 'q') {
			break;
		}
		mrlog::log("%d", n);
		attron(A_BOLD);
		printw("%c", n);
		attroff(A_BOLD);
		refresh();
	}
	return ExitCode::SUCCESS;
}

}

int main() {
	return TextEditor::run();
}
