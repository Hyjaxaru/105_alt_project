#pragma once

#include <map>
#include <fstream>

#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Framework/GameObject.h"

#include "AssetManager.h"
#include "Logger.h"
#include "Scene.h"

class LevelManager
{
public:
	LevelManager() = default;

	void loadLevel(std::string levelPath);

	void loadLevels(std::string dirPath);
	void loadLevels() { loadLevels(DEFAULT_LEVEL_DIR); }

private:
	std::map<std::string, int> m_levelIndex;

	inline std::string makeFileLoadFailErrorMessage(std::string filePath, std::string message) {
		return "Failed to load level '" + filePath + "', " + message;
	}

	const std::string DEFAULT_LEVEL_DIR = "levels/";
	const std::string LEVEL_FILE_EXT = "level";
	const int LOADER_VERSION = 1;
};

