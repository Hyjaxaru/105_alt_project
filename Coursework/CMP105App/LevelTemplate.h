#pragma once

#include "Framework/Collision.h"
#include "Framework/TileMap.h"

#include "Scene.h"
#include "Player.h"
#include "Logger.h"

class LevelTemplate :
	public Scene
{
public:
	LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	void onBegin() override;
	void onEnd() override;

	sf::Vector2i getWorldSize() { return m_worldSize; }
	void setWorldSize(sf::Vector2i const size) { m_worldSize = size; }

	sf::Vector2i getViewSize() { return m_viewSize; }
	void setViewSize(sf::Vector2i const size) { m_viewSize = size; }

private:
	TileMap m_tilemap;
	TileMap m_bgTilemap;
	Player m_player;
	sf::Texture* m_tileTexture;

	sf::Vector2i m_worldSize;
	sf::Vector2i m_viewSize;
};

