#include "datastructures/lines.hpp"
#include <cassert>

namespace TextEditor {


Lines::PositionType::PositionType(const LinesType& lines)
: line(lines.cbegin()), end(lines.cend()), index(0) {}

/*
Invariant: there is always at least one empty line */

Lines::Lines()
: lines(1), current(lines.begin()), topleft(lines) {
	assert(current == lines.begin());
}

void Lines::insertNewline() {
	lines.insert(std::next(current), Line(current->movePost()));
	++current;
}

std::size_t Lines::moveleft(std::size_t n) {
	return current->moveleft(n);
}

std::size_t Lines::moveright(std::size_t n) {
	return current->moveright(n);
}

void Lines::push(int c) {
	current->push(c);
}

void Lines::push(const std::string& s) {
	current->push(s);
}

void Lines::erase() {
	current->erase();
}

void Lines::insert(int c) {
	current->insert(c);
}

void Lines::del() {
	current->del();
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
		assert(topleft.line != lines.end());
	}
}

void Lines::cornerUp(std::size_t linesize) {
	if (topleft.index >= linesize) {
		topleft.index -= linesize;
	} else {
		assert(topleft.line != lines.begin());
		--topleft.line;
		topleft.index = topleft.line->size() - (topleft.line->size() % linesize);
		assert(topleft.index < topleft.line->size());
	}
}

Lines::PositionType Lines::getTopleft() const {
	return topleft;
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

void Lines::logcurrent() const {
	current->log();
}

}
