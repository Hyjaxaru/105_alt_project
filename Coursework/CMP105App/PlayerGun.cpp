#include "PlayerGun.h"

PlayerGun::PlayerGun()
{
	auto& assets = AssetManager::Instance();
	setTexture(assets.getTexture(AssetManager::Textures::WEAPON));
	setSize({ 32, 32 });
	setOrigin({ 16, 16 });
	setScale({ -1, 1 });
}

PlayerGun::~PlayerGun()
{
}

void PlayerGun::update(float dt)
{
}

void PlayerGun::render(sf::RenderWindow* window)
{
}

void PlayerGun::pointAtTarget(sf::Vector2f origin, sf::Vector2f target)
{
	// set the stored values so recoil can still be calcuated
	m_origin = origin; m_target = target;

	// calculate the angle from the reference position to the target position
	auto angle = VMath::calculatePointAngle(m_origin, m_target);

	// calculate the position the gun should be placed at
	auto dist = VMath::calculatePointDistance(m_origin, m_target);
	auto distC = std::clamp(dist, 0.f, MAX_DIST_FROM_REFERENCE);
	auto pos = VMath::calculateVector(m_origin, angle, distC);

	setPosition(pos);
	setRotation(angle);

	// if needed, flip the sprite so the gun always looks correctly held
	auto isFacingBack = abs(angle.asRadians()) > 1.5f;
	setScale({ 1.f, isFacingBack ? -1.f : 1.f });
}

sf::Vector2f PlayerGun::fireGunWithRecoil() {
	// if there is no ammo, we can't shoot!
	if (m_ammo <= 0)
	{
		// Todo: sound!
		return {};
	}
	m_ammo--;

	// claculate the angle to fire
	auto pos = getPosition();
	auto dir = pos - m_origin;
	if (dir.lengthSquared() <= 0)
	{
		std::stringstream s;
		s << dir.length();
		LOG_WARN("Calculated direction <= 0: " + s.str())
		return {};
	}

	auto norm = dir.normalized();

	// create the bullet in the bullet list
	m_bullets.push_back(new Bullet(pos, norm));

	// calculate and return the recoil of the gun (in the opposite direction to fired)
	return -norm * RECOIL_FORCE;
}