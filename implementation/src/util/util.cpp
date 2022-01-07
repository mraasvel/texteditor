#include "util/util.hpp"
#include "util/mrlog.hpp"
#include "util/exit_codes.hpp"
#include <cstring>
#include <string>
#include <cerrno>

namespace TextEditor {

int syscallError(const std::string& s) {
	mrlog::error("{}: {}", s, strerror(errno));
	return ExitCode::ERROR;
}

}
