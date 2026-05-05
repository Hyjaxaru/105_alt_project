#include "TileMap.h"

// Constructor sets default position value.
TileMap::TileMap()
{
	m_position = sf::Vector2f(0, 0);
	m_mapDimensions = { 0,0 };
}

TileMap::~TileMap()
{
	// The vector will handle memory management for us
}

// Uses window pointer to render level/section. Tile by Tile.
void TileMap::render(sf::RenderWindow& window)
{
	for (auto& tile : m_levelAssets)
	{
		window.draw(tile);
	}
}

// Loads and stores the spritesheet containing all the tiles required to build the level/section
void TileMap::loadTexture(const std::string& filename)
{
	m_texture = new sf::Texture();
	if (!m_texture->loadFromFile(filename))
	{
		std::cerr << "Failed to load tilemap file, filename: " << filename << std::endl;
	}
}

// Receives an array of GameObjects representing the tile set (in order)
void TileMap::setTileSet(const std::vector<GameObject>& ts)
{
	m_tileSet = ts;
}

// Receives and array of integers and map dimensions representing the map (where and what tiles to place).
void TileMap::setTileMap(const std::vector<int>& tm, sf::Vector2u mapDimensions)
{
	m_tileMap = tm;
	m_mapDimensions = mapDimensions;
}

// Once provided with the map and tile set, builds the level, creating an array of tile sprites positioned based on the map. Ready to render.
void TileMap::buildLevel()
{
	if (m_tileSet.empty() || m_tileMap.empty()) return;	// nothing to build.

	m_levelAssets.clear();

	int x = 0;
	int y = 0;

	// Assuming all tiles in the set are the same size, take the size of the first one
	sf::Vector2f tileSize(m_tileSet[0].getSize().x, m_tileSet[0].getSize().y);

	for (int i = 0; i < static_cast<int>(m_tileMap.size()); i++)
	{
		// Calculate Grid Position
		x = i % m_mapDimensions.x;
		y = i / m_mapDimensions.x; // Integer division acts as floor()

		// Get the specific tile ID from the map
		int tileID = m_tileMap[i];

		// Safety Check: ensure the ID is within the tileset bounds
		if (tileID >= 0 && tileID < static_cast<int>(m_tileSet.size()))
		{
			// Create the new tile from the prototype in tileSet
			GameObject newTile = m_tileSet[tileID];

			// Set Position: Map Position + (Grid * TileSize)
			newTile.setPosition({
				m_position.x + (x * tileSize.x),
				m_position.y + (y * tileSize.y)
			});

			// Apply Texture explicitly (pointers need to be fresh)
			newTile.setTexture(m_texture);

			// Add to world
			m_levelAssets.push_back(newTile);
		}
	}

}