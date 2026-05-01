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

	void save();

	std::optional<std::string> get(const std::string& key);

	void insert(const std::string& key, std::string value);

	void replace(const std::string& key, std::string value);

	void eraze(const std::string& key);

	template <typename T>
	std::optional<T> get(const std::string& key) { return (T)get(key); }

	template <typename T>
	void insert(const std::string& key, T value) { insert(key, (std::string)value); }

	template <typename T>
	void replace(const std::string& key, T value) { replace(key, (std::string)value); }

	void setAutosave(const bool& state) { m_autosave = state; };

private:
	std::string m_path;
	std::map<std::string, std::string> m_kv;
	bool m_autosave;

	bool fileExists() const { return std::filesystem::exists(m_path); }
};
