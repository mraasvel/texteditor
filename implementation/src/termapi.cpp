#include "termapi.hpp"
#include "util/util.hpp"
#include "util/exit_codes.hpp"
#include "util/mrlog.hpp"
#include <ncurses.h>

namespace TextEditor {

TermApi::TermApi()
: screen(nullptr) {
}

TermApi::~TermApi() {
	if (screen != nullptr) {
		if (endwin() == ERR) {
			syscallError("endwin");
		}
	}
}

int TermApi::init() {
	screen = initscr();
	if (screen == nullptr) {
		return syscallError("initscr");
	}
	return setRawMode();
}

int TermApi::setRawMode() const {
	if (raw() == ERR) {
		return syscallError("raw");
	}
	if (nonl() == ERR) {
		return syscallError("nonl");
	}
	if (noecho() == ERR) {
		return syscallError("noecho");
	}
	if (keypad(screen, true) == ERR) {
		return syscallError("keypad");
	}
	return ExitCode::SUCCESS;
}

int TermApi::getchar() const {
	return getch();
}

void TermApi::put(int ch) const {
	addch(ch);
	if (refresh() == ERR) {
		mrlog::error("putchar: refresh error");
	}
}

void TermApi::put(const std::string& s) const {
	addstr(s.c_str());
	if (refresh() == ERR) {
		mrlog::error("putchar: refresh error");
	}
}

}
