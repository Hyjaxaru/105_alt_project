#pragma once

#include <cmath>
#include <vector>

#include "Framework/GameObject.h"
#include "Framework/Collision.h"

#include "Logger.h"
#include "AssetManager.h"
#include "VectorMath.h"
#include "Bullet.h"

class PlayerGun :
    public GameObject
{
public:
    PlayerGun();
    ~PlayerGun();

    void update(float dt) override;
    void render(sf::RenderWindow* window);
    void reset();

    sf::Vector2f fireGunWithRecoil();
    
    void pointAtTarget(sf::Vector2f origin, sf::Vector2f target);
    void setRotationCenter(sf::Vector2f& newValue) { m_origin = newValue; }
    void setTarget(sf::Vector2f& newValue) { m_target = newValue; }
    void setAmmoMax(const int& ammo) { m_ammoMax = ammo; }
    void setAudio(AudioManager* audio) { m_audio = audio; };

    std::vector<Bullet*>* getBullets() { return &m_bullets; }

private:
    sf::Vector2f m_origin;
    sf::Vector2f m_target;

    int m_ammoMax;
    int m_ammo;

    std::vector<Bullet*> m_bullets;

    AudioManager* m_audio;

    // --- constants --- //
    const float PI_ESTIMATION = 3.141592654f;
    const float MAX_DIST_FROM_REFERENCE = 64.f;
    const float RECOIL_FORCE = 20.f;
};

