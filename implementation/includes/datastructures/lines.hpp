#pragma once

#include "line.hpp"
#include <list>
#include <fstream>

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
		PositionType(const ConstLineIterator& line, const ConstLineIterator& end, std::size_t index);
		std::size_t beforeIndex() const;
		std::size_t afterIndex() const;
		ConstLineIterator line;
		ConstLineIterator end;
		std::size_t index;
	};
public:
	Lines();
	Lines(Lines&& other);
	int fromFile(const std::string& filename);
	void insertPostString(const std::string& post);
	void insertNewline();
	void pushNewline();
	bool moveleft();
	bool moveright();
	bool moveNextLine();
	std::size_t moveStart();
	std::size_t moveEnd();
	bool moveDown(std::size_t linesize);
	bool moveUp(std::size_t linesize);
	void push(int c);
	void push(const std::string& s);
	bool erase();
	void insert(int c);
	void insert(const std::string& s);
	bool del();
	bool postEmpty() const;
	bool preEmpty() const;

	void cornerDown(std::size_t linesize);
	void cornerUp(std::size_t linesize);
	static char nextChar(PositionType& position);
	PositionType getTopleft() const;
	bool isCursor(const PositionType& pos) const;

	void recalibrateWindow(std::size_t linesize, std::size_t rows);

	const std::string& getFilename() const;
	int saveToFile() const;

/* Logging */
	void logcurrent() const;
	void log() const;

private:
	void eraseLine(LineIterator pos);
	std::size_t distanceToCursor(std::size_t linesize) const;

private:
	/*
	Very important that current is below lines (order of initializaiton) */
	LinesType lines;
	LineIterator current;
	PositionType topleft; // the character in the topleft corner
	std::string filename;
};

}
