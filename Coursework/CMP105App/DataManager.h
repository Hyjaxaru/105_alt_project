#pragma once

#include <vector>
#include <fstream>
#include <map>

#include "Logger.h"
#include "AssetManager.h"

class DataManager
{
public:
	DataManager() = default;
	~DataManager() = default;

	DataManager(const DataManager&) = delete;
	DataManager(DataManager&&) = delete;

	DataManager& operator = (const DataManager&) = delete;
	DataManager& operator = (DataManager&) = delete;

	static DataManager& Instance()
	{
		static DataManager instance;
		return instance;
	}

private:

};

//struct Savable {
//	std::string id;
//	std::string path;
//	std::map<std::string, std::string> kv
//};

class Savable {
public:
	Savable() = default;
	Savable(std::string path) {}

	void save();

	template <typename T>
	T* addValue(std::string key, T value);

	template <typename T>
	T* addValue(std::string key, T& value);

	template <typename T>
	T* addValue(std::string key, T* value);

private:
	std::map<std::string, std::string> data;
};

