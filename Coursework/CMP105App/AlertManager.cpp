#include "AlertManager.h"

using namespace alert;

AlertManager::AlertManager(sf::RenderWindow& window, AudioManager& audio) :
	m_window(window), m_audio(audio)
{

}

void AlertManager::update(float dt)
{
	// if there isn't an active alert, go looking for one
	if (m_current == nullptr)
	{
		auto attempt = getNext();
		if (attempt.has_value())
		{
			m_current = std::move(&attempt.value());	// move the next alert into the current pointer 
			m_activeTimer.restart();					// restart the alert active timer 
			return;
		}
	}

	auto showAlert = shouldShowAlert();

	// move the thing
}

void AlertManager::render()
{

}

void AlertManager::addToQueue(const Alert& alert)
{
	m_queue.push(alert);
}

std::optional<Alert> AlertManager::getNext()
{
	if (m_queue.empty()) return {};

	Alert item = m_queue.front();
	m_queue.pop();
	return item;
}