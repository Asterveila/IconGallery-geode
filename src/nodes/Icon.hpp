#pragma once
#include <Geode/Geode.hpp>

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
	IconCell * m_iconCell = nullptr;

	std::string m_name;
	std::string m_author;
	std::string m_description;
	std::string m_fileName;
	std::string m_previewURL;

	int m_downloads;
	bool m_hasProjectFiles;

	IconType m_gamemode;
	IconFormat m_format;

	std::vector<std::string> m_collaborators = {};
	std::filesystem::path m_zipfile = "";

	//	Something
	Slider * m_downloadBar = nullptr;

	//	Creating and adding data
	static Icon *create(std::string name, std::string author, std::string filename, std::string gamemode, std::string desc = "", int downloads = 0, std::string format = "vanilla", bool hasProjectFiles = false);
	static Icon *createNew(std::string name, std::string author, std::string filename, std::string previewURL, int gamemode, std::string desc = "", int downloads = 0, std::string format = "Vanilla");

	void addCollaborators(std::vector<std::string> collab);

	//	Downloading Processes
	void downloadIcon();
	bool isDownloading = false;
	bool isDownloadSuccesful = false;
	TaskHolder<geode::utils::web::WebResponse> m_listener;
	void unpackIcon();
};