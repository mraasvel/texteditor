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
	using ConstLineIterator = LinesType::const_iterator;

	struct PositionType {
		PositionType(const LinesType& lines);
		ConstLineIterator line;
		ConstLineIterator end;
		std::size_t index;
	};
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

	/*
	When we scroll up or down,
	we need to know the linesize to determine whether we have to go to a previous index or previous line */
	void cornerDown(std::size_t linesize);
	void cornerUp(std::size_t linesize);
	static char nextChar(PositionType& position);
	PositionType getTopleft() const;

private:
	/*
	Very important that current is below lines (order of initializaiton) */
	LinesType lines;
	LineIterator current;
	PositionType topleft; // the character in the topleft corner
};

}
