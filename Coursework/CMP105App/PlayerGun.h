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
    PlayerGun();
    PlayerGun(sf::Vector2f* m_origin, sf::Vector2f* m_target);
    ~PlayerGun();

    void update(float dt) override;
    
    sf::Vector2f fireGunWithRecoil();
    
    //void pointAt(sf::Vector2f origin, sf::Vector2f target);
    void pointAtTarget();

private:
    sf::Vector2f* m_origin;
    sf::Vector2f* m_target;

    // --- constants --- //
    const float PI_ESTIMATION = 3.141592654f;
    const float MAX_DIST_FROM_REFERENCE = 64.f;
    const float RECOIL_FORCE = 25.f;
};

