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
	bool moveleft();
	bool moveright();
	void push(int c);
	void push(const std::string& s);
	bool erase();
	void insert(int c);
	bool del();
	bool postEmpty() const;
	bool preEmpty() const;
	void logcurrent() const;
	void log() const;

	/*
	When we scroll up or down,
	we need to know the linesize to determine whether we have to go to a previous index or previous line */
	void cornerDown(std::size_t linesize);
	void cornerUp(std::size_t linesize);
	static char nextChar(PositionType& position);
	PositionType getTopleft() const;
	bool isCursor(const PositionType& pos) const;

private:
	void eraseLine(LineIterator pos);

private:
	/*
	Very important that current is below lines (order of initializaiton) */
	LinesType lines;
	LineIterator current;
	PositionType topleft; // the character in the topleft corner
	std::size_t line_index;
};

}
