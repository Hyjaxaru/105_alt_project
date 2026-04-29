#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f dir)
{
	// set up the underlying GameObject
	auto& assets = AssetManager::Instance();
	setTexture(assets.getTexture(AssetManager::Textures::PROJECTILE));
	setSize({ 32, 32 });
	setOrigin({ 16, 16 });
	setScale({ 0.5, 0.5 });
	
	// set position and velocity
	setPosition(position);
	m_velocity = dir * SPEED;

	// set expiry timer
	expireTimer = LIFETIME_FRAMES;
}

void Bullet::update(float dt)
{
	// move the gun
	move(m_velocity);

	// count down to bullet expiration
	if (expireTimer > 0)
		expireTimer--;
}