#pragma once

#include <vector>
#include <fstream>
#include <filesystem>
#include <map>

#include "Logger.h"
#include "AssetManager.h"

class DataFile {
public:
	// Create a new DataFile with the specified path
	DataFile(const std::string& path);

	// Load a new 
	//constexpr std::optional<DataFile> load(const std::string& path);

	void save();

	void insert(const std::string& key, std::string value);
	std::optional<std::string> get(const std::string& key);
	void eraze(const std::string& key);

	template <typename T>
	void insert(std::string key, T value)
	{
		insert(key, (std::string)value);
	}

	template <typename T>
	std::optional<T> get(const std::string& key)
	{
		return (T)get(key);
	}

private:
	std::string m_path;
	std::map<std::string, std::string> m_kv;

	bool fileExists() const;
};
