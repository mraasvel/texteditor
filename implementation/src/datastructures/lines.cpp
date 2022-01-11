#include "datastructures/lines.hpp"
#include "util/mrlog.hpp"
#include "text_editor.hpp"
#include "util/util.hpp"
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace TextEditor {


Lines::PositionType::PositionType(const LinesType& lines)
: line(lines.cbegin()), end(lines.cend()), index(0) {}

Lines::PositionType::PositionType(const ConstLineIterator& line, const ConstLineIterator& end, std::size_t index)
: line(line), end(end), index(index) {}

std::size_t Lines::PositionType::beforeIndex() const {
	return index;
}

std::size_t Lines::PositionType::afterIndex() const {
	return line->size() - index;
}

/*
Invariant: there is always at least one empty line */

Lines::Lines()
: lines(1), current(lines.begin()), topleft(lines) {
	if (lines.begin() != current) {
		throw std::runtime_error("Lines: constructor failed because of member variable order");
	}
}

Lines::Lines(Lines&& other)
: lines(other.lines), current(lines.begin()), topleft(lines) {}

void Lines::insertPostString(const std::string& post) {
	for (auto rit = post.rbegin(); rit != post.rend(); ++rit) {
		if (*rit == '\t') {
			insert("    ");
		} else {
			insert(*rit);
		}
	}
}

int Lines::fromFile(const std::string& filename) {
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		return syscallError("ifstream::open: " + filename);;
	}
	std::string post;
	while (std::getline(ifs, post)) {
		insertPostString(post);
		pushNewline();
	}
	ifs.close();
	current = lines.begin();
	return ExitCode::SUCCESS;
}

void Lines::insertNewline() {
	lines.emplace(std::next(current), Line(current->movePost()));
	++current;
}

void Lines::pushNewline() {
	lines.emplace(std::next(current));
	++current;
}

bool Lines::moveleft() {
	if (current->preEmpty()) {
		if (current == lines.begin()) {
			return false;
		}
		--current;
		current->moveEnd();
	} else {
		current->moveleft();
	}
	return true;
}

bool Lines::moveright() {
	if (current->postEmpty()) {
		return false;
	}
	current->moveright();
	return true;
}

bool Lines::moveNextLine() {
	if (std::next(current) == lines.end()) {
		return false;
	}
	++current;
	current->moveStart();
	return true;
}

std::size_t Lines::moveStart() {
	return current->moveStart();
}

std::size_t Lines::moveEnd() {
	return current->moveEnd();
}

bool Lines::moveDown(std::size_t linesize) {
	std::size_t index = current->getPre().size();
	std::size_t after = index % linesize + current->getPost().size();
	if (after >= linesize) {
		current->moveright(linesize);
	} else if (std::next(current) != lines.end()) {
		++current;
		current->moveStart();
		current->moveright(index % linesize);
	} else {
		current->moveEnd();
		return false;
	}
	return true;
}

bool Lines::moveUp(std::size_t linesize) {
	std::size_t index = current->getPre().size();
	if (index >= linesize) {
		current->moveleft(linesize);
	} else if (current != lines.begin()) {
		--current;
		current->moveTo(current->size() - (current->size() % linesize));
		current->moveright(index);
	} else {
		current->moveStart();
		return false;
	}
	return true;
}

void Lines::push(int c) {
	current->push(c);
}

void Lines::push(const std::string& s) {
	current->push(s);
}

/*
Return false if nothing was done */
bool Lines::erase() {
	if (current->preEmpty()) {
		if (current != lines.begin()) {
			eraseLine(current--);
		} else {
			return false;
		}
	} else {
		current->erase();
	}
	return true;
}

void Lines::eraseLine(LineIterator pos) {
	assert(pos != lines.begin());
	LineIterator prev {std::prev(pos)};
	prev->moveEnd();
	prev->insert(pos->getPost());
	lines.erase(pos);
}


void Lines::insert(int c) {
	current->insert(c);
}

void Lines::insert(const std::string& s) {
	current->insert(s);
}

bool Lines::del() {
	if (current->postEmpty()) {
		return false;
	}
	current->del();
	return true;
}

bool Lines::postEmpty() const {
	return current->postEmpty();
}

bool Lines::preEmpty() const {
	return current->preEmpty();
}

void Lines::cornerDown(std::size_t linesize) {
	if (topleft.line->size() - topleft.index >= linesize) {
		topleft.index += linesize;
	} else {
		++topleft.line;
		topleft.index = 0;
		if (topleft.line == lines.end()) {
			mrlog::fatal("cornerDown: cannot move further down\n");
			throw std::runtime_error("corner down");
		}
	}
}

void Lines::cornerUp(std::size_t linesize) {
	if (topleft.index >= linesize) {
		topleft.index -= linesize;
	} else {
		if (topleft.line == lines.begin()) {
			mrlog::fatal("cornerUp: cannot move further up\n");
			throw std::runtime_error("corner up");
		}
		--topleft.line;
		topleft.index = topleft.line->size() - (topleft.line->size() % linesize);
		if (topleft.index > topleft.line->size()) {
			mrlog::fatal("cornerUp: topleft index miscalculation\n");
			mrlog::info("index: {}, topleft.linesize: {}\n", topleft.index, topleft.line->size());
			throw std::runtime_error("corner up");
		}
	}
}

Lines::PositionType Lines::getTopleft() const {
	return topleft;
}

bool Lines::isCursor(const PositionType& pos) const {
	return pos.line == current && pos.index == current->getPre().size();
}

/*
Move topleft so that the current cursor position fits inside the window */
void Lines::recalibrateWindow(std::size_t linesize, std::size_t rows) {
	auto distance {distanceToCursor(linesize) + 1};
	while (distance-- > rows) {
		cornerDown(linesize);
	}
}

std::size_t Lines::distanceToCursor(std::size_t linesize) const {
	auto it = topleft.line;
	std::size_t distance {0};
	while (it != current) {
		distance += it->getPre().size() / linesize;
		++distance;
		++it;
	}
	distance += it->getPre().size() / linesize;
	return distance;
}

/*
Return: '\n' (LF) char on end of line, EOF on no lines remaining */
char Lines::nextChar(PositionType& position) {
	if (position.index == position.line->size()) {
		++position.line;
		if (position.line == position.end) {
			return 0;
		}
		position.index = 0;
		return '\n';
	}
	return position.line->operator[](position.index++);
}

void Lines::log() const {
	mrlog::info("Lines\n");
	for (auto it = lines.begin(); it != lines.end(); ++it) {
		mrlog::log("  ");
		it->log();
		if (it == current) {
			mrlog::log("  <- Current");
		}
		if (it == topleft.line) {
			mrlog::log(" <- Topleft Corner");
		}
		mrlog::log("\n");
	}
	topleft.line->log();
	mrlog::log("  <- Topleft: {}", topleft.index);
	mrlog::log("\n");
}

void Lines::logcurrent() const {
	current->log();
}

}
