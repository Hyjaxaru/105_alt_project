#pragma once

#include <vector>
#include <fstream>
#include <filesystem>
#include <map>

#include "Logger.h"
#include "AssetManager.h"

using KVArray = std::vector<std::pair<std::string, std::string>>;

class DataFile {
public:
	// Create a new DataFile with the specified path
	DataFile(const std::string& path);

	void save();

	std::optional<std::string> get(const std::string& key);

	std::optional<int> getInt(const std::string& key);
	std::optional<float> getFloat(const std::string& key);


	void insert(const std::string& key, std::string value);

	void replace(const std::string& key, std::string value);

	void eraze(const std::string& key);

	
	template <typename T>
	void insert(const std::string& key, T value) { insert(key, (std::string)value); }

	template <typename T>
	void replace(const std::string& key, T value) { replace(key, (std::string)value); }

	// allow accessing the data in vector form
	KVArray getArray();

	void setAutosave(const bool& state) { m_autosave = state; };

protected:
	std::string m_path;
	std::map<std::string, std::string> m_kv;
	bool m_autosave;

	bool fileExists() const { return std::filesystem::exists(m_path); }
};