#pragma once

#include <map>
#include <fstream>

#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Framework/GameObject.h"

#include "AssetManager.h"
#include "Logger.h"
#include "LevelTemplate.h"

class LevelManager
{
public:
	LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

	void loadLevel(std::string levelPath);

	void loadLevels(std::string dirPath);
	void loadLevels() { loadLevels(DEFAULT_LEVEL_DIR); }

private:
	std::map<std::string, int> m_levelIndex;

	struct Level {
		std::ifstream& file;
		std::string path;
		std::string name;
		LevelTemplate level;
	};

	enum class TileMapType { TERRAIN, BACKGROUND };

	bool loadVersion(Level& level);
	bool loadTileset(Level& level, TileMapType type);
	bool loadTilemap(Level& level, TileMapType type);

	inline std::string makeFileLoadFailErrorMessage(std::string filePath, std::string message) {
		return "Failed to load level '" + filePath + "', " + message;
	}

	sf::RenderWindow& m_window;
	Input& m_input;
	GameState& m_gameState;
	AudioManager& m_audio;

	const std::string DEFAULT_LEVEL_DIR = "levels/";
	const std::string LEVEL_FILE_EXT = "level";
	const int LOADER_VERSION = 1;
};

