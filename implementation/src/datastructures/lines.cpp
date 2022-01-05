#include "datastructures/lines.hpp"
#include <cassert>

namespace TextEditor {

/*
Invariant: there is always at least one empty line */

Lines::Lines()
: lines(1), current(lines.begin()) {
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

void Lines::logcurrent() const {
	current->log();
}

}