#include "LevelManager.h"

namespace fs = std::filesystem;

LevelManager::LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	m_window(window), m_input(input), m_gameState(gameState), m_audio(audio)
{
}

void LevelManager::loadLevels()
{
	// get the list of levels to load from the manifest file
	std::ifstream levelsFile(LEVELS_DIR + LEVEL_LIST_FILE);
	if (!levelsFile.good())
	{
		// if the index cant be loaded, then there won't be any levels
		LOG_ERROR("Cannot load level index file.");
		throw std::runtime_error("Cannot load level index file.");
	}

	// for each item in this list
	std::string levelName;
	while (levelsFile >> levelName)
		loadLevel(levelName);

	LOG_INFO_NOLINE("Level loading complete!")
}

void LevelManager::loadLevel(std::string fileName)
{
	DataFile config(LEVELS_DIR + fileName + EXTENSION_CONFIG);
	std::vector<int> tilemap;

	// load the terrain file, adn check if it's good
	std::ifstream terrainFile(LEVELS_DIR + fileName + EXTENSION_TERRAIN);
	if (!terrainFile.good())
	{
		LOG_ERROR(fileName + " | load failed. Cannot access terrain file");
		return;
	}

	// get the data from the terrain file
	int tile;
	while (terrainFile >> tile) tilemap.push_back(tile);

	// create the level in the index and grab a reference to it
	m_levelIndex.insert({ fileName, LevelTemplate(m_window, m_input, m_gameState, m_audio) });
	auto& level = m_levelIndex.at(fileName);

	// set the world size
	level.setWorldSize({
		config.getInt("worldX").value_or(0),
		config.getInt("worldY").value_or(0)
	});

	// set the player spawn location
	level.setPlayerSpawn({
		config.getFloat("playerX").value_or(0.f),
		config.getFloat("playerY").value_or(0.f)
	});

	// set the position of the goal
	level.setGoalLocation({
		config.getFloat("goalX").value_or(0.f),
		config.getFloat("goalY").value_or(0.f)
	});

	LOG_INFO_NOLINE(fileName + " | loaded successfully!");
}