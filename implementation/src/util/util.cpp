#include "util/util.hpp"
#include "util/mrlog.hpp"
#include <cstring>
#include <string>
#include <cerrno>

namespace TextEditor {

void syscallError(const std::string& s) {
	mrlog::error("%s: %s", s.c_str(), strerror(errno));
}

}
