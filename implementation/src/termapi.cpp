#include "termapi.hpp"
#include "util/util.hpp"
#include "util/exit_codes.hpp"
#include "util/mrlog.hpp"
#include "line.hpp"
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

void TermApi::render(const std::string& pre, const std::string& post) const {
	clearWindow();
	putPre(pre);
	putPost(post);
	refresh();
}

void TermApi::putPre(const std::string& pre) const {
	Point orig = getPosition();
	std::size_t bytes = charactersBeforeCursor();
	setPosition(Point(0, 0));
	if (bytes < pre.size()) {
		put(pre.substr(pre.size() - bytes));
	} else {
		put(pre);
	}
	setPosition(orig);
}

void TermApi::putPost(const std::string& post) const {
	Point orig = getPosition();
	std::size_t bytes = charactersAfterCursor() + 1;
	auto rit = post.crbegin();
	while (bytes-- > 0 && rit != post.crend()) {
		if (bytes == 0) {
			insch(*rit);
		} else {
			put(*rit);
		}
		rit++;
	}
	setPosition(orig);
}

int TermApi::getchar() const {
	return getch();
}

void TermApi::put(int ch) const {
	addch(ch);
}

void TermApi::put(const std::string& s) const {
	addstr(s.c_str());
}

void TermApi::insert(const std::string& s) const {
	Point original = getPosition();
	put(s);
	setPosition(original);
}

void TermApi::erase() const {
	moveleft();
	delch();
}

void TermApi::delchar() const {
	delch();
}

/*
Put string at previous position, erase first character after
*/
void TermApi::slideleft(const std::string& s) const {
	moveleft();
	Point pos = getPosition();
	put(s);
	delchar();
	setPosition(pos);
}

void TermApi::moveleft(int n) const {
	while (n-- > 0) {
		Point pos = getPreviousPosition();
		if (pos.y == -1) {
			scrollUp();
			setPosition(Point(pos.x, 0));
		} else {
			setPosition(getPreviousPosition());
		}
	}
}

void TermApi::moveright(int n) const {
	while (n-- > 0) {
		Point pos = getNextPosition();
		if (pos.y == getmaxy(stdscr)) {
			scrollDown();
			setPosition(Point(0, pos.y - 1));
		} else {
			setPosition(getNextPosition());
		}
	}
}

void TermApi::moveup(int n) const {
	while (n-- > 0) {
		Point pos = getPosition();
		if (pos.y > 0) {
			setPosition(Point(pos.x, pos.y - 1));
		}
	}
}

void TermApi::movedown(int n) const {
	while (n-- > 0) {
		Point pos = getPosition();
		if (pos.y < getmaxy(stdscr) - 1) {
			setPosition(Point(pos.x, pos.y + 1));
		}
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

std::size_t TermApi::charactersBeforeCursor() const {
	Point pos = getPosition();
	return pos.x + pos.y * getmaxx(stdscr);
}

std::size_t TermApi::charactersAfterCursor() const {
	return charactersLeftScreen();
}

void TermApi::clearWindow() const {
	Point pos = getPosition();
	setPosition(Point(0, 0));
	for (int y = 0; y < getmaxy(stdscr); y++) {
		for (int x = 0; x < getmaxx(stdscr); x++) {
			put(' ');
		}
	}
	setPosition(pos);
}

int TermApi::getLineSize() const {
	return getmaxx(stdscr);
}

}
