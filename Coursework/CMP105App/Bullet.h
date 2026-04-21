#pragma once

#include "Framework/GameObject.h"

#include "Logger.h"
#include "AssetManager.h"

class Bullet :
	public GameObject
{
public:
	Bullet(sf::Vector2f position, sf::Vector2f dir);

	void update(float dt) override;

private:
	const float BULLET_SPEED = 1.f;
};

