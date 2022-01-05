#pragma once

#include "line.hpp"
#include <list>

namespace TextEditor {

/*
Movement:
	- left
	- right
Adding new lines:
	- move post to the new line
	- Before
	- After
Pushbacking/Erasing/Inserting/Deleting chars

Post/Pre empty
*/

class Lines {
private:
	using LinesType = std::list<Line>;
	using LineIterator = LinesType::iterator;
public:
	Lines();
	void insertNewline();
	std::size_t moveleft(std::size_t n = 1);
	std::size_t moveright(std::size_t n = 1);
	void push(int c);
	void push(const std::string& s);
	void erase();
	void insert(int c);
	void del();
	bool postEmpty() const;
	bool preEmpty() const;
	void logcurrent() const;
private:
	/*
	Very important that current is below lines (order of initializaiton) */
	LinesType lines;
	LineIterator current;
};

}
