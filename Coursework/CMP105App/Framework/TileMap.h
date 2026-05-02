// Tile Map Class
// This class represents a Tile Map environment for rendering.
// Builds and store level sections based on Map and TileSet
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include "GameObject.h"

class TileMap
{
public:
	TileMap();
	~TileMap();

	// Loads and stores a reference to the spritesheet containing all the tiles required to build the level/section
	void loadTexture(const std::string& filename);
	// Sets the spritesheet contained within directly
	void setTexture(sf::Texture tex) { m_texture = tex; }
	// Receives a reference to an array of GameObjects representing the tile set (in order)
	void setTileSet(const std::vector<GameObject>& ts);
	// Receives a reference to an array of integers and map dimensions representing the map (where and what tiles to place).
	void setTileMap(const std::vector<int>& tm, sf::Vector2u mapDimensions);
	// Once provided with the map and tile set, builds the level, creating an array of tile sprites positioned based on the map. Ready to render.
	void buildLevel();

	// Receives window handle and renders the level/tilemap
	void render(sf::RenderWindow& window);
	// Returns the built level tile map. Used for collision detection, etc, where we need access to elements of the level.
	std::vector<GameObject>* getLevel(){ return &m_levelAssets; };

	// Set the origin position of the tilemap section. 
	void setPosition(sf::Vector2f pos) { m_position = pos; };

protected:
	std::vector<GameObject> m_tileSet;
	std::vector<int> m_tileMap;
	std::vector<GameObject> m_levelAssets;
	sf::Texture m_texture;
	sf::Vector2u m_mapDimensions;
	sf::Vector2f m_position;
};

