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
	auto angle = VMath::calculatePointAngle(*m_origin, *m_target).asRadians();

	// calculate the position the gun should be placed at
	auto dist = VMath::calculatePointDistance(*m_origin, *m_target);
	auto distC = std::clamp(dist, 0.f, MAX_DIST_FROM_REFERENCE);
	auto pos = sf::Vector2f{
		m_origin->x + distC * std::sin(angle),
		m_origin->y + distC * std::cos(angle)
	};

	// set transform
	setPosition(pos);

	auto isFacingBack = angle < 0.f;

	auto rotOffset = isFacingBack ? PI_ESTIMATION * 1.5f : PI_ESTIMATION * 0.5f;
	auto rotation = sf::radians(-angle + rotOffset);
	setRotation(rotation);

	setScale(sf::Vector2f{ isFacingBack ? -1.f : 1.f, 1.f });
}

sf::Vector2f PlayerGun::fireGunWithRecoil() {
	// TODO: Fire the gun

	// claculate and return the recoil force
	/*auto rotation = getRotation().asRadians();
	auto angle = rotation - PI_ESTIMATION;
	auto force = VMath::calculateVector(sf::Vector2f(), sf::radians(angle), RECOIL_FORCE);
	std::cout << "X: " << force.x << ", Y: " << force.y << ", Angle: " << sf::radians(angle).asDegrees() << std::endl;
	return force;*/

	// calculate force but the better way
	auto dir = getPosition() - *m_origin;

	Logger::Debug("X: " + dir.x + ", Y: " + dir.y);
	LOG_DEBUG("TEST")
	return sf::Vector2f();
}