#include "LevelTemplate.h"

LevelTemplate::LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	Scene(window, input, gameState, audio)
{
	m_tilemap.buildLevel();
	m_bgTilemap.buildLevel();

	// set up player
	m_player.setInput(&m_input);
	m_player.setEdges(0, m_worldSize.x);
	m_player.setWindow(&m_window);
	m_player.setPosition(m_playerSpawn);
}

void LevelTemplate::onBegin()
{
	LOG_INFO_NOLINE(debugLevelIdentifier() + " | Loaded")
}

void LevelTemplate::onEnd()
{
	// reset player
	m_player.reset();
	m_player.setPosition(m_playerSpawn);

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
	LOG_DEBUG(debugLevelIdentifier() + " | Handle Input");

	m_player.handleInput(dt);

	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
		m_gameState.setCurrentState(State::MENU);
}

void LevelTemplate::update(float dt)
{
	LOG_DEBUG(debugLevelIdentifier() + " | Update");

	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
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
	LOG_DEBUG(debugLevelIdentifier() + " | Render");

	beginDraw();

	m_bgTilemap.render(m_window);
	m_tilemap.render(m_window);
	
	m_player.render();

	endDraw();
}

void LevelTemplate::updateCameraAndBackground()
{
	auto view = m_window.getView();
	auto player_pos = m_player.getPosition() + m_player.getSize() * 0.5f;

	float halfViewWidth = m_viewSize.x / 2.0f;
	float halfViewHeight = m_viewSize.y / 2.0f;

	player_pos.x = std::clamp(player_pos.x, halfViewWidth, m_worldSize.x - halfViewWidth);
	player_pos.y = std::clamp(player_pos.y, halfViewHeight, m_worldSize.y - halfViewHeight);

	view.setCenter(player_pos);
	m_window.setView(view);

	m_bgTilemap.setPosition({ player_pos.x - halfViewWidth, 0 });
}