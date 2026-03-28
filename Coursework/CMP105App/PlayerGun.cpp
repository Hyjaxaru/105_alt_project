#include "PlayerGun.h"

PlayerGun::PlayerGun()
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

void PlayerGun::setPositionFromReference(sf::Vector2f origin, sf::Vector2f target)
{
	// calculate the angle from the reference position to the target position
	auto angle = vm::calculatePointAngle(origin, target).asRadians();

	// calculate the position the gun should be placed at
	auto dist = vm::calculatePointDistance(origin, target);
	auto distC = std::clamp(dist, 0.f, MAX_DIST_FROM_REFERENCE);
	auto pos = sf::Vector2f{
		origin.x + distC * std::sin(angle),
		origin.y + distC * std::cos(angle)
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
	auto rotation = getRotation().asRadians();
	auto angle = rotation - PI_ESTIMATION;
	return vm::calculateVector(sf::Vector2f(), sf::radians(angle), RECOIL_FORCE);
}