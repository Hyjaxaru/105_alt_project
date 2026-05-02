#pragma once

#include <map>
#include <fstream>

#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Framework/GameObject.h"

#include "AssetManager.h"
#include "Logger.h"
#include "LevelTemplate.h"
#include "DataFile.h"

class LevelManager
{
public:
	LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

	void loadLevels();
	std::vector<std::string> loadLevelManifest();
	std::optional<LevelTemplate> loadLevel(std::string fileName);

	LevelTemplate* getLevel(std::string name);

private:
	sf::RenderWindow& m_window;
	Input& m_input;
	GameState& m_gameState;
	AudioManager& m_audio;

	std::vector<GameObject> m_tsTerrain;
	std::vector<GameObject> m_tsBackground;

	std::map<std::string, LevelTemplate> m_levelIndex;
	
	void createTerrainTileSet();
	void createBackgroundTileSet();

	TileMap createTerrainTileMap(std::vector<int> tilemap, const sf::Vector2u& dimensions);
	TileMap createBackgroundTileMap(const sf::Vector2u& dimensions);

	AssetManager& m_assets = AssetManager::Instance();

	// Constants
	const std::string LEVELS_DIR = "levels/";
	const std::string LEVEL_LIST_FILE = "_levels.txt";
	const std::string EXTENSION_CONFIG = ".config.txt";
	const std::string EXTENSION_TERRAIN = ".terrain.txt";
};
