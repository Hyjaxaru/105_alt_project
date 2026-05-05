#include "Logger.h"

// disables warning about ctime being "unsafe"
#pragma warning(disable : 4996)

// --- public --- //

void Logger::Log(std::string message, uint8_t level, const char* file, int line)
{
	if (level > LoggerConfig::CURRENT_LEVEL)
		return;
	
	// create the output stream
	std::stringstream ostream;

	// prepare the log stream
	setTerminalColour(ostream, level);
	addCurrentTime(ostream);
	addLogLevel(ostream, level);
	addFileAndLine(ostream, file, line);
	ostream << message << std::endl;
	resetTerminalColour(ostream);

	// output
	std::cout << ostream.str();
}

// --- private --- //

void Logger::addCurrentTime(std::stringstream& stream)
{
	// get the current time
	time_t timestamp = time(NULL);
	std::string time = ctime(&timestamp);
	time.pop_back(); // ctime adds a newline to the end of the string, so we remove it
	stream << time << " | ";
}

void Logger::addLogLevel(std::stringstream& stream, uint8_t level)
{
	std::string levelString;
	switch (level)
	{
	case LogLevel::DEBUG: levelString = "DEBUG"; break;
	case LogLevel::INFO:  levelString = "INFO "; break;
	case LogLevel::WARN:  levelString = "WARN "; break;
	case LogLevel::ERROR: levelString = "ERROR"; break;
	}
	stream << levelString << " | ";
}

void Logger::addString(std::stringstream& stream, std::string string)
{
	stream << string << " | ";
}

void Logger::addFileAndLine(std::stringstream& stream, const char* file, int line)
{
	if (line > 0)
		stream << file << ':' << line << " | ";
	else
		stream << file << " | ";
}

void Logger::setTerminalColour(std::stringstream& stream, uint8_t level)
{
	int colour = 0;
	switch (level)
	{
	case LogLevel::DEBUG: colour = 90; break;
	case LogLevel::INFO:  colour = 0 ; break;
	case LogLevel::WARN:  colour = 33; break;
	case LogLevel::ERROR: colour = 31; break;
	}
	stream << "\033[" << colour << 'm';
}

void Logger::resetTerminalColour(std::stringstream& stream)
{
	stream << "\033[0m";
}