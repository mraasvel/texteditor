#include "text_editor.hpp"
#include "terminal_settings.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "termapi.hpp"
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
		return syscallError("atexit");
	}
	if (TerminalSettings::saveCurrent() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return TerminalSettings::setRawMode();
}

static void logBytes(int n) {
	mrlog::info("Bytes: %d", n);
	while (n != 0) {
		mrlog::info("%d", n % 256);
		n = n >> 8;
	}
}

static int run() {
	mrlog::clear();
	if (!isatty(STDIN_FILENO)) {
		return syscallError("isatty");
	}
	TermApi api;
	if (api.init() == ExitCode::ERROR) {
		return ExitCode::ERROR;
	}
	return ExitCode::SUCCESS;
}

}

struct Box {
	Box(chtype corner = '+', chtype hchar = '-', chtype vchar = '|')
	: corner(corner), hchar(hchar), vchar(vchar) {}
	chtype corner;
	chtype hchar;
	chtype vchar;
};

struct SubWindow {
	SubWindow(int h, int w, int y, int x)
	: height(h), width(w), y(y), x(x) {}
	int height, width;
	int y, x;
};

static void drawBox(const SubWindow& win, const Box& box) {
	attron(COLOR_PAIR(1));
	// Corners
	mvaddch(win.y, win.x, box.corner);
	mvaddch(win.y + win.height, win.x + win.width, box.corner);
	mvaddch(win.y + win.height, win.x, box.corner);
	mvaddch(win.y, win.x + win.width, box.corner);
	// Lines
	mvhline(win.y, win.x + 1, box.hchar, win.width - 1);
	mvhline(win.y + win.height, win.x + 1, box.hchar, win.width - 1);
	mvvline(win.y + 1, win.x, box.vchar, win.height - 1);
	mvvline(win.y + 1, win.x + win.width, box.vchar, win.height - 1);
	attroff(COLOR_PAIR(1));
}

static void testNcurses() {
	TextEditor::TermApi termapi;
	termapi.init();

	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	SubWindow win(10, 10, 0, 0);
	int ch;
	drawBox(win, Box('+', '-', '|'));
	while ((ch = getch()) != KEY_F(1)) {
		drawBox(win, Box(' ', ' ', ' '));
		if (ch == KEY_LEFT && win.x > 0) {
			win.x--;
		} else if (ch == KEY_RIGHT && win.x < COLS - win.width - 1) {
			win.x++;
		} else if (ch == KEY_UP && win.y > 0) {
			win.y--;
		} else if (ch == KEY_DOWN && win.y < LINES - win.height - 1) {
			win.y++;
		}
		drawBox(win, Box('+', '-', '|'));
		refresh();
	}
	scr_dump("filename.txt");
}

int main() {
	mrlog::clear();
	initscr();
	refresh();
	testNcurses();
	endwin();
	return 0;
	return TextEditor::run();
}
