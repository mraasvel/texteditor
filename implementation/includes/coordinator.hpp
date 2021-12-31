#pragma once

#include "termapi.hpp"

namespace TextEditor {

class Coordinator {
private:
	enum class State {
		ACTIVE,
		ERROR,
		EXIT
	};

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
	int dispatchEscape();
	int dispatchCtrlQ();

private:
	State state;
	TermApi termapi;
};

}
