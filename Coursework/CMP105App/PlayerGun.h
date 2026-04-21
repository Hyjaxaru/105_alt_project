#pragma once

#include <cmath>

#include "Framework/GameObject.h"

#include "Logger.h"
#include "AssetManager.h"
#include "VectorMath.h"

class PlayerGun :
    public GameObject
{
public:
    PlayerGun(sf::Vector2f* m_origin, sf::Vector2f* m_target);
    ~PlayerGun();

    void update(float dt) override;
    
    sf::Vector2f fireGunWithRecoil();
    
    void pointAtTarget();
    void setRotationCenter(sf::Vector2f* newValue) { m_origin = newValue; }
    void setTarget(sf::Vector2f* newValue) { m_target = newValue; }

private:
    sf::Vector2f* m_origin;
    sf::Vector2f* m_target;

    // --- constants --- //
    const float PI_ESTIMATION = 3.141592654f;
    const float MAX_DIST_FROM_REFERENCE = 64.f;
    const float RECOIL_FORCE = 20.f;
};

