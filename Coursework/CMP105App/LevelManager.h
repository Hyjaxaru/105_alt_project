#pragma once

#include <map>
#include <fstream>

#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Framework/GameObject.h"

#include "Scene.h"
#include "AssetManager.h"
#include "Logger.h"
#include "LevelTemplate.h"
#include "DataFile.h"
#include "Enemy.h";

class LevelManager
	: public Scene
{
public:
	LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

	void onBegin() override { m_current->onBegin(); }
	void onEnd() override { m_current->onEnd(); }
	void handleInput(float dt) override { m_current->handleInput(dt); }
	void update(float dt) override { m_current->update(dt); }
	void render() override { m_current->render(); }

	void loadLevels();
	std::vector<std::string> loadLevelManifest();
	std::optional<LevelTemplate> loadLevel(std::string fileName);

	LevelTemplate* getLevel(std::string name);
	std::map<std::string, LevelTemplate>* getAllLevels() { return &m_levelIndex; }

	std::string getActiveLevel() { return m_current->getLevelMetadata().name; }
	void setActiveLevel(std::string name);
	void forceSetActiveLevel(std::string name);

	// constants (public and static so they can be accessed outside of LevelManager)
	inline static const std::string LEVELS_DIR = "levels/";
	inline static const std::string DATA_DIR = "data/";
	inline static const std::string LEADERBOARD_DIR = "lb/";
	inline static const std::string LEVEL_LIST_FILE = "_levels.txt";
	inline static const std::string EXTENSION_CONFIG = ".config.txt";
	inline static const std::string EXTENSION_TERRAIN = ".terrain.txt";
	inline static const std::string EXTENSION_ENEMIES = ".enemies.txt";
	inline static const std::string EXTENSION_LEADERBOARD = ".txt";
	inline static const std::string EXTENSION_DATA = ".txt.";

private:
	std::vector<GameObject> m_tsTerrain;
	std::vector<GameObject> m_tsBackground;

	std::map<std::string, LevelTemplate> m_levelIndex;
	LevelTemplate* m_current;

	AssetManager& m_assets = AssetManager::Instance();

	void configureLevel(LevelTemplate& level, DataFile& config);
	std::vector<int> loadTerrain(const std::string& path);
	void loadEnemies(LevelTemplate& level, const std::string& path);
};
