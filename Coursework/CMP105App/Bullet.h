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

	bool isExpired() const { return expireTimer >= 0; }

private:
	int expireTimer;


	const float SPEED = 1.f;
	const int LIFETIME_FRAMES = 600; // 10 seconds @ 60FPS
};

