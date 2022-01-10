#include "text_editor.hpp"
#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "termapi.hpp"
#include "coordinator.hpp"
#include <unistd.h>

namespace TextEditor {

static int run(int argc, char *argv[]) {
	mrlog::clearLog();
	if (!isatty(STDIN_FILENO)) {
		return syscallError("isatty");
	}
	std::ifstream ifs(argv[0]);
	if (!ifs.is_open()) {
		return syscallError("ifs::open");
	}
	Coordinator coordinator(ifs);
	ifs.close();
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
int main(int argc, char *argv[]) {
	argc--; argv++;
	if (argc == 0) {
		mrlog::error("no input file given as argument\n");
		return 1;
	}
	return TextEditor::run(argc, argv);
}
#endif /* CATCH_CONFIG_MAIN */
