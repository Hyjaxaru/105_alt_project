#pragma once

#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Framework/AudioManager.h"

#include "AssetManager.h"

class Enemy :
	public GameObject
{
public:
	Enemy();

	void update(float dt);

	void collisionResponse(GameObject& collider) override;
	void setAudio(AudioManager* audio) { m_audio = audio; };
	void reset();

	void setSpawnPosition(sf::Vector2f pos) { m_spawnLoc = pos; }

private:
	Animation* m_currAnim;
	Animation m_idle;
	Animation m_walk;
	sf::Vector2f m_accel;
	bool m_isGrounded;

	bool m_moveRtl = false;
	int m_movePause = 0;

	AudioManager* m_audio;

	sf::Vector2f m_spawnLoc;

	const float SPEED = 5.0f;
	const float GRAVITY = 50.0f;
	const int MOVE_PAUSE_DURATION = 120; // 2 seconds @ 60FPS
};

