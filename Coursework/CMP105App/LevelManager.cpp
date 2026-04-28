#include "LevelManager.h"

void LevelManager::loadLevels(std::string dirPath)
{

}

void LevelManager::loadLevel(std::string filePath)
{
	// get the level and ensure we can access it
	std::ifstream file(filePath);

	if (!file.good()) {
		LOG_ERROR_NOLINE(makeFileLoadFailErrorMessage(filePath, "File not found."));
		return;
	}

	// decode all of the keywords 
	std::string keyword, levelName;
	bool validLevel = false, validVersion = false, validTileset = false;

	while (file >> keyword)
	{
		// I tried to make switch statements work here
		// unfortunately there isn't enough time to fight with it

		// detect the level header
		// if found, fill in the level name
		// this should must occur only once. no LEVEL = invalid, more than one = ignored
		if (keyword == "LEVEL" && !validLevel)
		{
			file >> levelName;
			validLevel = true;
		}

		// detect the minimum version of the loader required to load the level
		// helps with versioning if this continues
		else if (keyword == "VERSION" && !validVersion)
		{
			int version;
			file >> version;

			// fail if the file is too new
			if (version > LOADER_VERSION) {
				LOG_ERROR(makeFileLoadFailErrorMessage(filePath, "Newer loader version required."));
				return;
			}

			// the version number is valid!
			validVersion = true;
		}

		// get the name of the tileset to use
		else if (keyword == "TILESET" && !validTileset)
		{
			std::string tilesetPath;
			file >> tilesetPath;
			bool useDefault = tilesetPath == "default";

			// check if the tileset is loaded into AssetManager. if not we fail
			auto& assets = AssetManager::Instance();
			bool loaded = false;
			if (useDefault)
				loaded = assets.getTexture(AssetManager::Defaults::TILEMAP) != nullptr;
			else
				loaded = assets.getTexture(tilesetPath) != nullptr;

			// if we are using default and we cant find it, fail load immediately
			if (!loaded && useDefault)
			{
				LOG_ERROR(makeFileLoadFailErrorMessage(filePath, "Attempted to use default tileset before load."));
				return;
			}

			// if we arnt using default, and the tileset isnt loaded, try load it.
			// if we cant, fail the level load as the tiles don't exist at all
			else if (!loaded && !useDefault)
			{
				auto* result = assets.loadTexture(tilesetPath, levelName);
				if (result == nullptr)
				{
					LOG_ERROR(makeFileLoadFailErrorMessage(filePath, "Tileset '" + tilesetPath + "' does not exist"));
					return;
				}
			}
			
			// finish up the stage
			validTileset = true;
		}
	}
}