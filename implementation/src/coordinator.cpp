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
		// mrlog::info("{} - {}", key_name(ch), ch);
		if (shouldPrint(ch)) {
			updatechar(ch);
		} else if (dispatch(ch) == ExitCode::ERROR) {
			return ExitCode::ERROR;
		}
		termapi.render(lines);
		lines.logcurrent();
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
	lines.push(ch);
	termapi.moveright();
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
	static const std::unordered_map<Keys, DispatchFunction > functions = {
		{Keys::K_NEWLINE, &Coordinator::dispatchNewline},
		{Keys::K_BACKSPACE, &Coordinator::dispatchBackspace},
		{Keys::K_DELETEC, &Coordinator::dispatchDelete},
		{Keys::K_ARROW_DOWN, &Coordinator::dispatchArrowDown},
		{Keys::K_ARROW_UP, &Coordinator::dispatchArrowUp},
		{Keys::K_ARROW_LEFT, &Coordinator::dispatchArrowLeft},
		{Keys::K_ARROW_RIGHT, &Coordinator::dispatchArrowRight},
		{Keys::K_SARROW_DOWN, &Coordinator::dispatchShiftArrowDown},
		{Keys::K_SARROW_UP, &Coordinator::dispatchShiftArrowUp},
		{Keys::K_SARROW_LEFT, &Coordinator::dispatchShiftArrowLeft},
		{Keys::K_SARROW_RIGHT, &Coordinator::dispatchShiftArrowRight},
		{Keys::K_ESCAPE, &Coordinator::dispatchEscape},
		{Keys::K_CTRL_Q, &Coordinator::dispatchCtrlQ},
		{Keys::K_CTRL_V, &Coordinator::dispatchCtrlV},
		{Keys::K_WINCH, &Coordinator::dispatchWindowChange},
	};

	if (functions.count(static_cast<Keys>(ch)) > 0) {
		return (this->*(functions.at(static_cast<Keys>(ch))))();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchNewline() {
	lines.insertNewline();
	termapi.put('\n');
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchBackspace() {
	if (!lines.preEmpty()) {
		lines.erase();
		termapi.moveleft();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchDelete() {
	if (!lines.postEmpty()) {
		lines.del();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowDown() {
	int maxx = termapi.getLineSize();
	termapi.moveright(lines.moveright(maxx));
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowUp() {
	int maxx = termapi.getLineSize();
	termapi.moveleft(lines.moveleft(maxx));
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowLeft() {
	if (!lines.preEmpty()) {
		lines.moveleft();
		termapi.moveleft();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowDown() {
	mrlog::info("Call: {}", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowUp() {
	mrlog::info("Call: {}", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowLeft() {
	mrlog::info("Call: {}", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowRight() {
	mrlog::info("Call: {}", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowRight() {
	if (!lines.postEmpty()) {
		lines.moveright();
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

int Coordinator::dispatchCtrlV() {
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchWindowChange() {
	return ExitCode::SUCCESS;
}

}
