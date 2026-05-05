#include "Menu.h"

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud)
{
	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });

	// initialise the leaderboard text
	m_leaderboard = new sf::Text(*AssetManager::Instance().getDefaultFont());
	m_leaderboard->setCharacterSize(12);
	m_leaderboard->setFillColor(sf::Color::White);
	m_leaderboard->setPosition(LEADERBOARD_POS);
}

void Menu::onBegin()
{
	LOG_INFO("starting menu");

	auto view = m_window.getDefaultView();
	view.setCenter({ 216, 216 });
	m_window.setView(view);
	m_audio.playMusicbyName("bgm2");

	createLevelButtons();
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
			if (Collision::checkBoundingBox(button.button, mousePos))
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

	std::string leaderboardText = "";

	for (auto& button : m_buttonIndex)
	{
		if (Collision::checkBoundingBox(button.button, mousePos))
		{
			button.button.setFillColor(m_hoverButtonColour);
			leaderboardText = createLeaderboardText(button);
		}
		else
			button.button.setFillColor(m_defaultButtonColour);
	}

	m_leaderboard->setString(leaderboardText);
}

void Menu::render()
{
	beginDraw();

	m_window.draw(m_titleImage);

	for (auto& button : m_buttonIndex)
	{
		m_window.draw(button.button);
		m_window.draw(button.title);
		m_window.draw(button.subtitle);
		m_window.draw(button.award);
	}

	m_window.draw(*m_leaderboard);
}

void Menu::createLevelButtons()
{
	m_buttonIndex.clear();

	int i = 0;
	for (auto& pair : *m_levelIndex)
	{
		// get data
		auto font = m_assets.getDefaultFont();
		auto dFile = DataFile(files::DATA_DIR + pair.first + files::EXTENSION_LEADERBOARD);
		auto leaderboard = dFile.getArray();
		
		// make sure the leaderboard is sorted ( I love lambda functions!!! )
		// source: https://stackoverflow.com/a/279878
		std::sort(leaderboard.begin(), leaderboard.end(), [](auto& left, auto& right) {
			return left.second < right.second;
		});

		// create the new button
		auto button = LevelContainer{ GameObject(), { *font }, { *font }, pair.first, leaderboard };

		// calculate button position
		// initial position + ( (height + spacing) * number of buttons )
		auto pos = BUTTON_INITIAL + sf::Vector2f{ 0, (BUTTON_SIZE.y + BUTTON_SPACING) * static_cast<float>(i) };

		// configure button
		button.button.setPosition(pos);
		button.button.setSize(BUTTON_SIZE);
		button.button.setCollisionBox({ {0,0}, button.button.getSize() });
		button.button.setFillColor(m_defaultButtonColour);

		// configure the button's title
		button.title.setPosition(pos + BUTTON_TITLE_OFFSET);
		button.title.setCharacterSize(16);
		button.title.setString(pair.first);
		button.title.setFillColor(sf::Color::White);

		// configure the button's subtitle
		button.subtitle.setPosition(pos + BUTTON_SUBTITLE_OFFSET);
		button.subtitle.setCharacterSize(12);
		button.subtitle.setString("Created by " + pair.second.getLevelMetadata().author);
		button.subtitle.setFillColor(sf::Color::Cyan);

		// configure award sprite
		auto awardID = dFile.getInt("__AWARD").value_or(4);
		std::stringstream s;
		s << "Medal" << awardID;
		button.award.setTexture(m_assets.getTexture(s.str()));
		button.award.setPosition(pos + BUTTON_AWARD_OFFSET);
		button.award.setSize({ 24, 32 });

		// add the button to the index
		m_buttonIndex.push_back(button);

		// increment index
		// since we are directly iterating on the map, and maps do not support random access by index,
		// we keep track of the index independantly so we can still calculate the placement offsets and the keys
		i++;
	}
}

std::string Menu::createLeaderboardText(LevelContainer& button)
{
	std::stringstream s;
	s << button.levelName << std::endl;

	for (auto& pair : button.leaderboard)
		s << pair.first << ": " << stof(pair.second)/1000 << 's' << std::endl;

	return s.str();
}