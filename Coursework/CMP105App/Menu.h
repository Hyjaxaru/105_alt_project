#pragma once

#include <vector>
#include <map>

#include "Framework/Collision.h"

#include "Scene.h"
#include "LevelTemplate.h" 
#include "Logger.h"
#include "AssetManager.h"
#include "LevelManager.h"
#include "Files.h"


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

    void setLevelIndex(std::map<std::string, LevelTemplate>* index) { m_levelIndex = index; }
    void createLevelButtons();
    
private:
    struct LevelContainer {
        GameObject button;
        sf::Text title;
        sf::Text subtitle;
        std::string levelName;
        KVArray leaderboard;
        GameObject award;
    };

    std::vector<LevelContainer> m_buttonIndex;
    std::map<std::string, LevelTemplate>* m_levelIndex;

    GameObject m_titleImage;
    sf::Texture m_titleSplash;

    sf::Text* m_leaderboard;
    std::string createLeaderboardText(LevelContainer& button);

    AssetManager& m_assets = AssetManager::Instance();

    const sf::Color m_defaultButtonColour = sf::Color(100, 250, 100,  50);
    const sf::Color m_hoverButtonColour =   sf::Color( 80, 230,  80, 150);

    const sf::Vector2f BUTTON_INITIAL = {   8,  8 };
    const sf::Vector2f BUTTON_SIZE =    { 256, 38 };
    const float BUTTON_SPACING =                8.f;

    const sf::Vector2f BUTTON_TITLE_OFFSET =    {   4,  2 };
    const sf::Vector2f BUTTON_SUBTITLE_OFFSET = {   4, 20 };
    const sf::Vector2f BUTTON_AWARD_OFFSET =    { 220,  0 };

    const sf::Vector2f LEADERBOARD_POS = { 275, 130 };
    
};


