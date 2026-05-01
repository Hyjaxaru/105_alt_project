#include "AssetManager.h"

// --- Texture Management --- //

sf::Texture* AssetManager::loadTexture(sf::Texture tex, const std::string& texName)
{
	m_textureIndex.insert({ texName, tex });
	auto texture = getTexture(texName);

	LOG_INFO_NOLINE("Loaded new texture as '" + texName + '\'');

	return texture;
}

sf::Texture* AssetManager::loadTexture(const std::string& texPath, const std::string& texName)
{
	sf::Texture newTex;
	if (!newTex.loadFromFile(texPath))
	{
		LOG_ERROR_NOLINE("Could not load '" + texPath + '\'');
		return nullptr;
	}

	m_textureIndex.insert({ texName, newTex });
	auto texture = getTexture(texName);

	LOG_INFO_NOLINE("Loaded texture from '" + texPath + "' as '" + texName + '\'');

	return texture;
}

sf::Texture AssetManager::createTextureFromArea(sf::Texture& source, sf::IntRect area)
{
	sf::Texture newTex;
	sf::Image image = source.copyToImage();
	auto success = newTex.loadFromImage(image, false, area);

	if (!success)
		LOG_WARN_NOLINE("Failed to create texture from texture area");

	return newTex;
}

sf::Texture* AssetManager::createTextureFromArea(const std::string& sourceName, const std::string& newName, sf::IntRect area)
{
	auto& source = *getTexture(sourceName);
	auto newTex = createTextureFromArea(source, area);
	return loadTexture(newTex, newName);
}

sf::Texture* AssetManager::getTexture(const std::string& texName)
{
	if (resourceLoaded(m_textureIndex, texName))
		return &m_textureIndex.at(texName);
	return nullptr;
}

void AssetManager::removeTexture(const std::string& texName)
{
	m_textureIndex.erase(texName);
}

void AssetManager::clearTextureIndex()
{
	m_textureIndex.clear();
}

// --- Font Management --- //

sf::Font* AssetManager::loadFont(const std::string& fontPath, const std::string& fontName, int options)
{
	// create and load the font
	sf::Font newFont;
	if (!newFont.openFromFile(fontPath))
	{
		LOG_ERROR_NOLINE("Could not load '" + fontPath + '\'');
		return nullptr;
	}

	// add it to the font index
	m_fontIndex.insert({ fontName, newFont });
	auto font = getFont(fontName);

	// if there is no default font, make this the default... by default?
	if (m_defaultFont == nullptr)
		m_defaultFont = font;

	// set default fonts explicitly
	switch (options)
	{
	case LoadOptions::DEFAULT:
		// Load font as default
		m_defaultFont = font;
		LOG_INFO_NOLINE("Loaded default font '" + fontPath + "' as '" + fontName + '\'');
		break;
	case LoadOptions::DEFAULT | LoadOptions::DYSLEXIA:
		// Load font as dyslexia default
		m_defaultDyslexiaFont = font;
		LOG_INFO_NOLINE("Loaded default dyslexia font '" + fontPath + "' as '" + fontName + '\'');
		break;
	default:
		// font loaded normally
		LOG_INFO_NOLINE("Loaded font '" + fontPath + "' as '" + fontName + '\'');
		break;
	}

	return font;
}

void AssetManager::setDyslexiaMode(bool state) {
	m_useDyslexiaDefault = state;
}

sf::Font* AssetManager::getDefaultFont()
{
	if (m_useDyslexiaDefault && m_defaultDyslexiaFont != nullptr)
		return m_defaultDyslexiaFont;
	return m_defaultFont;
}

sf::Font* AssetManager::getFont(const std::string& fontName)
{
	if (resourceLoaded(m_fontIndex, fontName))
		return &m_fontIndex.at(fontName);
	return nullptr;
}

void AssetManager::removeFont(const std::string& fontName)
{
	m_fontIndex.erase(fontName);
}

void AssetManager::clearFontIndex()
{
	m_fontIndex.clear();
}


// --- Text Styles --- //

TextStyle* AssetManager::loadTextStyle(TextStyle style, const std::string& styleName, int options)
{
	m_textStyleIndex.insert({ styleName, style });
	auto* styleRef = getTextStyle(styleName);

	switch (options)
	{
	case DEFAULT:
		// load text style as default
		m_defaultTextStyle = styleRef;
		LOG_INFO_NOLINE("Loaded default text style " + styleName + '\'');
	}

	return styleRef;
}

TextStyle* AssetManager::getDefaultTextStyle()
{
	if (m_defaultTextStyle != nullptr)
		return m_defaultTextStyle;
	return nullptr;
}

TextStyle* AssetManager::getTextStyle(const std::string& styleName)
{
	if (resourceLoaded(m_textStyleIndex, styleName))
		return &m_textStyleIndex.at(styleName);
	return nullptr;
}

void AssetManager::styleText(sf::Text& text, TextStyle style)
{
	text.setCharacterSize(style.charSize);
	text.setFillColor(style.fillColor);
	text.setOutlineThickness(style.outlineSize);
	text.setOutlineColor(style.outlineColor);
}

void AssetManager::styleText(sf::Text& text, const std::string& styleName)
{
	auto style = getTextStyle(styleName);
	styleText(text, *style);
}


TextStyle* AssetManager::createTextStyle(const std::string& styleName, unsigned int charSize, sf::Color charColor, float outlineSize, sf::Color outlineColor)
{
	auto style = TextStyle({ charSize, charColor, outlineSize, outlineColor });
	return loadTextStyle(style, styleName);
}

void AssetManager::removeTextStyle(const std::string& styleName)
{
	m_textStyleIndex.erase(styleName);
}

void AssetManager::clearTextStyleIndex()
{
	m_textStyleIndex.clear();
}