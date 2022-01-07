#include "datastructures/lines.hpp"
#include "util/mrlog.hpp"
#include <stdexcept>
#include <cassert>

namespace TextEditor {


Lines::PositionType::PositionType(const LinesType& lines)
: line(lines.cbegin()), end(lines.cend()), index(0) {}

/*
Invariant: there is always at least one empty line */

Lines::Lines()
: lines(1), current(lines.begin()), topleft(lines), line_index(0) {
	if (lines.begin() != current) {
		throw std::runtime_error("Lines: constructor failed because of member variable order");
	}
}

void Lines::insertNewline() {
	lines.insert(std::next(current), Line(current->movePost()));
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
		if (std::next(current) == lines.end()) {
			return false;
		}
		++current;
		current->moveStart();
	} else {
		current->moveright();
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
		if (topleft.index >= topleft.line->size()) {
			mrlog::fatal("cornerUp: topleft index miscalculation\n");
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
		mrlog::log("\n");
	}
}

void Lines::logcurrent() const {
	current->log();
}

}
