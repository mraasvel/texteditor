#include "coordinator.hpp"
#include "text_editor.hpp"
#include "util/mrlog.hpp"
#include "util/util.hpp"
#include <ctype.h>
#include <ncurses.h>
#include <unordered_map>

namespace TextEditor {

Coordinator::Coordinator()
: state(State::ACTIVE), winch(false) {}

static bool shouldPrint(int ch);

int Coordinator::run() {
	if (init() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	while (state == State::ACTIVE) {
		int ch = termapi.getchar();
		logKey(ch);
		if (windowChanged(ch)) {
			mrlog::info("detected window change\n");
			winch = false;
			recalibrateWindow();
		}
		if (shouldPrint(ch)) {
			updatechar(ch);
		} else if (dispatch(ch) == ExitCode::ERROR) {
			return ExitCode::ERROR;
		}
		termapi.render(lines);
	}
	return ExitCode::SUCCESS;
}

void Coordinator::log() const {
	lines.log();
}

int Coordinator::init() {
	return termapi.init();
}

static bool shouldPrint(int ch) {
	return isprint(ch) || ch == '\t';
}

bool Coordinator::windowChanged(int ch) const {
	return winch && static_cast<Keys>(ch) != Keys::K_WINCH;
}

void Coordinator::recalibrateWindow() {
	lines.recalibrateWindow(termapi.getLineSize(), termapi.getRows());
}

void Coordinator::updatechar(int ch) {
	lines.push(ch);
	if (termapi.isEndOfScreen()) {
		lines.cornerDown(termapi.getLineSize());
	}
}

/*
Map of <key> to <function>
This function is part of the coordinator, it can update the state (so we know when to exit or not)
*/
int Coordinator::dispatch(int ch) {
	static const std::unordered_map<int, DispatchFunction > functions = {
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
		{Keys::K_CTRL_P, &Coordinator::dispatchCtrlP},
		{Keys::K_CTRL_Q, &Coordinator::dispatchCtrlQ},
		{Keys::K_CTRL_V, &Coordinator::dispatchCtrlV},
		{Keys::K_WINCH, &Coordinator::dispatchWindowChange},
		{Keys::K_HOME, &Coordinator::dispatchHome},
		{Keys::K_END, &Coordinator::dispatchEnd},
	};

	if (functions.count(ch) > 0) {
		return (this->*(functions.at(ch)))();
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchNewline() {
	lines.insertNewline();
	if (termapi.isEndOfLines()) {
		lines.cornerDown(termapi.getLineSize());
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchBackspace() {
	if (lines.erase() && termapi.isStartOfScreen()) {
		lines.cornerUp(termapi.getLineSize());
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchDelete() {
	lines.del();
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowDown() {
	const std::size_t linesize = termapi.getLineSize();
	if (lines.moveDown(linesize) && termapi.isEndOfLines()) {
		lines.cornerDown(linesize);
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowUp() {
	const std::size_t linesize = termapi.getLineSize();
	if (lines.moveUp(linesize) && termapi.isStartOfLines()) {
		lines.cornerUp(linesize);
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowLeft() {
	if (lines.moveleft() && termapi.isStartOfScreen()) {
		lines.cornerUp(termapi.getLineSize());
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchArrowRight() {
	if (lines.moveright() && termapi.isEndOfScreen()) {
		lines.cornerDown(termapi.getLineSize());
	}
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowDown() {
	mrlog::info("Call: {}\n", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowUp() {
	mrlog::info("Call: {}\n", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowLeft() {
	mrlog::info("Call: {}\n", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchShiftArrowRight() {
	mrlog::info("Call: {}\n", __FUNCTION__);
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchEscape() {
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchCtrlP() {
	lines.log();
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
	winch = true;
	return ExitCode::SUCCESS;
}

int Coordinator::dispatchHome() {
	auto chars = lines.moveStart();
	auto nlines = termapi.calculateUnderflowedLines(chars);
	auto linesize = termapi.getLineSize();
	while (nlines-- > 0) {
		lines.cornerUp(linesize);
	}
	return ExitCode::SUCCESS;
}
int Coordinator::dispatchEnd() {
	auto chars = lines.moveEnd();
	auto nlines = termapi.calculateOverflowedLines(chars);
	auto linesize = termapi.getLineSize();
	while (nlines-- > 0) {
		lines.cornerDown(linesize);
	}
	return ExitCode::SUCCESS;
}

}
