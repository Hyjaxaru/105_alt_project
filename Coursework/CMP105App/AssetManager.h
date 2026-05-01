//                                                                                
//     _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____ 
//    |  _  |   __|   __|   __|_   _|  |     |  _  |   | |  _  |   __|   __| __  |
//    |     |__   |__   |   __| | |    | | | |     | | | |     |  |  |   __|    -|
//    |__|__|_____|_____|_____| |_|    |_|_|_|__|__|_|___|__|__|_____|_____|__|__|
//                                                                                
//    CMP105 Solo Project Edition
//    Noah Albrock

#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>
#include "Framework/AudioManager.h"
#include "Framework/TileMap.h"

#include "Logger.h"

// sf::Text styling options
struct TextStyle {
	unsigned int charSize;
	sf::Color fillColor;
	float outlineSize;
	sf::Color outlineColor;
};

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() {}

	// --- Singleton Instance --- //

	// I understand how singletons and instances work. I use them a lot in Swift!
	// However, this appears to be the recommended implementation in C++, that trys to stop extra instances from appearing.
	// Source: https://andreasfertig.com/blog/2026/01/singleton-done-right-in-cpp/

	AssetManager(const AssetManager&) = delete;
	AssetManager(AssetManager&&) = delete;

	AssetManager& operator = (const AssetManager&) = delete;
	AssetManager& operator = (AssetManager&&) = delete;

	static AssetManager& Instance()
	{
		static AssetManager instance;
		return instance;
	}


	// File loading options. Tells AssetManager how you want assets loaded into the different indexes
	enum LoadOptions { DEFAULT = 1, DYSLEXIA = 2 };


	// --- Textures --- //

	
	// instead of an enum class, we use a normal class with an unscoped enum.
	// this allows us to have methods that appear to be in the enum
	// source: https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods
	
	class Stringable {
	public:
		virtual enum Value;

		template <typename T>
		std::string enumToString(std::map<T, std::string>& map, T key) {
			auto pair = map.find(key);
			return (pair != map.end()) ? pair->second : "UNKNOWN";
		}
	};

	// Default index entries
	class Textures: Stringable {
	public:
		enum Value { TERRAIN, BACKGROUND, PLAYER, WEAPON, PROJECTILE };

		Textures() = default;
		constexpr Textures(Value name) : m_value(name) {}

		std::string toString() const { return enumToString(m_stringIndex, m_value); };
		std::string toIndexValue() const { return "DEFAULT_" + toString(); };
	private:
		Value m_value;

		std::map<Value, std::string> m_stringIndex = {
			{ TERRAIN,    "TERRAIN" },
			{ BACKGROUND, "BACKGROUND" },
			{ PLAYER,     "PLAYER" },
			{ WEAPON,     "WEAPON" },
			{ PROJECTILE, "PROJECTILE" }
		};
	};

	// Load a textrue into the texture index
	// @param tex:     The texture to load into the index
	// @param texName: The name of the texture we are loading
	// @returns        A reference to the loaded texture in the font index
	sf::Texture* loadTexture(sf::Texture tex, const std::string& texName);

	// Load a textrue into the texture index from a file
	// @param texPath: The path of the texture file
	// @param texName: The name of the texture we are loading
	// @returns        A reference to the loaded texture in the font index
	sf::Texture* loadTexture(const std::string& texPath, const std::string& texName);

	// Load a textrue into the texture index from a file
	// @param texPath:     The path of the texture file
	// @param defaultName: The default index value
	// @returns            A reference to the loaded texture in the font index
	sf::Texture* loadTexture(const std::string& texPath, Textures defaultName) { return loadTexture(texPath, defaultName.toIndexValue()); };

	// Create a new texture from a portion of another
	// @param source: The source texture
	// @param area:   The area of the source texture to extract
	sf::Texture createTextureFromArea(sf::Texture& source, sf::IntRect area);

	// Create a new texture from a portion of another in the texture index
	// @param sourceName: The name of the source texture in the texture index
	// @param newName:	  The desired name for the new texture in the texture index
	// @param area:       The area of the source texture to extract
	sf::Texture* createTextureFromArea(const std::string& sourceName, const std::string& newName, sf::IntRect area);

	// Gets a loaded texture by it's name. Error if no font is found
	// @param texName: The name of the texture to get
	// @returns        A reference to the named texture in the texture index
	sf::Texture* getTexture(const std::string& texName);

	// Gets a loaded texture by it's name. Error if no font is found
	// @param defaultName: The name of the texture to get
	// @returns        A reference to the named texture in the texture index
	sf::Texture* getTexture(Textures defaultName);

	// Removes a texture from the index by it's name
	// @param texName: The name of the texture to remove
	void removeTexture(const std::string& texName);


	// --- Fonts --- //

	// Font loading options. See `UI::UIManager::LoadOptions`
	using FontOptions = LoadOptions;

	// Load a font into the font index. Will become the default font if one is not set
	// @param fontPath: The path to the font file
	// @param fontName: The name of the font we are loading
	// @param options:  Loading options. See `UI::UIManager::FontOptions`
	// @returns         A reference to the loaded font in the font index
	sf::Font* loadFont(const std::string& fontPath, const std::string& fontName, int options = 0);

	// Gets the default font for this UIManager instance
	// @returns The current default font. Will return dyslexia varient if enabled and possible. Returns nullptr if none is set
	sf::Font* getDefaultFont();

	// Gets a loaded font by it's name. Error if no font is found
	// @param fontName: The name of the font to get.
	// @returns         A reference to the named font in the font index.
	sf::Font* getFont(const std::string& fontName);

	// Removes a font from the font index
	// @param fontName: The name of the font we want to remove
	void removeFont(const std::string& fontName);

	// Set dyslexia mode
	// @param state: new state
	void setDyslexiaMode(bool state);

	// Enable dyslexia mode
	inline void enableDyslexiaMode() { setDyslexiaMode(true); }
	// Disable dislexia mode
	inline void disableDislexiaMode() { setDyslexiaMode(false); }


	// --- Text Styles --- //

	// Text style loading options. See `UI::UIManager::LoadOptions`
	using TextStyleOptions = LoadOptions;

	// Load a text style into the text style index
	// @param style:     The text style to be loaded
	// @param styleName: The name of the text style we are loading
	// @param options:   Loading options. See `UI::UIManager::TextStyleOptions`
	// @returns          A reference to the loaded text style in the test style index
	TextStyle* loadTextStyle(TextStyle style, const std::string& styleName, int options = 0);

	// Gets the default text style
	// @returns A reference to the default text style. Returns nullptr if none is set
	TextStyle* getDefaultTextStyle();

	// Gets a text style in index
	// @param styleName: The name of the text style in the index
	// @returns          A reference to the loaded text style in the text style index
	TextStyle* getTextStyle(const std::string& styleName);

	// Styles the given text
	// @param text:  The text element you want styled
	// @param style: The text style to style the text with
	void styleText(sf::Text& text, TextStyle style);

	// Styles the given text using the text style index
	// @param text:      The text element you want styled
	// @param styleName: The name of the text style in the index
	void styleText(sf::Text& text, const std::string& styleName);

	// Creates a text style and loads it into the index
	// @param styleName:     The name of the text style we are loading
	// @param charSize:      The size of the text characters
	// @param charColor:     The fill colour for the text
	// @param outlineSize:   The size of the outline around the text characters
	// @param outlineColour: The colour of the outline
	// @returns              A reference to the loaded text style in the index
	TextStyle* createTextStyle(const std::string& styleName, unsigned int charSize, sf::Color fillColor, float outlineSize, sf::Color outlineColor);

	// Removes a text style from the index
	// @param styleName: The name of the text style we want to remove
	void removeTextStyle(const std::string& styleName);


	// --- TileMaps --- //

	class TileMaps {
	public:
		enum Value { TERRAIN, BACKGROUND };

		TileMaps() = default;
		constexpr TileMaps(Value name) : m_value(name) {}

		std::string toString() const;
		std::string toIndexValue() const { return "DEFAULT_" + toString(); };
	private:
		Value m_value;

		std::map<Value, std::string> m_stringIndex = {
			{ TERRAIN,    "TERRAIN" },
			{ BACKGROUND, "BACKGROUND" }
		};
	};

	TileMap* loadTileMap(TileMap tm, std::string const tmName);

	TileMap* loadTileMap(TileMap tm, TileMaps defaultName);

	TileMap* getTileMap(std::string const tmName);

	TileMap* getTileMap(TileMaps defaultName);

private:
	// check if a value is loaded in an index by it's key
	template <typename T>
	inline bool resourceLoaded(std::map<std::string, T>& index, std::string const key)
	{
		return index.find(key) != index.end();
	}

	// --- Texture Management --- //

	// The texture index. Stores all the currently loaded textures
	std::map<std::string, sf::Texture> m_textureIndex;

	// Clears the texture index
	void clearTextureIndex();


	// --- Font Management --- //

	// The font index. Stores all the currently loaded fonts
	std::map<std::string, sf::Font> m_fontIndex;

	// The current default font
	sf::Font* m_defaultFont = nullptr;

	// The current default dyslexia font
	sf::Font* m_defaultDyslexiaFont = nullptr;

	// Should dyslexia font be on?
	bool m_useDyslexiaDefault = false;

	// Clears the font index
	void clearFontIndex();


	// --- Text Styles --- //

	// The text style index. Stores all the currently loaded text styles
	std::map<std::string, TextStyle> m_textStyleIndex;

	// The current default text style
	TextStyle* m_defaultTextStyle = nullptr;

	// The current default dyslexia text style
	TextStyle* m_defaultDyslexiaTextStyle = nullptr;

	// Clears the text style index
	void clearTextStyleIndex();

	// --- Audio --- //

	// AudioManager instance
	AudioManager m_audioManager = AudioManager();
};