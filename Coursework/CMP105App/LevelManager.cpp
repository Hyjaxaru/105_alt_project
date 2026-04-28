#include "LevelManager.h"

LevelManager::LevelManager(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio) :
	m_window(window), m_input(input), m_gameState(gameState), m_audio(audio)
{
}

void LevelManager::loadLevels(std::string dirPath)
{

}

void LevelManager::loadLevel(std::string filePath)
{
	LOG_INFO_NOLINE("Loading level '" + filePath + '\'')

	// get the level and ensure we can access it
	std::ifstream file(filePath);

	if (!file.good()) {
		LOG_ERROR_NOLINE(makeFileLoadFailErrorMessage(filePath, "File not found."));
		return;
	}

	// create the level creation struct that contains all the information
	// required for the creation process
	auto level = Level{
		file,	  // the file
		filePath, // the path to the level file
		filePath, // the name of the level (using path for now, overriden later)
		LevelTemplate(m_window, m_input, m_gameState, m_audio)
	};

	// decode all of the keywords 
	std::string keyword;
	bool validLevel = false, validVersion = false, validTerrain = false;

	while (file >> keyword)
	{
		// I tried to make switch statements work here
		// unfortunately there isn't enough time to fight with it

		// detect the level header
		// if found, fill in the level name
		// this should must occur only once. no LEVEL = invalid, more than one = ignored
		if (keyword == "LEVEL" && !validLevel)
		{
			file >> level.name;
			validLevel = true;
		}

		// detect the minimum version of the loader required to load the level
		// helps with versioning if this continues
		else if (keyword == "VERSION" && !validVersion)
			validVersion = loadVersion(level);

		// load the terrain tilemap
		else if (keyword == "TERRAIN")
			validTerrain = loadTilemap();
	}
}

bool LevelManager::loadVersion(Level& level)
{
	int version;
	level.file >> version;

	// fail if the file is too new
	if (version > LOADER_VERSION) {
		LOG_ERROR(makeFileLoadFailErrorMessage(level.path, "Newer loader version required."));
		return false;
	}

	// the version number is valid!
	return true;
}

bool LevelManager::loadTileset(Level& level, TileMapType type)
{
	// load in the texture path
	std::string path;
	int rows, columns, size, spacing, haveCollideLeft, haveCollideRight;
	level.file >> path >> rows >> columns >> size >> spacing >> haveCollideLeft >> haveCollideRight;
	//bool useDefault = path == "default";

	// load the texture
	auto& assets = AssetManager::Instance();
	auto* tex = assets.getTexture(path);

	// assuming it isn't found, attempt to load the texure
	// if it's still not found, the file doesnt exist and we fail
	if (tex == nullptr)
	{
		tex = assets.loadTexture(path, path);
		if (tex == nullptr)
		{
			LOG_ERROR(makeFileLoadFailErrorMessage(level.path, "Tileset texture'" + path + "' does not exist"));
			return false;
		}
		else
			LOG_DEBUG("Tileset texture '" + path + "' successfully loaded.");
	}
	else
		LOG_DEBUG("Tileset texture '" + path + "' already loaded.");

	// begin forming the tileset
	GameObject tile;
	std::vector<GameObject> tileSet;

	// Set GameObject size (Scaling up 4x for visibility)
	// 4 * 18 = 3 * 24 = 72 (dino size is 24).
	tile.setSize(sf::Vector2f(size * 4, size * 4));
	tile.setCollisionBox({ { 0, 0 }, tile.getSize() });

	for (int i = 0; i < columns * rows; i++)
	{
		int row = i / columns;
		int col = i % columns;

		tile.setTextureRect({
			{ (size + spacing) * col, (size + spacing) * row },
			{ size, size }
		});

		if (col <= haveCollideLeft || col >= haveCollideRight) tile.setCollider(true);
		else tile.setCollider(false);
		tileSet.push_back(tile);
	}
}

bool LevelManager::loadTilemap(Level& level, TileMapType type)
{
	std::string path;
	int x, y;
	level.file >> path >> x >> y;
	bool useDefault = path == "default";

	// check if the tilemap texture  is loaded into AssetManager. if not we fail
	auto& assets = AssetManager::Instance();
	sf::Texture* tex = nullptr;
	if (useDefault)
		tex = assets.getTexture(type == TileMapType::BACKGROUND
			? AssetManager::Defaults::BACKGROUND
			: AssetManager::Defaults::TERRAIN);
	else
		tex = assets.getTexture(path);

	// if we are using default and we cant find it, fail load immediately
	if (tex != nullptr && useDefault)
	{
		LOG_ERROR(makeFileLoadFailErrorMessage(level.path, "Attempted to use default tileset before load."));
		return false;
	}

	// if we arnt using default, and the tileset isnt loaded, try load it.
	// if we cant, fail the level load as the tiles don't exist at all
	else if (tex != nullptr && !useDefault)
	{
		auto* result = assets.loadTexture(path, "terrain_" + level.name);
		if (result == nullptr)
		{
			LOG_ERROR(makeFileLoadFailErrorMessage(level.path, "Tileset '" + path + "' does not exist"));
			return false;
		}
	}
}