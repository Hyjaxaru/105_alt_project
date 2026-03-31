#include "Menu.h"

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud), m_playButtonLabel(m_font), m_playButton2Label(m_font)
{
	if (!m_font.openFromFile("font/bitcount.ttf"))
		std::cerr << "failed to load bitcount font";

	m_playButtonLabel.setCharacterSize(24);		// setup labels
	m_playButtonLabel.setPosition({ 185,93 });
	m_playButtonLabel.setString("Level 1");
	m_playButtonLabel.setFillColor(sf::Color::Black);
	m_playButton2Label.setCharacterSize(24);
	m_playButton2Label.setPosition({ 185,233 });
	m_playButton2Label.setString("Level 2");
	m_playButton2Label.setFillColor(sf::Color::Black);


	m_playButton.setSize({ 216,100 });			// setup buttons
	m_playButton.setPosition({ 108,58 });
	m_playButton.setCollisionBox({ {0,0}, m_playButton.getSize()});
	m_playButton.setFillColor(m_defaultButtonColour); 
	m_play2Button.setSize({ 216,100 });			
	m_play2Button.setPosition({ 108,198 });
	m_play2Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_play2Button.setFillColor(m_defaultButtonColour);

	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });
}

void Menu::handleInput(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY()};
	if(m_input.isLeftMousePressed() && 
		Collision::checkBoundingBox(m_playButton, mousePos))
	{
		m_gameState.setCurrentState(State::LEVELONE);
	}
	if (m_input.isLeftMousePressed() &&
		Collision::checkBoundingBox(m_play2Button, mousePos))
	{
		m_gameState.setCurrentState(State::LEVELTWO);
	}
}

void Menu::render()
{
	beginDraw();
	m_window.draw(m_titleImage);
	m_window.draw(m_playButton);
	m_window.draw(m_playButtonLabel);
	m_window.draw(m_play2Button);
	m_window.draw(m_playButton2Label);
	endDraw();
}

void Menu::update(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };
	if (Collision::checkBoundingBox(m_playButton, mousePos))
	{
		m_playButton.setFillColor(m_hoverButtonColour);
	}
	else
	{
		m_playButton.setFillColor(m_defaultButtonColour);
	}
	if (Collision::checkBoundingBox(m_play2Button, mousePos))
	{
		m_play2Button.setFillColor(m_hoverButtonColour);
	}
	else
	{
		m_play2Button.setFillColor(m_defaultButtonColour);

	}
}

void Menu::onBegin()
{
	LOG_INFO("starting menu");
	auto view = m_window.getDefaultView();
	view.setCenter({ 216, 216 });
	m_window.setView(view);
	m_audio.playMusicbyName("bgm2");
}

void Menu::onEnd()
{
	LOG_INFO("leaving menu");
	m_audio.stopAllMusic();
}