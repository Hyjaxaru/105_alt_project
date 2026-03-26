//                             
//     __    _____ _____ _____ 
//    |  |  |     |   __| __  | 
//    |  |__|  |  |  |  |    -|  
//    |_____|_____|_____|__|__|
//                             

#pragma once

#include <ctime>
#include <iostream>

// Log message levels
enum LogLevel {
	INFO,
	WARN,
	ERROR
};

// A stupid simple loging class!
class LOGR
{
public:
	// Initialises a standard LOGR instance
	// @param classname: The classname where this logger will be called from. Decorative.
	// @param ostream:   The output stream the logger should output to. if unsure, use std::cout.
	LOGR(std::string classname, std::ostream& ostream = std::cout);
	~LOGR() {};

	// Make a log!
	// @param message: The message to be sent in the log
	// @param level:   The level of the log message. currently just for terminal highlighting.
	void log(std::string message, LogLevel level = INFO);

private:
	// The output stream where the logger should dump to
	std::ostream& m_ostream;
	// The name of the class this logger is responsible for
	std::string m_classname;

	// Outputs the formatted time the log was logged
	void outputCurrentTime();
	// Outputs the log level of the log message
	// @param level: The level of the log message.
	void outputLogLevel(LogLevel& level);
	// Outputs the formatted classname where the logger was spawned
	void outputClassname();
	// Sets the terminal text color, if we should
	// @param level: The level of the log message.
	void changeTerminalColour(LogLevel& level);
	// Resets the terminal colour
	void resetTerminalColour();
};
