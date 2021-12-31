#include "text_editor.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "termapi.hpp"
#include "coordinator.hpp"
#include <unistd.h>

namespace TextEditor {

static int run() {
	mrlog::clear();
	if (!isatty(STDIN_FILENO)) {
		return syscallError("isatty");
	}
	try {
		Coordinator coordinator;
		return coordinator.run();
	} catch (const std::exception& e) {
		mrlog::fatal("caught exception: %s", e.what());
		return ExitCode::ERROR;
	}
}

}

int main() {
	return TextEditor::run();
}
