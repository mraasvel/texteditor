#include "coordinator.hpp"
#include "text_editor.hpp"
#include "util/mrlog.hpp"
#include "util/keymaps.hpp"
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
	termapi.put("Press Ctrl + Q to exit\n");
	while (state == State::ACTIVE) {
		int ch = termapi.getchar();
		mrlog::info("%s - %d", key_name(ch), ch);
		if (shouldPrint(ch)) {
			updatechar(ch);
		} else if (dispatch(ch) == ExitCode::ERROR) {
			return ExitCode::ERROR;
		}
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
	termapi.put(ch);
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
		{KEY_BACKSPACE, &Coordinator::dispatchBackspace},
		{KEY_DC, &Coordinator::dispatchDelete},
		{KEY_DOWN, &Coordinator::dispatchArrowDown},
		{KEY_UP, &Coordinator::dispatchArrowUp},
		{KEY_LEFT, &Coordinator::dispatchArrowLeft},
		{KEY_RIGHT, &Coordinator::dispatchArrowRight},
		{Keys::ESCAPE, &Coordinator::dispatchEscape},
		{CTRL_KEY('q'), &Coordinator::dispatchCtrlQ},
		{Keys::NEWLINE, &Coordinator::dispatchNewline},
	};

	if (functions.count(ch) > 0) {
		return (this->*(functions.at(ch)))();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchNewline() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchBackspace() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchDelete() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowDown() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowUp() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowLeft() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowRight() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchEscape() {
	mrlog::info("Called: %s", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchCtrlQ() {
	mrlog::info("Called: %s", __FUNCTION__);
	state = State::EXIT;
	return ExitCode::SUCCESS;
}

}
