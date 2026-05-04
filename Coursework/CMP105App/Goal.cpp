#include "Goal.h"

Goal::Goal()
{
	setTexture(AssetManager::Instance().getTexture(AssetManager::Textures::TERRAIN));
	setSize({ 72, 72 });
	setCollisionBox({ { 0, 0 }, { 72, 72 } });

	// we are using the flag from the terrain texture sheet,
	// which we know exactly where that will be
	int sheetRow     =  5;
	int sheetColumn1 = 11;
	int sheetColumn2 = 12;
	int tileSize     = 18;
	int sheetSpacing =  1;

	// add the frames to the animation
	m_animation.addFrame({
		{ (tileSize + sheetSpacing) * sheetColumn1, (tileSize + sheetSpacing) * sheetRow },
		{ tileSize, tileSize }
	});
	m_animation.addFrame({
		{ (tileSize + sheetSpacing) * sheetColumn2, (tileSize + sheetSpacing) * sheetRow },
		{ tileSize, tileSize }
	});

	m_animation.setFrameSpeed(1.f);
}

void Goal::update(float dt)
{
	m_animation.animate(dt);
	setTextureRect(m_animation.getCurrentFrame());
}