#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/util.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace TextEditor {

typedef wint_t buffer_t;

static bool shouldExit(buffer_t c) {
	return c == 'q' || std::wcin.eof();
}

int setTerminalMode() {
	if (atexit(TerminalSettings::reset) != 0) {
		syscallError("atexit");
		return ExitCode::ERROR;
	}
	return TerminalSettings::set();
}

int run() {
	if (!isatty(STDIN_FILENO)) {
		syscallError("isatty");
		return ExitCode::ERROR;
	}
	if (setTerminalMode() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return ExitCode::OK;
}

}

int main() {
	return TextEditor::run();
}
