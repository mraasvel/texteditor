#pragma once

#include "termapi.hpp"
#include "datastructures/lines.hpp"

namespace TextEditor {

class Coordinator {
private:
	enum class State {
		ACTIVE,
		ERROR,
		EXIT
	};

	typedef TermApi::Keys Keys;

public:
	Coordinator();
	Coordinator(Lines&& lines);
	int run();

	void log() const;

private:
	int init();
	void updatechar(int ch);
	int dispatch(int ch);
	bool windowChanged(int ch) const;
	void recalibrateWindow();

/* Dispatchers */
	typedef int (Coordinator::*DispatchFunction)();
	typedef bool (*IsFunction)(int);
	typedef int (Lines::*CurrentCharFunction)() const;

	int dispatchNewline();
	int dispatchTab();
	int dispatchBackspace();
	int dispatchDelete();
	int dispatchArrowDown();
	int dispatchArrowUp();
	int dispatchArrowLeft();
	int dispatchArrowRight();
	int dispatchShiftArrowDown();
	int dispatchShiftArrowUp();
	int dispatchShiftArrowLeft();
	int dispatchShiftArrowRight();
	int dispatchCtrlArrowDown();
	int dispatchCtrlArrowUp();
	int dispatchCtrlArrowLeft();
	int dispatchCtrlArrowRight();
	int dispatchEscape();
	int dispatchCtrlP();
	int dispatchCtrlQ();
	int dispatchCtrlS();
	int dispatchCtrlV();
	int dispatchWindowChange();
	int dispatchHome();
	int dispatchEnd();

	void skipchars(CurrentCharFunction currentchar, IsFunction istype, DispatchFunction);

private:
	State state;
	TermApi termapi;
	Lines lines;
	bool winch;
};

}
