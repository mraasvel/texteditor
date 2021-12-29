#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/mrlog.hpp"
#include "util/util.hpp"
#include <unistd.h>
#include <cstring>

namespace TextEditor {

/*
See documentation: termios.txt for details
*/
struct termios TerminalSettings::getRawModeSettings() {
	struct termios settings(terminfo.orig);
	settings.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	settings.c_oflag &= ~(OPOST);
	settings.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	settings.c_cflag &= ~(CSIZE | PARENB);
	settings.c_cflag |= CS8;
	settings.c_cc[VMIN] = 1;
	settings.c_cc[VTIME] = 0;
	return settings;
}

// TODO: More specific error: list the settings not being turned on for example
static int validateTerminalChanges(struct termios& settings) {
	struct termios current;
	memset(&current, 0, sizeof(struct termios));
	if (tcgetattr(STDIN_FILENO, &current) == -1) {
		return syscallError("tcgetattr");
	}
	if (memcmp(&current, &settings, sizeof(struct termios)) != 0) {
		mrlog::error("tcsetattr: some terminal settings were not set");
		return ExitCode::ERROR;
	}
	mrlog::debug("termios settings modified");
	return ExitCode::SUCCESS;
}

int TerminalSettings::setRawMode() {
	if (!isOriginalSet()) {
		mrlog::warn("set original terminal first");
		return ExitCode::ERROR;
	}
	struct termios settings = getRawModeSettings();
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &settings)) {
		return syscallError("tcsetattr");
	}
	terminfo.set = true;
	return validateTerminalChanges(settings);
}

bool TerminalSettings::isOriginalSet() {
	return terminfo.set;
}

int TerminalSettings::saveCurrent() {
	memset(&terminfo.orig, 0, sizeof(struct termios));
	if (tcgetattr(STDIN_FILENO, &terminfo.orig) == -1) {
		return syscallError("tcgetattr");
	}
	terminfo.set = true;
	mrlog::debug("termios state saved");
	return ExitCode::SUCCESS;
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
