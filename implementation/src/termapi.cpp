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
	if (keypad(stdscr, true) == ERR) {
		return syscallError("keypad");
	} else if (scrollok(stdscr, true) == ERR) {
		return syscallError("scrollok");
	}
	return ExitCode::SUCCESS;
}

int TermApi::getchar() const {
	return getch();
}

void TermApi::put(int ch) const {
	addch(ch);
	refresh();
}

void TermApi::put(const std::string& s) const {
	addstr(s.c_str());
	refresh();
}

void TermApi::insert(const std::string& s) const {
	Point original = getPosition();
	put(s);
	setPosition(original);
}

void TermApi::erase() const {
	setPosition(getPreviousPosition());
	delch();
}

void TermApi::delchar() const {
	delch();
}

/*
Put string at previous position, erase first character after
*/
void TermApi::slideleft(const std::string& s) const {
	Point position = getPreviousPosition();
	setPosition(position);
	put(s);
	delchar();
	setPosition(position);
}

void TermApi::moveleft(int n) const {
	while (n-- > 0) {
		setPosition(getPreviousPosition());
	}
}

void TermApi::moveright(int n) const {
	while (n-- > 0) {
		setPosition(getNextPosition());
	}
}

void TermApi::setPosition(const Point& p) const {
	move(p.y, p.x);
}

TermApi::Point TermApi::getPosition() const {
	int y, x;
	getyx(stdscr, y, x);
	return Point(x, y);
}

TermApi::Point TermApi::getPreviousPosition() const {
	int y, x;
	getyx(stdscr, y, x);
	if (x == 0) {
		return Point(getmaxx(stdscr) - 1, y - 1);
	}
	return Point(x - 1, y);
}

TermApi::Point TermApi::getNextPosition() const {
	int y, x;
	getyx(stdscr, y, x);
	if (x == getmaxx(stdscr) - 1) {
		return Point(0, y + 1);
	}
	return Point(x + 1, y);
}

void TermApi::scrollDown() const {
	scrl(1);
}

void TermApi::scrollUp() const {
	scrl(-1);
}

// Returns number of characters left on the screen
std::size_t TermApi::charactersLeftLine() const {
	return getmaxx(stdscr) - getPosition().x - 1;
}

std::size_t TermApi::charactersLeftScreen() const {
	return charactersLeftLine() +
		(getmaxy(stdscr) - getPosition().y - 1) * getmaxx(stdscr);
}

}
