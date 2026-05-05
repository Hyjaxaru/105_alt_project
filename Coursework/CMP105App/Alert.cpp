#include "Alert.h"

using namespace Alert;

Manager::Manager(sf::RenderWindow& window, AudioManager& audio) :
	m_window(window), m_audio(audio)
{
	m_uiBackground = new sf::Sprite(*m_assets.getTexture("AlertBG"));
	m_uiBackground->setPosition(ALERT_POS_INACTIVE);

	m_uiImage = new sf::Sprite(*m_assets.getTexture("AlertImage-Placeholder"));

	m_uiTitle = new sf::Text(*m_assets.getDefaultFont());
	m_uiTitle->setCharacterSize(16);
	m_uiTitle->setFillColor(sf::Color::White);
	
	LOG_DEBUG("Ready!")
}

void Manager::update(float dt)
{
	// if there isn't an active alert, go looking for one
	if (m_current == nullptr)
	{
		auto attempt = getNext();
		if (attempt.has_value())
		{
			setCurrentAlert(attempt.value());
			return;
		}
	}

	auto showAlert = shouldShowAlert();
	if (readyForNextAlert()) m_current = nullptr;

	// move the things
	auto anchor = calculateDisplayPosition();
	m_uiBackground->setPosition(anchor);
	m_uiImage->setPosition(anchor + ALERT_OFFSET_IMAGE);
	m_uiTitle->setPosition(anchor + ALERT_OFFSET_TITLE);
}

void Manager::render()
{
	m_window.draw(*m_uiBackground);
	m_window.draw(*m_uiImage);
	m_window.draw(*m_uiTitle);
}

std::optional<Alert::Data> Manager::getNext()
{
	auto& queue = m_queue.getQueue();
	if (queue.empty()) return {};

	Data item = queue.front();
	queue.pop();
	return item;
}

void Manager::setCurrentAlert(Data& alert)
{
	m_current = std::move(&alert);
	m_activeTimer.restart();
	LOG_DEBUG("Set current alert");

	m_uiTitle->setString(m_current->title);
	m_uiImage->setTexture(m_current->image);
}

bool Manager::shouldShowAlert()
{
	if (m_current == nullptr) return false;
	return m_activeTimer.getElapsedTime().asMilliseconds() < ALERT_VISIBLE_DURATION.asMilliseconds();
}

bool Manager::readyForNextAlert()
{
	return m_activeTimer.getElapsedTime().asMilliseconds() > ALERT_VISIBLE_DURATION.asMilliseconds() + ALERT_GAP.asMilliseconds();
}

sf::Vector2f Manager::calculateDisplayPosition()
{
	m_uiAnchor = vm::lerp(m_uiAnchor, shouldShowAlert() ? ALERT_POS_ACTIVE : ALERT_POS_INACTIVE, 0.2f);

	auto& view = m_window.getView();
	auto size = view.getSize();
	auto center = view.getCenter();
	return center - sf::Vector2f{ size.x * 0.5f, size.y * 0.5f } + m_uiAnchor + ALERT_POS_PADDING;
}