#pragma once

#include <cmath>

// quick way to get SMFL in
#include "Framework/GameObject.h"

// namespace VectorMath
namespace vm {
	sf::Angle calculatePointAngle(sf::Vector2f origin, sf::Vector2f target)
	{
		auto diff = target - origin;
		return sf::radians(std::atan2(diff.x, diff.y));
	}

	float calculatePointDistance(sf::Vector2f origin, sf::Vector2f target)
	{
		auto diff = target - origin;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}

	sf::Vector2f calculateVector(sf::Vector2f origin, sf::Angle angle, float length) {
		auto radians = angle.asRadians();
		return sf::Vector2f{
			origin.x + length * std::sin(radians),
			origin.y + length * std::cos(radians)
		};
	}
}