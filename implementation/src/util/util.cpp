#include "util/util.hpp"
#include <cstdio>
#include <string>

namespace TextEditor {

void syscallError(const std::string& s) {
	perror(s.c_str());
}

}
