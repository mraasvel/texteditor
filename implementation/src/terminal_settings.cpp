#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/mrlog.hpp"
#include "util/util.hpp"
#include <unistd.h>

namespace TextEditor {

int TerminalSettings::set() {
	if (tcgetattr(STDIN_FILENO, &terminfo.orig) == -1) {
		syscallError("tcgetattr");
		return ExitCode::ERROR;
	}
	struct termios settings(terminfo.orig);
	settings.c_lflag &= ~(ECHO);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &settings)) {
		syscallError("tcsetattr");
		return ExitCode::ERROR;
	}
	terminfo.set = true;
	return ExitCode::OK;
}

void TerminalSettings::reset() {
	if (!terminfo.set) {
		return;
	}

	mrlog::info("Resetting Terminal Settings");
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminfo.orig) == -1) {
		syscallError("tcsetattr");
	}
}

TerminalSettings::TermInfo::TermInfo()
: set(false) {}

struct TerminalSettings::TermInfo TerminalSettings::terminfo;

}
