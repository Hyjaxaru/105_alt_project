#include "PlayerGun.h"

PlayerGun::PlayerGun(sf::Vector2f* origin, sf::Vector2f* target)
	: m_origin(origin), m_target(target)
{
	auto& assets = AssetManager::Instance();
	setTexture(assets.getTexture("gun"));
	setSize({ 32, 32 });
	setOrigin({ 16, 16 });
	setScale({ -1, 1 });
}

PlayerGun::~PlayerGun()
{
}

void PlayerGun::update(float dt)
{
	// transform the gun
	pointAtTarget();

	// update all the bullets
	for (auto* bullet : m_bullets)
		bullet->update(dt);
}

void PlayerGun::render(sf::RenderWindow* window)
{
	window->draw(*this);

	// render all of the bullets in the bullet index
	for (auto* bullet : m_bullets)
		window->draw(*bullet);
}

void PlayerGun::pointAtTarget()
{
	// calculate the angle from the reference position to the target position
	auto angle = VMath::calculatePointAngle(*m_origin, *m_target);

	// calculate the position the gun should be placed at
	auto dist = VMath::calculatePointDistance(*m_origin, *m_target);
	auto distC = std::clamp(dist, 0.f, MAX_DIST_FROM_REFERENCE);
	auto pos = VMath::calculateVector(*m_origin, angle, distC);

	// set position
	setPosition(pos);

	// rotate the gun in the direction of fire
	auto isFacingBack = angle.asRadians() < 0.f;
	auto rotOffset = sf::radians(isFacingBack ? PI_ESTIMATION * 1.5f : PI_ESTIMATION * 0.5f);
	auto rotation = -angle + rotOffset;
	setRotation(rotation);

	// if needed, flip the sprite so the gun always looks correctly held
	setScale({ isFacingBack ? -1.f : 1.f, 1.f });
}

sf::Vector2f PlayerGun::fireGunWithRecoil() {
	// claculate the angle to fire
	auto pos = getPosition();
	auto dir = pos - *m_origin;
	auto norm = dir.normalized();

	// create the bullet in the bullet list
	m_bullets.push_back(new Bullet(pos, norm));

	// calculate and return the recoil of the gun (in the opposite direction to fired)
	return -norm * RECOIL_FORCE;
}