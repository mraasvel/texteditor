#include "util/mrlog.hpp"
#include <string>
#include <cstdarg>
#include <cstdio>
#include <ctime>

namespace mrlog {

	namespace __detail__ {

	static const std::string LOGFILE = "./mrlog.log";

	static std::string generateDate() {
		char buffer[100];
		time_t rawtime;
		time(&rawtime);
		struct tm* timeinfo = gmtime(&rawtime);
		strftime(buffer, sizeof(buffer), "%FT%T%z", timeinfo);
		return std::string(buffer);
	}

	static std::string getLevelString(Level level) {
		switch(level) {
			case Level::FATAL:
				return "fatal";
				break;
			case Level::ERROR:
				return "error";
				break;
			case Level::WARN:
				return "warn";
				break;
			case Level::INFO:
				return "info";
				break;
			case Level::DEBUG:
				return "debug";
				break;
			default:
				return "";
		}
	}

	static void print(FILE* outfile, const char* fmt, va_list arg, Level level) {
		fprintf(outfile, "%s: ", generateDate().c_str());
		if (level != Level::OFF) {
			fprintf(outfile, "[%s] ", getLevelString(level).c_str());
		}
		vfprintf(outfile, fmt, arg);
		fprintf(outfile, "\r\n");
	}

	static void log(const char* fmt, va_list arg, Level level = Level::OFF) {
		// print(stdout, fmt, arg, level);
		// va_end(arg);
		// return;
		FILE* logfile = fopen(LOGFILE.c_str(), "a");
		if (logfile == nullptr) {
			perror("fopen");
			return;
		}
		print(logfile, fmt, arg, level);
		if (fclose(logfile) == -1) {
			perror("fclose");
		}
		va_end(arg);
	}

	}

void log(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	__detail__::log(fmt, ap);
}

void fatal(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	__detail__::log(fmt, ap, Level::FATAL);
}

void error(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	__detail__::log(fmt, ap, Level::ERROR);
}

void warn(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	__detail__::log(fmt, ap, Level::WARN);
}

void info(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	__detail__::log(fmt, ap, Level::INFO);
}

void debug(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	__detail__::log(fmt, ap, Level::DEBUG);
}

}
