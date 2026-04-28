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
	bool validLevel, foundVersion = false;

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
		if (keyword == "VERSION" && !foundVersion)
		{
			int version;
			file >> version;

			if (version > LOADER_VERSION) {
				LOG_ERROR(makeFileLoadFailErrorMessage(filePath, "Newer loader version required."));
				return;
			}
		}
	}
}