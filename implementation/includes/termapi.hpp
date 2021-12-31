#pragma once

// TODO: determine if this can be removed
#include <ncurses.h>
#include <string>

namespace TextEditor {

class TermApi {
public:
	TermApi();
	~TermApi();

	int init();

	// Returns the value of a keysequence
	int getchar() const;
	void put(int ch) const;
	void put(const std::string& s) const;

private:
	int setRawMode() const;

private:
	WINDOW* screen;
};

}
