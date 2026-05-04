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

			// stop crashing on launch
			if (m_current == nullptr)
				forceSetActiveLevel(levelName);
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

	sf::Vector2u terrainSize = {
		static_cast<unsigned int>(config.getInt("terrainX").value_or(0)),
		static_cast<unsigned int>(config.getInt("terrainY").value_or(0))
	};

	// begin creating the world
	auto tilemap = loadTerrain(LEVELS_DIR + fileName + EXTENSION_TERRAIN);
	auto level = LevelTemplate(m_window, m_input, m_gameState, m_audio, tilemap, terrainSize);

	// configure the level
	configureLevel(level, config);

	// load eneimes (if we can)
	loadEnemies(level, LEVELS_DIR + fileName + EXTENSION_ENEMIES);

	LOG_INFO_NOLINE(fileName + " | loaded successfully!");
	return level;
}

void LevelManager::configureLevel(LevelTemplate& level, DataFile& config)
{
	// set level metadata
	auto& metadata = level.getLevelMetadata();
	metadata.name = config.get("name").value_or("Level Name Unknown");
	metadata.author = config.get("author").value_or("Author Unknown");

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

	// set weapon ammp
	level.setWeaponAmmo(config.getInt("bullets").value_or(0));
}

std::vector<int> LevelManager::loadTerrain(const std::string& path)
{
	std::ifstream file(path);

	if (!file.good())
	{
		LOG_ERROR(path + " | load failed. Cannot access terrain file");
		return {};
	}

	// get the data from the terrain file
	int tile;
	std::vector<int> tilemap;
	while (file >> tile) tilemap.push_back(tile);

	return tilemap;
}

void LevelManager::loadEnemies(LevelTemplate& level, const std::string& path)
{
	std::ifstream file(path);

	if (!file.good())
	{
		LOG_WARN(path + " | No enemies file found, continuing...");
		return;
	}
	

	// since the file header has to be there (because I said it does)
	// we check the value and ensure this is an enemies file
	std::string fType;
	file >> fType >> fType; // we only care about the second value
	if (fType != "LEVELENEMIES")
	{
		LOG_WARN(path + " | Enemy file is invalud, continuing...");
		return;
	}
	
	LOG_DEBUG(path + " | Enemy file valid! loading...");

	// get the data from the terrain file
	std::string type;
	float x, y;
	std::vector<Enemy*> enemies;

	while (file >> type >> x >> y)
	{
		enemies.push_back(new Enemy());
		auto* enemy = enemies.back() - 1;
		enemy->setSpawnPosition({ x, y });
		
	}

	level.setEnemies(enemies);
}

LevelTemplate* LevelManager::getLevel(std::string name)
{
	if (m_levelIndex.find(name) == m_levelIndex.end()) return nullptr;
	return &m_levelIndex.at(name);
}

void LevelManager::forceSetActiveLevel(std::string name)
{
	m_current = getLevel(name);
}

void LevelManager::setActiveLevel(std::string name)
{
	m_current->onEnd();
	m_current = getLevel(name);
	m_current->onBegin();
}