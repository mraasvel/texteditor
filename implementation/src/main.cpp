#include "text_editor.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "termapi.hpp"
#include "coordinator.hpp"
#include <unistd.h>

namespace TextEditor {

static int run() {
	mrlog::setLogFile("mrlog.log");
	mrlog::clearLog();
	if (!isatty(STDIN_FILENO)) {
		return syscallError("isatty");
	}
	Coordinator coordinator;
	try {
		return coordinator.run();
	} catch (const std::exception& e) {
		mrlog::fatal("caught exception: {}\n", e.what());
		coordinator.log();
		return ExitCode::ERROR;
	}
}

}

#ifndef CATCH_CONFIG_MAIN
int main() {
	return TextEditor::run();
}
#endif /* CATCH_CONFIG_MAIN */
