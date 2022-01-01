#pragma once

// TODO: determine if this can be removed
#include "util/point.hpp"
#include <ncurses.h>
#include <string>

#define K_CTRL_CHAR(c) ((c) & 0x1F)

namespace TextEditor {

class TermApi {
public:
	enum Keys {
		K_NEWLINE		= 13,
		K_BACKSPACE		= KEY_BACKSPACE,
		K_DELETEC		= KEY_DC,
		K_ARROW_DOWN	= KEY_DOWN,
		K_ARROW_UP		= KEY_UP,
		K_ARROW_LEFT	= KEY_LEFT,
		K_ARROW_RIGHT	= KEY_RIGHT,
		K_ESCAPE		= 27,
		K_CTRL_Q		= K_CTRL_CHAR('q'),
		K_WINCH			= KEY_RESIZE,
	};

private:
	typedef util::Point<int> Point;

public:
	TermApi();
	~TermApi();

	int init();

	// Returns the value of a keysequence
	int getchar() const;
	void put(int ch) const;
	void put(const std::string& s) const;
	void insert(const std::string& s) const;
	void erase() const;

	void moveleft(int n = 1) const;
	void moveright(int n = 1) const;

private:
	int setRawMode() const;
	void setPosition(const Point& p) const;
	Point getPosition() const;
	Point getPreviousPosition() const;
	Point getNextPosition() const;

private:
	WINDOW* screen;
};

}
