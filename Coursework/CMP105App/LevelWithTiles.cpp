#include "LevelWithTiles.h"

LevelWithTiles::LevelWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: Scene(window, input, gameState, audio), m_alertText(m_font)
{
	GameObject tile;
	std::vector<GameObject> tileSet;

	int num_columns = 20;
	int num_rows = 9;
	int tile_size = 18;      // Visual size of the tile
	int sheet_spacing = 1;   // Gap between tiles


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

	sf::Vector2u mapDimensions{ 40, 8 };
	std::vector<int> tileMap = {
		b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  ,
		b  , 131, b  , b  , b  , 131, b  , b  , b  , 131,  21,  22,  23, b, b, b,  21,  22,  22,  22,  22,  23, b, b, b,  21,  22,  22,  23, b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  ,
		b  , 131, b  , b  , b  , 131, b  , 21 ,  22, 22 , 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 22 , 22 , 23 , b  , b  , b  , b  , b  , b  , b  , b  ,
		b  , 131, b  , b  , 21 ,  22, 22 , 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 22 , 22 , 22 , 22 , 22 , 22 , 22 , 23 ,
		21 ,  22, 22 , 22 , 121, 122, 122, 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 123,
		121, 122, 122, 122, 121, 122, 122, 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 123,
		121, 122, 122, 122, 121, 122, 122, 121, 122, 122, 121, 122, 123, b, b, b, 121, 122, 122, 122, 122, 123, b, b, b, 121, 122, 122, 123, 122, 122, 123, 122, 122, 122, 122, 122, 122, 122, 123,
		141, 142, 142, 142, 141, 142, 142, 141, 142, 142, 141, 142, 143, b, b, b, 141, 142, 142, 142, 142, 143, b, b, b, 141, 142, 142, 143, 142, 142, 143, 142, 142, 142, 142, 142, 142, 142, 143
	};
	

	m_tilemap.loadTexture("gfx/tilemap.png");
	m_tilemap.setTileSet(tileSet);
	m_tilemap.setTileMap(tileMap, mapDimensions);
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

	mapDimensions = { 14,3 };
	tileMap = {
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		14,14,14,14,14,14,14,14,14,14,14,14,14,14,
		22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22
	};
	m_bgtilemap.loadTexture("gfx/tilemap-backgrounds.png");
	m_bgtilemap.setTileSet(tileSet);
	m_bgtilemap.setTileMap(tileMap, mapDimensions);
	m_bgtilemap.setPosition({ 0, 0 });
	m_bgtilemap.buildLevel();

	// setup player 
	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);
	m_player.setWindow(&m_window);

	//m setup text
	if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
	m_alertText.setString("Who keeps turning\nthe wind off?");
	m_alertText.setPosition({ 50, 150});
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);
	m_promptTimer = PROMPT_TIME;
	if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "no tile image found";

	// setup flags and end game pos
	m_player.setEndGamePosition({ 24, 325 });
	for (int i = 0; i < 3; i++)
	{
		Flag* new_flag = new Flag();
		new_flag->setSize({ 72,72 });
		new_flag->setPosition({72.f + (i * 288), 100.f});
		new_flag->setTexture(&m_tileTexture);
		new_flag->setup();	// ensure first frame is good.
		m_flags.push_back(new_flag);
	}

	m_lever.setPosition({ 2730, 252 });
	m_lever.setTexture(&m_tileTexture);
	m_lever.setSize({ 72,72 });
	m_lever.setUsed(false);
	m_player.setLeverPosition({ 2730, 252 });
	m_player.setAudio(&m_audio);
}

void LevelWithTiles::handleInput(float dt)
{
	m_player.handleInput(dt);

	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
		m_gameState.setCurrentState(State::MENU);
}

void LevelWithTiles::update(float dt)
{

	if (m_flagLeverPulled)
	{
		for (auto& flag : m_flags) flag->update(dt);
	}
	m_lever.update(dt);
	m_player.update(dt);


	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
		}
	}
	
	// show text if player has dropped very low down
	if (m_promptTimer > 0)
		m_promptTimer -= dt;
	else if (m_alertText.getString() != "")
	{
		// turn off prompt
		m_alertText.setString("");
	}
	else if (m_player.getPosition().y > WORLD_SIZE.y)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter());
		m_alertText.setString("Press R to reset");
	}
	// show text if the player in lever range
	else if (m_player.inLeverRange())
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter() + sf::Vector2f(-100.f, -150.f));
		m_promptTimer = PROMPT_TIME;
		if (!m_flagLeverPulled)
			m_alertText.setString("Press F to fix\nthe wind");
		else
			m_alertText.setString("Better check\nthose flags");
	}
	else if (m_player.inEndRange())
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(m_window.getView().getCenter() + sf::Vector2f(-100.f, -150.f));
		m_promptTimer = PROMPT_TIME;
		if (m_flagLeverPulled)
			m_alertText.setString("Good job! Press\nF to end the day");
	}

	if (m_player.getLeverPulled())
	{
		if (!m_flagLeverPulled) m_promptTimer = 0;
		m_flagLeverPulled = true;
		m_lever.setUsed(true);
	}
	else
	{
		m_lever.setUsed(false);
	}
	if (m_player.getGameEndTriggered())
	{
		
		m_gameState.setCurrentState(State::MENU);
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

void LevelWithTiles::updateCameraAndBackground()
{
	auto view = m_window.getView();
	auto player_pos = m_player.getPosition() + m_player.getSize() * 0.5f;

	float halfViewWidth = VIEW_SIZE.x / 2.0f;
	float halfViewHeight = VIEW_SIZE.y / 2.0f;

	player_pos.x = std::clamp(player_pos.x, halfViewWidth, WORLD_SIZE.x - halfViewWidth);
	player_pos.y = std::clamp(player_pos.y, halfViewHeight, WORLD_SIZE.y - halfViewHeight);

	view.setCenter(player_pos);
	m_window.setView(view);

	m_bgtilemap.setPosition({ player_pos.x - halfViewWidth, 0 });
}

void LevelWithTiles::render()
{
	beginDraw();
	m_bgtilemap.render(m_window);
	m_tilemap.render(m_window);
	m_window.draw(m_lever);
	for (auto& flag : m_flags) m_window.draw(*flag);
	m_player.render();
	m_window.draw(m_alertText);
	endDraw();
}

void LevelWithTiles::onBegin()
{
	std::cout << "Level one has been started\n";
	m_audio.playMusicbyName("bgm1");
	
}

void LevelWithTiles::onEnd()
{
	std::cout << "Level one has been left\n";
	// reset player and level state
	m_player.reset();
	m_flagLeverPulled = false;
	// reset alert text
	m_alertText.setString("Who keeps turning\nthe wind off?");
	m_alertText.setPosition({ 50, 150 });
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);
	m_promptTimer = PROMPT_TIME;
	// sfx
	m_audio.stopAllSounds();
	m_audio.stopAllMusic();
}
