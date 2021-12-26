#pragma once

/*
https://www.ibm.com/docs/en/cognos-analytics/10.2.2?topic=SSEP7J_10.2.2/com.ibm.swg.ba.cognos.ug_rtm_wb.10.2.2.doc/c_n30e74.html
Log levels:
	[fatal]
	[error]
	[warn]
	[info]
	[debug]
	[debug-low]
	[debug-medium]
	[debug-high]
*/
namespace mrlog {

enum class Level {
	OFF,
	FATAL,
	ERROR,
	WARN,
	INFO,
	DEBUG
};

void log(const char* fmt, ...);
void fatal(const char* fmt, ...);
void error(const char* fmt, ...);
void warn(const char* fmt, ...);
void info(const char* fmt, ...);
void debug(const char* fmt, ...);

}
