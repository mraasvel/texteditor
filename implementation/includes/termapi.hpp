#pragma once

// TODO: determine if this can be removed
#include <ncurses.h>

namespace TextEditor {

class TermApi {
public:
	TermApi();
	~TermApi();

	int init();

private:
	int setRawMode() const;

private:
	WINDOW* screen;
};

}
