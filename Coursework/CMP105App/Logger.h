//                                         
//     __    _____ _____ _____ _____ _____ 
//    |  |  |     |   __|   __|   __| __  |
//    |  |__|  |  |  |  |  |  |   __|    -|
//    |_____|_____|_____|_____|_____|__|__|
//                                                                                                                  
//    CMP105 Solo Project Edition
//    Noah Albrock                        

#pragma once

#include <ctime>
#include <iostream>
#include <sstream>

// Log message levels
enum LogLevel : uint8_t {
	DEBUG = 4,
	INFO = 3,
	WARN = 2,
	ERROR = 1,
	NONE = 0
};

// IMPORTANT
// The following constexpr controls the logs that you will see
// This is set at compile time
// please set it to any of the LogLevel values listed below

namespace LoggerConfig {
	inline constexpr uint8_t CURRENT_LEVEL = LogLevel::DEBUG;
}

// define global logger functions
// source: https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG_DEBUG(msg) Logger::Log(msg, LogLevel::DEBUG, __FILENAME__, __LINE__);
#define LOG_INFO(msg)  Logger::Log(msg, LogLevel::INFO,  __FILENAME__, __LINE__);
#define LOG_WARN(msg)  Logger::Log(msg, LogLevel::WARN,  __FILENAME__, __LINE__);
#define LOG_ERROR(msg) Logger::Log(msg, LogLevel::ERROR, __FILENAME__, __LINE__);

#define LOG_DEBUG_NOLINE(msg) Logger::Log(msg, LogLevel::DEBUG, __FILENAME__);
#define LOG_INFO_NOLINE(msg)  Logger::Log(msg, LogLevel::INFO,  __FILENAME__);
#define LOG_WARN_NOLINE(msg)  Logger::Log(msg, LogLevel::WARN,  __FILENAME__);
#define LOG_ERROR_NOLINE(msg) Logger::Log(msg, LogLevel::ERROR, __FILENAME__);

// A stupid simple loging class!
class Logger
{
public:
	Logger() = default;
	~Logger() {};

	// Write a log message to the console
	// @param message: The message to be sent in the log
	// @param level:   The level of the log message.
	static void Log(std::string message, uint8_t level, const char* file = "EXCLUDE", int line = -1);

	// Write a DEBUG log to the terminal
	// @param message: The message to be sent in the log
	static void Debug(std::string message) { Log(message, LogLevel::DEBUG); }
	
	// Write an INFO log to the terminal
	// @param message: The message to be sent in the log
	static void Info(std::string message) { Log(message, LogLevel::INFO); }
	
	// Write a WARNING log to the terminal
	// @param message: The message to be sent in the log
	static void Warn(std::string message) { Log(message, LogLevel::WARN); }
	
	// Write an ERROR log to the terminal
	// @param message: The message to be sent in the log
	static void Error(std::string message) { Log(message, LogLevel::ERROR); }

private:
	// Adds the formatted time the log was... well, logged
	// @param stream: The log string stream being constructed
	static void addCurrentTime(std::stringstream& stream);

	// Adds the log level of the log
	// @param stream: The log string stream being constructed
	// @param level:  The level of the log message.
	static void addLogLevel(std::stringstream& stream, uint8_t level);

	// Adds an arbitrary string into the log before the message
	static void addString(std::stringstream& stream, std::string message);

	// Adds the file and line number to the log
	// @param stream: The log string stream being constructed
	// @param file:   The name of the c++ file the log comes from. Intended to be used with `__file__`
	// @param line:	  The line the log was triggered from. Intended to be used with `__line__`
	static void addFileAndLine(std::stringstream& stream, const char* file, int line);

	// Sets the terminal text color
	// @param stream: The log string stream being constructed
	// @param level:  The level of the log message.
	static void setTerminalColour(std::stringstream& stream, uint8_t level);

	// Resets the terminal colour
	// @param stream: The log string stream being constructed
	static void resetTerminalColour(std::stringstream& stream);
};
