#include "termapi.hpp"
#include "util/util.hpp"
#include "util/exit_codes.hpp"
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
	if (noecho() == ERR) {
		return syscallError("noecho");
	}
	if (keypad(screen, true) == ERR) {
		return syscallError("keypad");
	}
	return ExitCode::SUCCESS;
}

}
