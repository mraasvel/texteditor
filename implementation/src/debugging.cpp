#include "text_editor.hpp"
#include "util/mrlog.hpp"
#include <ncurses.h>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio> //REMOVE
#include <cerrno> //REMOVE
#include <cstring>

namespace TextEditor {

constexpr char PREFIX_FILENAME[] = "keylog";
constexpr char DIRNAME[] = "./keylogs/";

static bool isDirectory(const std::string& dirname) {
	struct stat s;
	if (stat(dirname.c_str(), &s) == 0) {
		return S_ISDIR(s.st_mode);
	}
	return false;
}

static bool fileExists(const std::string& filename) {
	return access(filename.c_str(), F_OK) == 0;
}

static void createDirectory(const std::string& dirname) {
	if (fileExists(dirname)) {
		return;
	}
	mkdir(dirname.c_str(), 0777);
}

static std::size_t countFiles(const std::string& dirname) {
	DIR* dir = opendir(dirname.c_str());
	if (dir == nullptr) {
		return 0;
	}
	std::size_t count {0};
	while (struct dirent* ent = readdir(dir)) {
		const std::string filename = dirname + ent->d_name;
		if (isDirectory(filename)) {
			continue;
		}
		++count;
	}
	closedir(dir);
	return count;
}

static std::string generateFilename() {
	createDirectory(DIRNAME);
	std::size_t filecount = countFiles(DIRNAME);
	return DIRNAME + std::string(PREFIX_FILENAME) + std::to_string(filecount) + ".log";
}

void logKey(int ch) {
	static const std::string file = generateFilename();
	if (!isDirectory(DIRNAME)) {
		mrlog::error("logkey: default directory is not a directory: {}\n", DIRNAME);
		return;
	}
	std::ofstream ofs(file, std::ios_base::app);
	if (!ofs.is_open()) {
		mrlog::error("logkey: failed to open keylog file: {}\n", file);
		return;
	}
	if (key_name(ch) == nullptr) {
		ofs << ch << " null\n";
	} else {
		ofs << ch << " " << key_name(ch) << "\n";
	}
	ofs.close();
}

}
