#pragma once

#include <vector>
#include <queue>

#include "Framework/GameObject.h";

#include "Scene.h"

namespace alert
{
	struct Alert {
		
	};

	class AlertManager
	{
	public:
		AlertManager(sf::RenderWindow& window, AudioManager& audio);
		~AlertManager() = default;

		void update(float dt);
		void render();

		void addToQueue(const Alert& alert);

	private:
		sf::RenderWindow& m_window;
		AudioManager& m_audio;

		std::queue<Alert> m_queue;
		Alert* m_current;

		sf::Clock m_activeTimer;

		std::optional<Alert> getNext();

		inline bool shouldShowAlert() {
			if (m_current == nullptr) return false;
			return m_activeTimer.getElapsedTime().asMilliseconds() < ALERT_VISIBLE_DURATION.asMilliseconds();
		}

		const sf::Time ALERT_VISIBLE_DURATION = sf::seconds(3);
		const sf::Time ALERT_GAP = sf::seconds(0.5f);

		const sf::Vector2f ALERT_POS_ACTIVE = { 0, 0 };
		const sf::Vector2f ALERT_POS_INACTIVE = { -1024, 0 };
	};
}
