#include "termapi.hpp"
#include "util/util.hpp"
#include "util/exit_codes.hpp"
#include "util/mrlog.hpp"
#include <ncurses.h>
#include <array>

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
	const std::array<int (*)(), 3> table = {
		&raw,
		&nonl,
		&noecho
	};
	for (const auto& init : table) {
		if (init() == ERR) {
			return syscallError("setRawMode");
		}
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

void TermApi::erase() const {
	Point previous = getPreviousPosition();
	mrlog::info("Position: (%d, %d)", previous.x, previous.y);
	mvdelch(previous.y, previous.x);
}

TermApi::Point TermApi::getPreviousPosition() const {
	int y, x;
	getyx(stdscr, y, x);
	if (x == 0) {
		return Point(getmaxx(stdscr) - 1, y - 1);
	}
	return Point(x - 1, y);
}

}
