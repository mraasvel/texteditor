#pragma once

#include <string>

namespace TextEditor {

int syscallError(const std::string& s);
void logKey(int ch);
bool isNormal(int ch);
bool isDelimiter(int ch);

}
