#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GalleryObject : public CCObject
{
public:
	int m_index;
	int m_downloads;

	std::string m_name;
	std::string m_author;
	std::string m_description;
	std::string m_fileName;

	bool m_isVanilla;
	bool m_hasProjectFiles;

	std::vector<std::string> m_collaborators = {};

	IconType m_gamemode;
	std::string m_gamemodeString;

	// static GalleryObject *create(int index, std::string name, std::string author, std::string desc, IconType gamemode, int downloads, bool isVanilla, bool hasProjectFiles = false);

	static GalleryObject *create(std::string name, std::string author, std::string desc = "", std::string gamemode = "Cube", int downloads = 0, bool isVanilla = false, bool hasProjectFiles = false);
	void addCollaborators(std::vector<std::string> collab);

	//	void downloadIcon(); 
	//	bool isDownloading = false;
	//	bool downloadSuccesful = false;
	//	async::TaskHolder<geode::utils::web::WebResponse> m_downloadListener;
};