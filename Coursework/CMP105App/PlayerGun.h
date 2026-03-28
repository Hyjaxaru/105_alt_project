#pragma once

#include <cmath>

#include "Framework/GameObject.h"

#include "AssetManager.h"
#include "VectorMath.h"

class PlayerGun :
    public GameObject
{
public:
    PlayerGun();
    ~PlayerGun();
    
    sf::Vector2f fireGunWithRecoil();
    
    void setPositionFromReference(sf::Vector2f origin, sf::Vector2f target);

private:
    const float PI_ESTIMATION = 3.141592654f;
    const float MAX_DIST_FROM_REFERENCE = 64.f;
    const float RECOIL_FORCE = 50.f;
};

