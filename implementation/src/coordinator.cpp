#include "coordinator.hpp"
#include "text_editor.hpp"
#include "util/mrlog.hpp"
#include <ctype.h>
#include <ncurses.h>
#include <unordered_map>

namespace TextEditor {

Coordinator::Coordinator()
: state(State::ACTIVE) {}

static bool shouldPrint(int ch);

int Coordinator::run() {
	if (init() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	while (state == State::ACTIVE) {
		int ch = termapi.getchar();
		// mrlog::info("%s - %d", key_name(ch), ch);
		if (shouldPrint(ch)) {
			updatechar(ch);
		} else if (dispatch(ch) == ExitCode::ERROR) {
			return ExitCode::ERROR;
		}
		// line.log();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::init() {
	return termapi.init();
}

static bool shouldPrint(int ch) {
	return isprint(ch) || ch == '\t';
}

/*
1. Put in window
2. Add to data structure
3. Update cursor

Edge case: overflows to next line (current Y is already at the end)
*/
void Coordinator::updatechar(int ch) {
	line.insert(ch);
	termapi.put(ch);
	if (!line.postEmpty()) {
		termapi.insert(line.getPost());
	}
}

/*
- Newline
- Backspace
- Delete
- Arrow keys
- Escape
- Shift + arrow keys
- Control characters

Map of <key> to <function>
This function is part of the coordinator, it can update the state (so we know when to exit or not)
*/
int Coordinator::dispatch(int ch) {
	static const std::unordered_map<int, DispatchFunction> functions = {
		{Keys::K_NEWLINE, &Coordinator::dispatchNewline},
		{Keys::K_BACKSPACE, &Coordinator::dispatchBackspace},
		{Keys::K_DELETEC, &Coordinator::dispatchDelete},
		{Keys::K_ARROW_DOWN, &Coordinator::dispatchArrowDown},
		{Keys::K_ARROW_UP, &Coordinator::dispatchArrowUp},
		{Keys::K_ARROW_LEFT, &Coordinator::dispatchArrowLeft},
		{Keys::K_ARROW_RIGHT, &Coordinator::dispatchArrowRight},
		{Keys::K_ESCAPE, &Coordinator::dispatchEscape},
		{Keys::K_CTRL_Q, &Coordinator::dispatchCtrlQ},
		{Keys::K_WINCH, &Coordinator::dispatchWindowChange},
	};

	if (functions.count(ch) > 0) {
		return (this->*(functions.at(ch)))();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchNewline() {
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchBackspace() {
	if (!line.preEmpty()) {
		line.erase();
		termapi.slideleft(line.getPost());
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchDelete() {
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowDown() {
	termapi.scrollDown();
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowUp() {
	termapi.scrollUp();
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowLeft() {
	if (!line.preEmpty()) {
		line.moveleft();
		termapi.moveleft();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowRight() {
	if (!line.postEmpty()) {
		line.moveright();
		termapi.moveright();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchEscape() {
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchCtrlQ() {
	state = State::EXIT;
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchWindowChange() {
	mrlog::info("called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

}
