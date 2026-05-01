#include "LevelTemplate.h"

LevelTemplate::LevelTemplate(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	Scene(window, input, gameState, audio)
{
}

void LevelTemplate::onBegin()
{
	LOG_INFO_NOLINE(m_name + "Loaded")
}

void LevelTemplate::onEnd()
{
	// reset the player
	m_player.reset();

	// reset audio
	m_audio.stopAllMusic();
	m_audio.stopAllSounds();

	// reset the timer
	m_timer.stop();
	m_timer.reset();

	LOG_INFO_NOLINE(m_name + "Reset")
}

void LevelTemplate::handleInput(float dt)
{
	m_player.handleInput(dt);

	if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
		m_gameState.setCurrentState(State::MENU);
}

void LevelTemplate::update(float dt)
{
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