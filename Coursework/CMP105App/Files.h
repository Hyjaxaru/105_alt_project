#pragma once

#include <string>

// constants (public and static so they can be accessed outside of LevelManager)
namespace files {
	inline static const std::string LEVELS_DIR = "levels/";
	inline static const std::string DATA_DIR = "data/";
	inline static const std::string LEVEL_LIST_FILE = "_levels.txt";
	inline static const std::string EXTENSION_CONFIG = ".config.txt";
	inline static const std::string EXTENSION_TERRAIN = ".terrain.txt";
	inline static const std::string EXTENSION_ENEMIES = ".enemies.txt";
	inline static const std::string EXTENSION_LEADERBOARD = ".txt";
	inline static const std::string EXTENSION_DATA = ".txt.";
}