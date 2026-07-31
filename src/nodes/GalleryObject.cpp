#include "GalleryObject.hpp"

IconType convertToIcon(std::string gamemode)
{
	auto value = std::string_view(gamemode);

	if (value == std::string_view("Ship"))
	{
		return IconType::Ship;
	}
	else if (value == std::string_view("Ball"))
	{
		return IconType::Ball;
	}
	else if (value == std::string_view("UFO"))
	{
		return IconType::Ufo;
	}
	else if (value == std::string_view("Wave"))
	{
		return IconType::Wave;
	}
	else if (value == std::string_view("Robot"))
	{
		return IconType::Robot;
	}
	else if (value == std::string_view("Spider"))
	{
		return IconType::Spider;
	}
	else if (value == std::string_view("Swing"))
	{
		return IconType::Swing;
	}
	else if (value == std::string_view("Jetpack"))
	{
		return IconType::Jetpack;
	};

	return IconType::Cube;
};

GalleryObject *GalleryObject::create(std::string name, std::string author, std::string desc, std::string gamemode, int downloads, bool isVanilla, bool hasProjectFiles)
{
	auto icon = new GalleryObject();
	auto test = convertToIcon(gamemode);

	//	Fills it with the info
	icon->m_name = name;
	icon->m_author = author;
	icon->m_description = desc;

	icon->m_gamemodeString = gamemode;
	icon->m_gamemode = test;

	icon->m_downloads = downloads;
	icon->m_isVanilla = isVanilla;
	icon->m_hasProjectFiles = hasProjectFiles;

	//	Returns the created Object
	return icon;
};

void GalleryObject::addCollaborators(std::vector<std::string> collab)
{
	this->m_collaborators = collab;
}

void GalleryObject::downloadIcon()
{
	auto weak = geode::WeakRef(this);
	auto req = geode::utils::web::WebRequest();

	req.onProgress([this, weak](web::WebProgress const &progress)
				   { log::info("progress: {}", progress.downloadProgress().value_or(0.f)); });

	m_listener.spawn(
		req.get("https://github.com/Asterveila/IconGallery/raw/refs/heads/main/icons/Azoth.gdicon"),
		[this, weak](geode::utils::web::WebResponse res)
		{
			if (!weak.lock())
			{
				return;
			}

			if (res.ok())
			{
				if (res.into(fmt::format("{}/{}.zip", Mod::get()->getConfigDir(), "Azoth")))
				{
					Notification::create("Icon Downloaded!", NotificationIcon::Success)->show();
				}
			}
		});
}