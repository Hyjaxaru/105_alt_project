#pragma once

#include <cmath>

// quick way to get SMFL in
#include "Framework/GameObject.h"

namespace vm {
	// Get the angle from one point to another
	// @param origin: The origin point
	// @param target: The target point
	// @returns The angle to the target from the origin
	inline sf::Angle calculatePointAngle(const sf::Vector2f& origin, const sf::Vector2f& target)
	{
		auto diff = target - origin;
		return sf::radians(std::atan2(diff.y, diff.x));
	}

	// Get the angle to the target from origin
	// @param target: The target point
	// @returns The angle to the target from (0, 0)
	inline sf::Angle calculatePointAngle(sf::Vector2f target)
	{
		return calculatePointAngle({ 0,0 }, target);
	}

	// Get Vector
	inline sf::Vector2f calculateVector(const sf::Vector2f& origin, const sf::Angle& angle, float length) {
		auto radians = angle.asRadians();
		return sf::Vector2f{
			origin.x + length * std::cos(radians),
			origin.y + length * std::sin(radians)
		};
	}

	inline float lerp(const float& a, const float& b, const float& t)
	{
		return a + t * (b - a);
	}

	// lerp
	inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float& t)
	{
		return { lerp(a.x, b.x, t), lerp(a.y, b.y, t) };
	}
};
