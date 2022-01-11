#include "text_editor.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "termapi.hpp"
#include "coordinator.hpp"
#include <unistd.h>

namespace TextEditor {

static int run(Coordinator& coordinator) {
	try {
		return coordinator.run();
	} catch (const std::exception& e) {
		mrlog::fatal("caught exception: {}\n", e.what());
		coordinator.log();
		return ExitCode::ERROR;
	}
}

static int inputFile(const std::string& filename) {
	Lines lines;
	if (lines.fromFile(filename) != ExitCode::SUCCESS) {
		mrlog::fatal("Could not parse file: {}\n", filename);
		return ExitCode::ERROR;
	}
	Coordinator coordinator(std::move(lines));
	return run(coordinator);
}

int main(int argc, char *argv[]) {
	argc--; argv++;
	mrlog::clearLog();
	if (!isatty(STDIN_FILENO)) {
		return syscallError("isatty");
	}
	if (argc > 0) {
		return TextEditor::inputFile(argv[0]);
	}
	Coordinator coordinator;
	return TextEditor::run(coordinator);
}

}

#ifndef CATCH_CONFIG_MAIN
int main(int argc, char *argv[]) {
	return TextEditor::main(argc, argv);
}
#endif /* CATCH_CONFIG_MAIN */
