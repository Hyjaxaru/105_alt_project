#include "Enemy.h"

Enemy::Enemy()
{
	setTexture(AssetManager::Instance().getTexture(AssetManager::Textures::ENEMY));
	setSize({ 72, 72 });
	
	for (int i = 0; i < 6; i++)
		m_walk.addFrame({ { i * 64, 0 }, { 64, 64 } });

	m_currAnim = &m_walk;
	m_walk.setFrameSpeed(1.f / 10.f);
	
	setCollisionBox({ { 8, 48 }, { 48, 16 } });

	m_isGrounded = false;
}

void Enemy::update(float dt)
{
	if (m_movePause > 0)
	{
		m_movePause--;
		m_velocity = { 0, 0 };
	}
	else
	{
		m_velocity = { m_moveRtl ? -SPEED : SPEED, 0 };
	}

	move(m_velocity);

	m_currAnim->animate(dt);
	setTextureRect(m_currAnim->getCurrentFrame());
}

void Enemy::collisionResponse(GameObject& collider)
{
	sf::FloatRect enemyCollider = getCollisionBox();
	sf::FloatRect wallBounds = collider.getCollisionBox();

	auto overlap = enemyCollider.findIntersection(wallBounds);
	if (!overlap) return; // if there is no overlap, then leave.

	if (overlap->size.x < overlap->size.y)
	{
		// taller than wide -> collision is side-on
		if (enemyCollider.position.x < wallBounds.position.x)
		{
			move({ -overlap->size.x, 0 });
			m_moveRtl = true;
		}
		else
		{
			move({ overlap->size.x, 0 });
			m_moveRtl = false;
		}

		m_movePause = MOVE_PAUSE_DURATION;
	}
}

void Enemy::reset()
{
	setPosition(m_spawnLoc);
	m_velocity = { 0, 0 };
}