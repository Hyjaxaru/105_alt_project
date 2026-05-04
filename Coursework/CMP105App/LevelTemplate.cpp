#include "LevelTemplate.h"

LevelTemplate::LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio, std::vector<int> terrainSet, sf::Vector2u terrainSize) :
	Scene(window, input, gameState, audio)
{
	GameObject tile;
	std::vector<GameObject> tileSet;

	int num_columns = 20;
	int num_rows = 9;
	int tile_size = 18;      // Visual size of the tile
	int sheet_spacing = 1;   // Gap between tiles

	m_worldSize = sf::Vector2i(terrainSize) * (tile_size * 4);

	// Set GameObject size (Scaling up 4x for visibility)
	// 4 * 18 = 3 * 24 = 72 (dino size is 24).
	tile.setSize(sf::Vector2f(tile_size * 4, tile_size * 4));
	tile.setCollisionBox({ { 0,0 }, tile.getSize() });

	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		if (col <= 4 || col >= 12) tile.setCollider(true);
		else tile.setCollider(false);
		tileSet.push_back(tile);
	}

	// Add Blank
	tile.setTextureRect({ {0, 0}, {-24, -24} }); // Empty rect for blank
	int b = tileSet.size();
	tile.setCollider(false);
	tileSet.push_back(tile);

	auto& tileMap = terrainSet;
	std::replace(tileMap.begin(), tileMap.end(), -1, b);

	m_tilemap.setTexture(AssetManager::Instance().getTexture(AssetManager::Textures::TERRAIN));
	m_tilemap.setTileSet(tileSet);
	m_tilemap.setTileMap(tileMap, terrainSize);
	m_tilemap.setPosition({ 0, 100 });
	m_tilemap.buildLevel();

	tileSet.clear();

	// setup background
	tile_size = 24;
	num_columns = 8;
	num_rows = 3;
	// 24 * 9 = 216, a multiple of 72, the LCM of the player and tile size.
	tile.setSize(sf::Vector2f(tile_size * 9, tile_size * 9));

	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		tile.setCollider(false);		// don't collide with background
		tileSet.push_back(tile);
	}

	tileMap = {
		 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
		22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22
	};
	m_bgTilemap.setTexture(AssetManager::Instance().getTexture(AssetManager::Textures::BACKGROUND));
	m_bgTilemap.setTileSet(tileSet);
	m_bgTilemap.setTileMap(tileMap, { 28, 3 });
	m_bgTilemap.setPosition({ 0, 0 });
	m_bgTilemap.buildLevel();

	// set up player
	m_player.setInput(&m_input);
	m_player.setEdges(0, m_worldSize.x);
	m_player.setWindow(&m_window);
	m_player.setPosition(m_playerSpawn);
	m_player.setAudio(&m_audio);
}

void LevelTemplate::onBegin()
{
	LOG_INFO_NOLINE(debugLevelIdentifier() + " | Loaded")

	m_player.reset();

	for (auto enemy : m_enemies)
		enemy->reset();
}

void LevelTemplate::onEnd()
{
	// reset player
	m_player.reset();
	m_player.setPosition(m_playerSpawn);

	// reset enemies
	for (auto enemy : m_enemies)
		enemy->reset();

	// reset audio
	m_audio.stopAllMusic();
	m_audio.stopAllSounds();

	// reset the timer
	m_timer.stop();
	m_timer.reset();

	LOG_INFO_NOLINE(debugLevelIdentifier() + " | Reset")
}

void LevelTemplate::handleInput(float dt)
{
	m_player.handleInput(dt);

	for (auto enemy : m_enemies)
		enemy->update(dt);

	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
		m_gameState.setCurrentState(State::MENU);
}

void LevelTemplate::update(float dt)
{
	m_player.update(dt);

	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
			m_player.collisionResponse(t);

		// if the collision isn't with the player
		else if (t.isCollider())
		{
			for (auto enemy : m_enemies)
			{
				if (Collision::checkBoundingBox(*enemy, t))
					enemy->collisionResponse(t);
			}
		}
	}

	// reset if fallen too far
	if (m_player.getPosition().y > 1200)
	{
		m_player.reset();
		m_audio.playSoundbyName("death");
	}

	// camera follows player, bounded.
	updateCameraAndBackground();
}

void LevelTemplate::render()
{
	beginDraw();

	m_bgTilemap.render(m_window);
	m_tilemap.render(m_window);

	m_player.render();

	for (auto enemy : m_enemies)
		m_window.draw(*enemy);

	endDraw();
}

void LevelTemplate::updateCameraAndBackground()
{
	sf::View view =   m_window.getView();
	auto player_pos = m_player.getPosition() + m_player.getSize() * 0.5f;

	float halfViewWidth =  m_viewSize.x / 2.0f;
	float halfViewHeight = m_viewSize.y / 2.0f;

	player_pos.x = std::clamp(player_pos.x, halfViewWidth,  m_worldSize.x - halfViewWidth);
	player_pos.y = std::clamp(player_pos.y, halfViewHeight, m_worldSize.y - halfViewHeight);

	view.setCenter(player_pos);
	m_window.setView(view);

	m_bgTilemap.setPosition({ player_pos.x - halfViewWidth, player_pos.y - halfViewHeight });
}