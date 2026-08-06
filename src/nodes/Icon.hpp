#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/Dirs.hpp>

using namespace geode::prelude;

class IconCell;

enum IconFormat
{
	Vanilla,
	MoreIcons
};

class Icon : public CCObject
{
public:
	IconCell *m_cell = nullptr;

	std::string m_name;
	std::string m_author;
	std::string m_fileName;
	std::string m_description;
	std::string m_previewURL;
	IconType m_gamemode;
	IconFormat m_format;
	int m_downloads;

	std::filesystem::path m_zipPath;
	std::vector<std::string> m_collaborators = {};

	//	Creating and adding data
	static Icon *create(std::string name, std::string author, std::string filename, std::string previewURL, int gamemode, int downloads = 0, std::string desc = "", std::string format = "More Icons");
	void addCollab(std::vector<std::string> contributors);

	//	Downloading Processes
	bool isDownloading = false;
	bool isDownloadSuccesful = false;

	Slider *m_downloadBar = nullptr;
	TaskHolder<geode::utils::web::WebResponse> m_listener;

	void downloadIcon();
	void unpackIcon();
};