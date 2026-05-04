#pragma once

#include <iostream>
#include <string>

#include "Framework/GameObject.h"
#include "Framework/Animation.h"

#include "AssetManager.h"

class Goal :
	public GameObject
{
public:
	Goal();

	void update(float dt) override;
private:
	Animation m_animation;
};

