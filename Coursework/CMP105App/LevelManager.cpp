#include "LevelManager.h"

LevelManager::LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	m_window(window), m_input(input), m_gameState(gameState), m_audio(audio)
{
}

void LevelManager::loadLevels(std::string dirPath)
{

}

void LevelManager::loadLevel(std::string filePath)
{
	LOG_INFO_NOLINE("Loading level '" + filePath + '\'')

	// get the level and ensure we can access it
	std::ifstream file(filePath);

	if (!file.good()) {
		LOG_ERROR_NOLINE(makeFileLoadFailErrorMessage(filePath, "File not found."));
		return;
	}

	// create the level creation struct that contains all the information
	// required for the creation process
	auto level = Level{
		file,	  // the file
		filePath, // the path to the level file
		filePath, // the name of the level (using path for now, overriden later)
		LevelTemplate(m_window, m_input, m_gameState, m_audio)
	};

	// decode all of the keywords 
	std::string keyword;
	bool validLevel = false, validVersion = false, validTerrain = false;

	while (file >> keyword)
	{
		// I tried to make switch statements work here
		// unfortunately there isn't enough time to fight with it

		// detect the level header
		// if found, fill in the level name
		// this should must occur only once. no LEVEL = invalid, more than one = ignored
		if (keyword == "LEVEL" && !validLevel)
		{
			file >> level.name;
			validLevel = true;
		}
	}
}