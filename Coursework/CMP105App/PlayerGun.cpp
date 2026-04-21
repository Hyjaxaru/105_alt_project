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
	pointAtTarget();
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
	// TODO: Fire the gun

	// claculate the angle to fire
	auto dir = getPosition() - *m_origin;
	auto norm = dir.normalized();

	// fire the gun

	// calculate the recoil of the gun (in the opposite direction to fired)
	auto force = -norm * RECOIL_FORCE;
	
	// log
	std::stringstream logstream;
	logstream << "Gun Fired, Force applied - X: " << force.x << ", Y: " << force.y << ", Angle: " << VMath::calculatePointAngle(dir).asDegrees();
	LOG_DEBUG(logstream.str());

	// return the recoil force
	return force;
}