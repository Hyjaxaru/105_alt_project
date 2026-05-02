// Game State Class
// Simple game state controller. enum lists possible game states (can be modified), tracks and updates the current state
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once

#include <string>

// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU, LEVEL, PAUSE, CREDITS };

class GameState
{
public:
	// Set the current state
	void setCurrentState(State s);
	// Returns the current state.
	State getCurrentState();

	// Sets the current level manager level name
	void setCurrentLevel(std::string name);
	// Returns the current level manager level name
	std::string getCurrentLevel();

protected:
	State m_currentState;
	std::string m_currentLevel;
};