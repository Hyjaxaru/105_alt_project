#pragma once

#include <vector>
#include <queue>

#include "Framework/GameObject.h";

#include "AssetManager.h"
#include "Scene.h"
#include "VectorMath.h";

namespace Alert
{
	struct Data {
		std::string imageName;
		std::string title;
	};

	class Queue
	{
	public:
		Queue() = default;
		~Queue() = default;

		Queue(const Queue&) = delete;
		Queue(Queue&&) = delete;

		Queue& operator = (const Queue&) = delete;
		Queue& operator = (Queue&&) = delete;

		static Queue& Instance()
		{
			static Queue instance;
			return instance;
		}

		std::queue<Data>& getQueue() { return m_queue; }
		void push(const Data& alert) { m_queue.push(alert); }

	private:
		std::queue<Data> m_queue;
	};

	class Manager
	{
	public:
		Manager(sf::RenderWindow& window, AudioManager& audio);
		~Manager() = default;

		void update(float dt);
		void render();

	private:
		sf::RenderWindow& m_window;
		AudioManager& m_audio;
		AssetManager& m_assets = AssetManager::Instance();

		Alert::Queue& m_queue = Queue::Instance();
		Data* m_current;

		sf::Vector2f m_uiAnchor;
		sf::Sprite* m_uiBackground;
		sf::Text* m_uiTitle;
		sf::Sprite* m_uiImage;

		sf::Clock m_activeTimer;

		std::optional<Data> getNext();
		void setCurrentAlert(Data& alert);

		bool shouldShowAlert();
		bool readyForNextAlert();

		sf::Vector2f calculateDisplayPosition();

		const sf::Time ALERT_VISIBLE_DURATION = sf::seconds(3);
		const sf::Time ALERT_GAP = sf::seconds(0.5f);

		const sf::Vector2f ALERT_POS_ACTIVE = { 0,  0 };
		const sf::Vector2f ALERT_POS_INACTIVE = { -512,  0 };
		const sf::Vector2f ALERT_POS_PADDING = { 16, 16 };

		const sf::Vector2f ALERT_OFFSET_IMAGE = { 16, 16 };
		const sf::Vector2f ALERT_OFFSET_TITLE = { 56, 22 };
	};
}