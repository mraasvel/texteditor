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
	int run();

private:
	int init();
	void updatechar(int ch);
	int dispatch(int ch);

/* Dispatchers */
	typedef int (Coordinator::*DispatchFunction)();

	int dispatchNewline();
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
	int dispatchCtrlQ();
	int dispatchCtrlV();
	int dispatchWindowChange();

private:
	State state;
	TermApi termapi;
	Line line;
	Lines lines;
};

}
