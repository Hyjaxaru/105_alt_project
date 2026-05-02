#include "LevelManager.h"

namespace fs = std::filesystem;

LevelManager::LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	Scene(window, input, gameState, audio)
{
	createTerrainTileSet();
	createBackgroundTileSet();
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

	// begin creating the world
	auto level = LevelTemplate(m_window, m_input, m_gameState, m_audio);

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

	// create and set the terrain tilemap
	sf::Vector2u terrainSize = {
		static_cast<unsigned int>(config.getInt("terrainX").value_or(0)),
		static_cast<unsigned int>(config.getInt("terrainY").value_or(0)),
	};
	level.setTerrainTileMap(createTerrainTileMap(tilemap, terrainSize));

	// create and set the background tilemap
	sf::Vector2u backgroundSize = {
		static_cast<unsigned int>(config.getInt("backgroundX").value_or(0)),
		static_cast<unsigned int>(config.getInt("backgroundY").value_or(0)),
	};
	level.setBackgroundTileMap(createBackgroundTileMap(backgroundSize));

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

void LevelManager::createTerrainTileSet()
{
	// Repurposed from LevelWithTiles.cpp

	GameObject tile;
	std::vector<GameObject> tileSet;

	int num_columns = 20;
	int num_rows = 9;
	int tile_size = 18;      // Visual size of the tile
	int sheet_spacing = 1;   // Gap between tiles

	// Set GameObject size (Scaling up 4x for visibility)
	// 4 * 18 = 3 * 24 = 72 (dino size is 24).
	tile.setSize(sf::Vector2f(tile_size * 4, tile_size * 4));
	tile.setCollisionBox({ { 0,0 }, tile.getSize() });


	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		if (col <= 4 || col >= 12) tile.setCollider(true);
		else tile.setCollider(false);
		tileSet.push_back(tile);

		// Add Blank
		tile.setTextureRect({ {0, 0}, {-24, -24} }); // Empty rect for blank
		int b = m_tsTerrain.size();
		tile.setCollider(false);
		tileSet.push_back(tile);
	}

	m_tsTerrain = tileSet;
}

void LevelManager::createBackgroundTileSet()
{
	GameObject tile;
	std::vector<GameObject> tileSet;

	int num_columns = 8;
	int num_rows = 3;
	int tile_size = 24;
	int sheet_spacing = 1;

	// 24 * 9 = 216, a multiple of 72, the LCM of the player and tile size.
	tile.setSize(sf::Vector2f(tile_size * 9, tile_size * 9));

	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		tile.setCollider(false);		// don't collide with background
		tileSet.push_back(tile);
	}

	m_tsBackground = tileSet;
}

TileMap LevelManager::createTerrainTileMap(std::vector<int> tilemap, const sf::Vector2u& dimensions)
{
	// get last item for background, and replace the file placeholder (-1) with it
	int b = m_tsTerrain.size() - 1;
	std::replace(tilemap.begin(), tilemap.end(), -1, b);

	TileMap tileMap;
	tileMap.setTexture(*m_assets.getTexture(AssetManager::Textures::TERRAIN));
	tileMap.setTileMap(tilemap, dimensions);
	tileMap.setPosition({ 0, 0 });

	return tileMap;
}

TileMap LevelManager::createBackgroundTileMap(const sf::Vector2u& dimensions)
{
	// get last item for background
	int b = m_tsTerrain.size() - 1;

	// this is a test
	std::vector<int> tilemap = {
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		14,14,14,14,14,14,14,14,14,14,14,14,14,14,
		22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22
	};

	TileMap tileMap;
	tileMap.setTexture(*m_assets.getTexture(AssetManager::Textures::BACKGROUND));
	tileMap.setTileMap(tilemap, dimensions);
	tileMap.setPosition({ 0, 0 });

	return tileMap;
}