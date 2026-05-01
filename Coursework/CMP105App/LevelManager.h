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
	void loadLevel(std::string fileName);

private:
	

	sf::RenderWindow& m_window;
	Input& m_input;
	GameState& m_gameState;
	AudioManager& m_audio;

	std::map<std::string, LevelTemplate> m_levelIndex;

	// Constants
	const std::string LEVELS_DIR = "levels/";
	const std::string LEVEL_LIST_FILE = "_levels.txt";
	const std::string EXTENSION_CONFIG = ".config.txt";
	const std::string EXTENSION_TERRAIN = ".terrain.txt";
};
