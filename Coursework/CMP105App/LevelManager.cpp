#include "LevelManager.h"

namespace fs = std::filesystem;

LevelManager::LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	Scene(window, input, gameState, audio)
{
}

void LevelManager::loadLevels()
{
	LOG_INFO_NOLINE("Loading levels");

	for (auto& levelName : loadLevelManifest())
	{
		auto level = loadLevel(levelName);
		if (level.has_value())
		{
			m_levelIndex.insert({ levelName, level.value() });

			// if no current level is loaded, load this
			if (m_current == nullptr)
				m_current = &m_levelIndex.at(levelName);
		}
	}

	LOG_INFO_NOLINE("Level loading complete!");
}

std::vector<std::string> LevelManager::loadLevelManifest()
{
	std::ifstream levelsFile(LEVELS_DIR + LEVEL_LIST_FILE);
	if (!levelsFile.good())
	{
		// if the index cant be loaded, then there won't be any levels
		LOG_ERROR("Cannot load level manifest file.");
		throw std::runtime_error("Cannot load level manifest file.");
	}

	// for each item in this list
	std::vector<std::string> levels;
	std::string levelName;
	while (levelsFile >> levelName)
		levels.push_back(levelName);

	return levels;
}

std::optional<LevelTemplate> LevelManager::loadLevel(std::string fileName)
{
	DataFile config(LEVELS_DIR + fileName + EXTENSION_CONFIG);
	std::vector<int> tilemap;

	// load the terrain file, adn check if it's good
	std::ifstream terrainFile(LEVELS_DIR + fileName + EXTENSION_TERRAIN);
	if (!terrainFile.good())
	{
		LOG_ERROR(fileName + " | load failed. Cannot access terrain file");
		return {};
	}

	// get the data from the terrain file
	int tile;
	while (terrainFile >> tile) tilemap.push_back(tile);

	sf::Vector2u terrainSize = {
		static_cast<unsigned int>(config.getInt("terrainX").value_or(0)),
		static_cast<unsigned int>(config.getInt("terrainY").value_or(0))
	};
	
	sf::Vector2u backgroundSize = {
		static_cast<unsigned int>(config.getInt("backgroundX").value_or(0)),
		static_cast<unsigned int>(config.getInt("backgroundY").value_or(0))
	};

	// begin creating the world
	auto level = LevelTemplate(m_window, m_input, m_gameState, m_audio, tilemap, terrainSize, backgroundSize);

	// set level metadata
	level.setLevelMetadata({fileName, config.get("author").value_or("Author Unknown")});

	// set the world size
	level.setWorldSize({
		config.getInt("worldX").value_or(0),
		config.getInt("worldY").value_or(0)
	});

	// set the view size
	level.setViewSize({
		config.getInt("viewX").value_or(0),
		config.getInt("viewY").value_or(0)
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
	return level;
}

LevelTemplate* LevelManager::getLevel(std::string name)
{
	if (m_levelIndex.find(name) == m_levelIndex.end()) return nullptr;
	return &m_levelIndex.at(name);
}

void LevelManager::setActiveLevel(std::string name)
{
	m_current->onEnd();
	m_current = getLevel(name);
	m_current->onBegin();
}