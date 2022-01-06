#pragma once

// TODO: determine if this can be removed
#include "util/point.hpp"
#include "datastructures/lines.hpp"
#include <ncurses.h>
#include <string>

#define K_CTRL_CHAR(c) ((c) & 0x1F)

namespace TextEditor {

class TermApi {
public:
	enum class Keys : int32_t {
		K_NEWLINE		= 13,
		K_BACKSPACE		= KEY_BACKSPACE,
		K_DELETEC		= KEY_DC,
		K_ARROW_DOWN	= KEY_DOWN,
		K_ARROW_UP		= KEY_UP,
		K_ARROW_LEFT	= KEY_LEFT,
		K_ARROW_RIGHT	= KEY_RIGHT,
		K_SARROW_DOWN	= KEY_SF,
		K_SARROW_UP		= KEY_SR,
		K_SARROW_LEFT	= KEY_SLEFT,
		K_SARROW_RIGHT	= KEY_SRIGHT,
		K_ESCAPE		= 27,
		K_CTRL_Q		= K_CTRL_CHAR('q'),
		K_CTRL_C		= K_CTRL_CHAR('c'),
		K_CTRL_V		= K_CTRL_CHAR('v'),
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
	void delchar() const;
	void slideleft(const std::string& s) const;

	void moveleft(int n = 1) const;
	void moveright(int n = 1) const;
	void moveup(int n = 1) const;
	void movedown(int n = 1) const;

	void render(const Lines& lines) const;

	void scrollDown() const;
	void scrollUp() const;
	int getLineSize() const;

private:
	int setRawMode() const;
	void setPosition(const Point& p) const;
	Point getPosition() const;
	Point getPreviousPosition() const;
	Point getNextPosition() const;
	std::size_t charactersLeftLine() const;
	std::size_t charactersLeftScreen() const;
	std::size_t charactersBeforeCursor() const;
	std::size_t charactersAfterCursor() const;
	void putPre(const std::string& pre) const;
	void putPost(const std::string& pre) const;
	void clearWindow() const;
private:
	WINDOW* screen;
};

}
