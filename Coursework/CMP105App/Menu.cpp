#include "Menu.h"

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud)
{
//	if (!m_font.openFromFile("font/bitcount.ttf"))
//		std::cerr << "failed to load bitcount font";
//
//	m_playButtonLabel.setCharacterSize(24);		// setup labels
//	m_playButtonLabel.setPosition({ 185,93 });
//	m_playButtonLabel.setString("Level 1");
//	m_playButtonLabel.setFillColor(sf::Color::Black);
//	m_playButton2Label.setCharacterSize(24);
//	m_playButton2Label.setPosition({ 185,233 });
//	m_playButton2Label.setString("Level 2");
//	m_playButton2Label.setFillColor(sf::Color::Black);
//
//
//	m_playButton.setSize({ 256, 32 });			// setup buttons
//	m_playButton.setPosition({ 16, 16 });
//	m_playButton.setCollisionBox({ {0,0}, m_playButton.getSize()});
//	m_playButton.setFillColor(m_defaultButtonColour); 
//	m_play2Button.setSize({ 216,100 });			
//	m_play2Button.setPosition({ 108,198 });
//	m_play2Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
//	m_play2Button.setFillColor(m_defaultButtonColour);

	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });
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

void Menu::handleInput(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY()};

	if (m_input.isLeftMousePressed())
	{
		// only do this collision check if the left mouse button is pressed
		for (auto& button : m_buttonIndex)
		{
			if (Collision::checkBoundingBox(button.obj, mousePos))
			{
				m_gameState.setCurrentState(State::LEVEL);
				m_gameState.setCurrentLevel(button.levelName);
			}
		}
	}
	
	/*if(m_input.isLeftMousePressed() && 
		Collision::checkBoundingBox(m_playButton, mousePos))
	{
		m_gameState.setCurrentState(State::LEVEL);
		m_gameState.setCurrentLevel("Level1");
	}
	if (m_input.isLeftMousePressed() &&
		Collision::checkBoundingBox(m_play2Button, mousePos))
	{
		m_gameState.setCurrentState(State::LEVEL);
	}*/
}

void Menu::update(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };

	for (auto& button : m_buttonIndex)
	{
		if (Collision::checkBoundingBox(button.obj, mousePos))
			button.obj.setFillColor(m_hoverButtonColour);
		else
			button.obj.setFillColor(m_defaultButtonColour);
	}
}

void Menu::render()
{
	beginDraw();

	m_window.draw(m_titleImage);

	for (auto& button : m_buttonIndex)
	{
		m_window.draw(button.obj);
		m_window.draw(button.title);
		m_window.draw(button.subtitle);
	}

	endDraw();
}

void Menu::createLevelButtons(std::map<std::string, LevelTemplate>& levelIndex)
{
	int i = 0;
	for (auto& pair : levelIndex)
	{
		// create a new button
		auto font = m_assets.getDefaultFont();
		auto button = LevelButton{ GameObject(), sf::Text(*font), sf::Text(*font), pair.first };

		// calculate button position
		// initial position + ( (height + spacing) * number of buttons )
		auto pos = BUTTON_INITIAL + sf::Vector2f{ 0, (BUTTON_SIZE.y + BUTTON_SPACING) * static_cast<float>(i) };

		// configure button
		button.obj.setPosition(pos);
		button.obj.setSize(BUTTON_SIZE);
		button.obj.setCollisionBox({ {0,0}, button.obj.getSize() });
		button.obj.setFillColor(m_defaultButtonColour);

		// configure the button's title
		button.title.setPosition(pos + BUTTON_TITLE_OFFSET);
		button.title.setCharacterSize(16);
		button.title.setString(pair.first);
		button.title.setFillColor(sf::Color::White);

		button.subtitle.setPosition(pos + BUTTON_TITLE_OFFSET + BUTTON_SUBTITLE_OFFSET);
		button.subtitle.setCharacterSize(12);
		button.subtitle.setString("Created by " + pair.second.getLevelMetadata().author);
		button.subtitle.setFillColor(sf::Color::Cyan);

		// add the button to the index
		m_buttonIndex.push_back(button);

		// increment index
		// since we are directly iterating on the map, and maps do not support random access by index,
		// we keep track of the index independantly so we can still calculate the placement offsets and the keys
		i++;
	}
}