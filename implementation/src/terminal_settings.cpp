#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/mrlog.hpp"
#include "util/util.hpp"
#include <unistd.h>
#include <cstring>

namespace TextEditor {

static int confirmTcChanges(struct termios& settings) {
	struct termios current;
	memset(&current, 0, sizeof(struct termios));
	if (tcgetattr(STDIN_FILENO, &current) == -1) {
		syscallError("tcgetattr");
		return ExitCode::ERROR;
	}
	if (memcmp(&current, &settings, sizeof(struct termios)) != 0) {
		mrlog::error("tcsetattr: some terminal settings were not set");
		return ExitCode::ERROR;
	}
	return ExitCode::OK;
}

int TerminalSettings::set() {
	memset(&terminfo.orig, 0, sizeof(struct termios));
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
	return confirmTcChanges(settings);
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
