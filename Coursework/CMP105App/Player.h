#pragma once

#include <iostream>
#include <string>

#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Framework/AudioManager.h"

#include "AssetManager.h"
#include "PlayerGun.h"

class Player :
    public GameObject
{
public:
    Player();

    void handleInput(float dt) override;
    void update(float dt) override;
    void render();

    void collisionResponse(GameObject& collider) override;
    void setEdges(float left, float right) { m_leftEdge = left; m_rightEdge = right; };
    void setLeverPosition(sf::Vector2f leverPos) { m_leverPosition = leverPos; };
    void setEndGamePosition(sf::Vector2f endPos) { m_endPosition = endPos; };
    bool inLeverRange() { return false; };
    bool inEndRange() { return false; };
    bool getLeverPulled() { return m_leverPulled; };
    bool getGameEndTriggered() { return m_gameEndTriggered; };
    void reset();
    void setCanDoubleJump(bool value) { m_canDoubleJump = value; };
    bool canDoubleJump() { return m_canDoubleJump; };
    void setAudio(AudioManager* audio) { m_audio = audio; };

private:
    sf::Texture m_dinoTexture;
    Animation* m_currAnim;
    Animation m_idle;
    Animation m_walk;
    Animation m_sprint;
    sf::Vector2f m_accel;
    float m_sprintTimer = 0.f;
    bool m_isGrounded;
    float m_leftEdge;
    float m_rightEdge;
    sf::Vector2f m_leverPosition;
    sf::Vector2f m_endPosition;
    bool m_leverPulled = false;
    bool m_gameEndTriggered = false;
    bool m_canDoubleJump;
    bool m_hasDoubleJumped;
    AudioManager* m_audio;

    PlayerGun m_gun;

    const float SPRINT_COOLDOWN = 2.0f;
    const float SPRINT_SPEED_MULT = 2.5f;
    const float SPEED = 10.0f;
    const float GRAVITY = 50.0f;
    const float COEFF_OF_REST = 0.8f;
    const float DRAG_FACTOR = 0.9f;
    const float AIR_DRAG_FACTOR = 0.99f;
    const float TURN_DRAG = 0.6f;       // allow snappy decel for turning
    const float JUMP_FORCE = 20.0f;
    const float SPRINT_ANIM_THRESHOLD = 1.2f * SPEED;
    const float ACTIVATE_RANGE_SQUARED = 700.0f;

    const float GUN_DISTANCE = 20.f;

};

