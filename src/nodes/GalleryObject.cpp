#include "GalleryObject.hpp"

GalleryObject *GalleryObject::create(int index, std::string name, std::string author, std::string desc, IconType gamemode, int downloads, bool isVanilla, bool hasProjectFiles)
{
	auto icon = new GalleryObject();

	//	Fills it with the info
	icon->m_index = index;
	icon->m_iconName = name;
	icon->m_iconAuthor = author;
	icon->m_description = desc;
	icon->m_gamemode = gamemode;
	icon->m_downloads = downloads;
	icon->m_isVanilla = isVanilla;
	icon->m_hasProjectFiles = hasProjectFiles;

	//	Returns the created Object
	return icon;
};