#include "DataFile.h"

DataFile::DataFile(const std::string& path)
	: m_path(path)
{
	// if a file with the same path exists, attempt to load it instead
	if (fileExists())
	{
		LOG_DEBUG("DataFile at '" + m_path + "' found, attempting to load...");

		std::ifstream file(m_path);

		if (file.good())
		{
			std::string key, value;
			while (file >> key >> value)
				m_kv.insert({ key, value });

			LOG_INFO('\'' + m_path + "' finsihed loading successfully!");
		}
		else
		{
			LOG_WARN('\'' + m_path + "' could not be opened. load failed, creating new");
		}
	}
}

void DataFile::save()
{
	LOG_DEBUG("Beginning save to '" + m_path + '\'');

	std::ofstream file(m_path);

	if (file.good())
	{
		for (auto& pair : m_kv)
			file << pair.first << ' ' << pair.second << '\n';

		LOG_INFO('\'' + m_path + "' finsihed saving successfully!")
	}
	else
	{
		LOG_ERROR('\'' + m_path + "' could not be opened. Save failed.");
	}

	file.close();
}

void DataFile::insert(const std::string& key, std::string value)
{
	m_kv.insert({ key, value });
}

std::optional<std::string> DataFile::get(const std::string& key)
{
	auto pair = m_kv.find(key);
	if (pair != m_kv.end())
		return pair->second;
	else
		return {}
}

void DataFile::eraze(const std::string& key)
{
	if (m_kv.find(key) != m_kv.end())
		m_kv.erase(key);
}

bool DataFile::fileExists() const
{
	return std::filesystem::exists(m_path);
}
