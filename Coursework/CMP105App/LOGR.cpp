#include "LOGR.h"

// disables warning about ctime being "unsafe"
#pragma warning(disable : 4996)

LOGR::LOGR(std::string classname, std::ostream& ostream)
	: m_classname(classname), m_ostream(ostream)
{
}

// --- public --- //

void LOGR::log(std::string message, LogLevel level)
{
	// prepare to output the log message
	changeTerminalColour(level);
	outputLogLevel(level);
	outputCurrentTime();
	outputClassname();

	// output the log message
	m_ostream << message << std::endl;

	resetTerminalColour();
}

// --- private --- //

void LOGR::outputCurrentTime()
{
	// get the current time
	time_t timestamp = time(NULL);
	std::string time = ctime(&timestamp);
	time.pop_back(); // ctime adds a newline to the end of the string, so we remove it
	m_ostream << '[' << time << "] ";
}

void LOGR::outputLogLevel(LogLevel& level)
{
	std::string levelString;
	switch (level)
	{
	case LogLevel::INFO:  levelString = "INFO"; break;
	case LogLevel::WARN:  levelString = "WARN"; break;
	case LogLevel::ERROR: levelString = "ERRO"; break;
	}
	m_ostream << '[' << levelString << "] ";
}

void LOGR::outputClassname()
{
	m_ostream << '[' << m_classname << "] ";
}

void LOGR::changeTerminalColour(LogLevel& level)
{
	int colour = 0;
	switch (level)
	{
	case LogLevel::INFO:  colour = 0; break;
	case LogLevel::WARN:  colour = 33; break;
	case LogLevel::ERROR: colour = 31; break;
	}
	m_ostream << "\033[" << colour << 'm';
}

void LOGR::resetTerminalColour()
{
	m_ostream << "\033[0m";
}