#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>

namespace TextEditor {

typedef wint_t buffer_t;

static int setTerminalMode() {
	if (atexit(TerminalSettings::reset) != 0) {
		syscallError("atexit");
		return ExitCode::ERROR;
	}
	if (TerminalSettings::saveCurrent() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return TerminalSettings::setRawMode();
}

static void printBytes(const char* buffer, size_t n) {
	while (n-- > 0) {
		std::cout << (int)(*buffer++);
		if (n > 0) {
			std::cout << ' ';
		}
	}
	std::cout << "\r\n";
}

static void readStdin() {
	mrlog::info("Start Reading");
	char buffer[6] = {0};
	ssize_t n;
	do {
		memset(&buffer, 0, sizeof(buffer));
		n = read(STDIN_FILENO, buffer, sizeof(buffer));
		printBytes(buffer, 6);
	} while (buffer[0] != 'q');
}

static int run() {
	if (!isatty(STDIN_FILENO)) {
		syscallError("isatty");
		return ExitCode::ERROR;
	}
	if (setTerminalMode() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	readStdin();
	return ExitCode::OK;
}

}

int main() {
	return TextEditor::run();
}
