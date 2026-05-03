#pragma once

#include <vector>
#include <map>

#include "Framework/Collision.h"

#include "Scene.h"
#include "LevelTemplate.h" 
#include "Logger.h"
#include "AssetManager.h"


class Menu :
    public Scene
{
public:
    Menu(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

    void createLevelButtons(std::map<std::string, LevelTemplate>& levelIndex);
    

private:
    struct LevelButton {
        GameObject obj;
        sf::Text title;
        sf::Text subtitle;
        std::string levelName;
    };

    std::vector<LevelButton> m_buttonIndex;

    GameObject m_titleImage;
    sf::Texture m_titleSplash;

    AssetManager& m_assets = AssetManager::Instance();

    const sf::Color m_defaultButtonColour = sf::Color(100, 250, 100,  50);
    const sf::Color m_hoverButtonColour =   sf::Color( 80, 230,  80, 150);

    const sf::Vector2f BUTTON_INITIAL = {   8,  8 };
    const sf::Vector2f BUTTON_SIZE =    { 256, 38 };
    const float BUTTON_SPACING =                8.f;

    const sf::Vector2f BUTTON_TITLE_OFFSET =    { 4,  2 };
    const sf::Vector2f BUTTON_SUBTITLE_OFFSET = { 0, 18 };
};

