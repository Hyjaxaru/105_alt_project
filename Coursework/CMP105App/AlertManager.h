#pragma once

#include <vector>
#include <queue>

#include "Framework/GameObject.h";

#include "AssetManager.h"
#include "Scene.h"
#include "VectorMath.h";

namespace alert
{
	struct Alert {
		std::string imageName;
		std::string title;
	};

	class AlertManager
	{
	public:
		AlertManager(sf::RenderWindow& window, AudioManager& audio);
		~AlertManager() = default;

		//AlertManager();
		//~AlertManager() {}

		//// --- Singleton Instance: See AssetManager for more information --- //

		//AlertManager(const AlertManager&) = delete;
		//AlertManager(AlertManager&&) = delete;

		//AlertManager& operator = (const AlertManager&) = delete;
		//AlertManager& operator = (AlertManager&&) = delete;

		//static AlertManager& Instance()
		//{
		//	static AlertManager instance;
		//	return instance;
		//}

		void update(float dt);
		void render();

		void addToQueue(const Alert& alert);

	private:
		sf::RenderWindow& m_window;
		AudioManager& m_audio;

		std::queue<Alert> m_queue;
		Alert* m_current;

		sf::Vector2f m_uiAnchor;
		sf::Sprite*  m_uiBackground;
		sf::Text*    m_uiTitle;
		sf::Sprite*  m_uiImage;

		sf::Clock m_activeTimer;

		std::optional<Alert> getNext();
		void setCurrentAlert(Alert& alert);

		bool shouldShowAlert();
		bool readyForNextAlert();

		sf::Vector2f calculateDisplayPosition();

		const sf::Time ALERT_VISIBLE_DURATION = sf::seconds(3);
		const sf::Time ALERT_GAP = sf::seconds(0.5f);

		const sf::Vector2f ALERT_POS_ACTIVE =   {    0,  0 };
		const sf::Vector2f ALERT_POS_INACTIVE = { -512,  0 };
		const sf::Vector2f ALERT_POS_PADDING =  {   16, 16 };

		const sf::Vector2f ALERT_OFFSET_IMAGE = { 16, 16 };
		const sf::Vector2f ALERT_OFFSET_TITLE = { 56, 22 };
	};
}
