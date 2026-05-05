#pragma once

#include <cmath>

// quick way to get SMFL in
#include "Framework/GameObject.h"

namespace vm {
	// Get Angle
	inline sf::Angle calculatePointAngle(sf::Vector2f origin, sf::Vector2f target)
	{
		auto diff = target - origin;
		return sf::radians(std::atan2(diff.y, diff.x));
	}

	inline sf::Angle calculatePointAngle(sf::Vector2f target)
	{
		return calculatePointAngle({ 0,0 }, target);
	}


	// Get Distance
	inline float calculatePointDistance(sf::Vector2f origin, sf::Vector2f target)
	{
		auto diff = target - origin;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}

	inline float calculatePointDistance(sf::Vector2f target)
	{
		return calculatePointDistance({0,0}, target);
	}

	// Get Vector
	inline sf::Vector2f calculateVector(sf::Vector2f origin, sf::Angle angle, float length) {
		auto radians = angle.asRadians();
		return sf::Vector2f{
			origin.x + length * std::cos(radians),
			origin.y + length * std::sin(radians)
		};
	}
};
