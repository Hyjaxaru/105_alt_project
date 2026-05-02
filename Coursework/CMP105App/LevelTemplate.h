#pragma once

#include <algorithm>
#include <vector>

#include "Framework/Collision.h"
#include "Framework/TileMap.h"

#include "Scene.h"
#include "Player.h"
#include "Logger.h"

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
		sf::Vector2u terrainSize,
		sf::Vector2u backgroundSize
	);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	void onBegin() override;
	void onEnd() override;

	void updateCameraAndBackground();

	void setWorldSize(sf::Vector2i const size) { m_worldSize = size; }
	void setViewSize(sf::Vector2i const size) { m_viewSize = size; }
	void setPlayerSpawn(sf::Vector2f const pos) { m_playerSpawn = pos; }
	void setGoalLocation(sf::Vector2f const pos) { m_goalPosition = pos; }

	std::pair<std::string, std::string> getLevelMetadata() { return m_metadata; }
	void setLevelMetadata(std::pair<std::string, std::string> metadata) { m_metadata = metadata; }


private:
	std::pair<std::string, std::string> m_metadata;

	TileMap m_tilemap;
	TileMap m_bgTilemap;

	Player m_player;
	sf::Vector2f m_playerSpawn;

	sf::Vector2f m_goalPosition;

	sf::Clock m_timer;

	sf::Vector2i m_worldSize;
	sf::Vector2i m_viewSize;

	inline std::string debugLevelIdentifier() const
	{
		return m_metadata.first + ':' + m_metadata.second;
	}
};

