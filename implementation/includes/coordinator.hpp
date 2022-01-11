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
	int dispatchEscape();
	int dispatchCtrlP();
	int dispatchCtrlQ();
	int dispatchCtrlS();
	int dispatchCtrlV();
	int dispatchWindowChange();
	int dispatchHome();
	int dispatchEnd();

private:
	State state;
	TermApi termapi;
	Lines lines;
	bool winch;
};

}
