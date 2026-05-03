// Main
// Entry point of the application.
// Handles window creation and window events.
// Contains the game loop, updates input class and contains the level objects.
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#include <iostream>

#include "Framework/AudioManager.h"
#include "Framework/GameState.h"

#include "AssetManager.h"
#include "LevelWithTiles.h"
#include "LevelTwoWithTiles.h"
#include "Scene.h"
#include "Menu.h"
#include "LevelThatSaves.h"
#include "LevelManager.h"
#include "DataFile.h"

#ifndef SFML_VERSION_MAJOR
	#error "SFML 3 is required for this framework."
#endif


void windowProcess(sf::RenderWindow& window, Input& in)
{
	// Handle window events.
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		if (const auto* resized = event->getIf<sf::Event::Resized>())
		{
			sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(resized->size));
			window.setView(sf::View(visibleArea));
		}
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			in.setKeyDown(keyPressed->scancode);
		}
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			in.setKeyUp(keyReleased->scancode);
		}
		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				in.setLeftMouse(Input::MouseState::PRESSED);
			}
			else
			{
				in.setRightMouse(Input::MouseState::PRESSED);
			}
		}
		if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseReleased->button == sf::Mouse::Button::Left)
			{
				in.setLeftMouse(Input::MouseState::UP);
			}
			else
			{
				in.setRightMouse(Input::MouseState::UP);
			}
		}
		if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			in.setMousePosition(mouseMoved->position.x, mouseMoved->position.y);
		}
		/*
		* There are other events you may wish to poll for, such as:
		* controller or joystick input, scrolling mouse wheel or focus gained/lost
		*/
	}
}


int main()
{
	//Create the window
	sf::RenderWindow window(sf::VideoMode({ 432, 432 }), "Dino Handyman");
	window.setVerticalSyncEnabled(true);

	// Initialise input and manager objects.
	AudioManager audioManager;
	Input input;
	GameState gameState;

	// prepare assets that need to be persistent
	auto& assets = AssetManager::Instance();

	assets.loadTexture("gfx/tilemap.png",             AssetManager::Textures::TERRAIN);
	assets.loadTexture("gfx/tilemap-backgrounds.png", AssetManager::Textures::BACKGROUND);
	assets.loadTexture("gfx/dino1.png",               AssetManager::Textures::PLAYER);
	assets.loadTexture("gfx/gun.png",                 AssetManager::Textures::WEAPON);
	assets.loadTexture("gfx/bullet.png",              AssetManager::Textures::PROJECTILE);

	assets.loadFont("font/arial.ttf", "Arial", AssetManager::LoadOptions::DEFAULT);

	// Create level objects that may reference manager objects
	LevelManager levels(window, input, gameState, audioManager);
	levels.loadLevels();

	Menu menu(window, input, gameState, audioManager);
	menu.createLevelButtons(*levels.getAllLevels());

	Scene* currentScene = &menu;

	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime = 0.f;

	gameState.setCurrentState(State::MENU);
	menu.onBegin();

	std::map<State, Scene*> sceneRegistry =
	{
		{State::MENU, &menu},
		{State::LEVEL, &levels},
	};
	
	// Game Loop
	while (window.isOpen())
	{
		//Process window events
		windowProcess(window, input);

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 0.1f) deltaTime = 0.1f; // Clamp delta time to avoid large jumps

		// handle level manager level change
		State requestedState = gameState.getCurrentState();
		if (requestedState == State::LEVEL)
		{
			std::string levelName = gameState.getCurrentLevel();
			if (levels.getActiveLevel() != levelName)
			{
				LOG_INFO("Switching Level");
				levels.setActiveLevel(levelName);
			}
		}

		// manage overall scene change
		if (sceneRegistry[requestedState] != currentScene)
		{
			LOG_INFO("Switching Scene");
			currentScene->onEnd();
			currentScene = sceneRegistry[requestedState];
			currentScene->onBegin();
		}

		// run the core loop for the current scene
		currentScene->handleInput(deltaTime);
		currentScene->update(deltaTime);
		currentScene->render();

		// Update input class, handle pressed keys
		// Must be done last.
		input.update();
	}

}
