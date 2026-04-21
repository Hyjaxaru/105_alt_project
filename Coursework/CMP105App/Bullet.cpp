#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f dir)
{
	// set up the underlying GameObject
	auto& assets = AssetManager::Instance();
	setTexture(assets.getTexture("bullet"));
	setSize({ 32, 32 });
	setOrigin({ 16, 16 });
	setScale({ 1, 1 });
	
	// set position and velocity
	setPosition(position);
	m_velocity = dir * BULLET_SPEED;
}

void Bullet::update(float dt)
{
	move(m_velocity);
}