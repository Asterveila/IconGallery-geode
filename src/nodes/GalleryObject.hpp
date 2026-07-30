#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GalleryObject : public CCObject
{
public:
	int m_index;
	int m_downloads;

	std::string m_iconName;
	std::string m_iconAuthor;
	std::string m_description;

	bool m_hasProjectFiles;
	bool m_isVanilla;

	IconType m_gamemode;

	static GalleryObject *create(int index, std::string name, std::string author, std::string desc, IconType gamemode, int downloads, bool isVanilla, bool hasProjectFiles = false);

	//	void downloadIcon();
	//	bool isDownloading = false;
	//	bool downloadSuccesful = false;
	//	async::TaskHolder<geode::utils::web::WebResponse> m_downloadListener;
};