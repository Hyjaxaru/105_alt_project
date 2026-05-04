#pragma once

#include <algorithm>
#include <vector>

#include "Framework/Collision.h"
#include "Framework/TileMap.h"

#include "Scene.h"
#include "Player.h"
#include "Logger.h"
#include "Enemy.h";
#include "Goal.h";
#include "AssetManager.h";
#include "DataFile.h";

class LevelTemplate :
	public Scene
{
public:
	LevelTemplate(
		sf::RenderWindow& window,
		Input& input,
		GameState& gameState,
		AudioManager& audio,
		std::vector<int> terrainSet,
		sf::Vector2u terrainSize
	);

	struct Metadata {
		std::string name;
		std::string author;
		DataFile* leaderboard;
	};

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	void onBegin() override;
	void onEnd() override;

	void updateCameraAndBackground();

	void setWorldSize(sf::Vector2i const size)       { m_worldSize    = size; }
	void setViewSize(sf::Vector2i const size)        { m_viewSize     = size; }
	void setPlayerSpawn(sf::Vector2f const pos)      { m_playerSpawn  = pos; }
	void setEnemies(std::vector<Enemy*> enemies)     { m_enemies      = enemies; }

	void setGoalLocation(sf::Vector2f const pos) { m_goal.setPosition(pos); }
	void setWeaponAmmo(const int& ammo) { m_player.getWeapon()->setAmmoMax(ammo); }

	Metadata& getLevelMetadata() { return m_metadata; }


private:
	Metadata m_metadata;

	TileMap m_tilemap;
	TileMap m_bgTilemap;

	Player m_player;
	PlayerGun* m_playerWeapon;
	sf::Vector2f m_playerSpawn;

	std::vector<Enemy*> m_enemies;

	Goal m_goal;

	sf::Clock m_timer;

	sf::Vector2i m_worldSize;
	sf::Vector2i m_viewSize;

	inline std::string debugLevelIdentifier() const { return m_metadata.name + ':' + m_metadata.author; }

	const float GOAL_RANGE = 10.f;
};

