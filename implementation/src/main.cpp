#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <array>
#include <ncurses.h>

// Returns the value of a given control character
#define CTRL_KEY(key) ((key) & 0x1F)

namespace TextEditor {

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

static std::array<char, 6> readByte() {
	ssize_t n;
	std::array<char, 6> buffer;
	memset(buffer.data(), 0, buffer.size());
	if (read(STDIN_FILENO, buffer.data(), 6) == -1 && errno != EAGAIN) {
		syscallError("readByte: read");
		throw std::runtime_error("read failed");
	}
	return buffer;
}

static int processByte() {
	std::array<char, 6> buffer = readByte();
	switch(buffer.front()) {
		case CTRL_KEY('q'):
			return ExitCode::ERROR;
		default:
			printBytes(buffer.data(), buffer.size());
			break;
	}
	return ExitCode::SUCCESS;
}

static int run() {
	if (!isatty(STDIN_FILENO)) {
		syscallError("isatty");
		return ExitCode::ERROR;
	}
	if (setTerminalMode() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	while (processByte() == ExitCode::SUCCESS) {}
	return ExitCode::SUCCESS;
}

}

int main() {
	return TextEditor::run();
}
