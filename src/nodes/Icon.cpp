#include "Icon.hpp"
#include "../ui/IconCell.hpp"

IconType iconFromString(std::string gamemode)
{
	std::vector<std::string> gamemodes = {
		"Cube",
		"Ship",
		"Ball",
		"UFO",
		"Wave",
		"Robot",
		"Spider",
		"Swing",
		"Jetpack"};

	for (int ii = 0; ii < gamemodes.size(); ii++)
	{
		if (utils::string::equalsIgnoreCase(gamemodes[ii], gamemode))
			return IconType{ii};
	}

	return IconType::Item;
}

IconFormat formatFromString(std::string format)
{
	if (utils::string::equalsIgnoreCase(format, "vanilla"))
	{
		return IconFormat::Vanilla;
	};

	return IconFormat::MoreIcons;
}

Icon *Icon::create(std::string name, std::string author, std::string filename, std::string gamemode, std::string desc, int downloads, std::string format, bool hasProjectFiles)
{
	auto icon = new Icon();

	//	Feels the Filling
	icon->m_name = name;
	icon->m_author = author;
	icon->m_description = desc;
	icon->m_fileName = filename;
	icon->m_gamemode = iconFromString(gamemode);
	icon->m_format = formatFromString(format);
	icon->m_downloads = downloads;
	icon->m_hasProjectFiles = hasProjectFiles;

	//	Returns the created Icon
	return icon;
}

void Icon::addCollaborators(std::vector<std::string> collab)
{
	//	Hi, wanna collab?
	this->m_collaborators = collab;
}

void Icon::downloadIcon()
{
	isDownloading = true;

	if (m_iconCell)
	{
		m_iconCell->m_downloadBtn->setVisible(false);
		m_downloadBar->setVisible(true);
	}

	auto weak = geode::WeakRef(this);
	auto req = geode::utils::web::WebRequest();

	//	Progress
	req.onProgress(
		[this, weak](web::WebProgress const &progress)
		{
			//	Test
			log::info("Progress {}", progress.downloadProgress().value_or(0.f));

			if (m_downloadBar && weak.valid())
			{
				m_downloadBar->setValue(progress.downloadProgress().value_or(0.01f) / 100.0f);
			}
		});

	//	Downloading
	auto downloadURL = fmt::format("https://github.com/Asterveila/IconGallery/raw/refs/heads/main/icons/{}", m_fileName);
	m_listener.spawn(
		req.get(downloadURL),
		[this, weak](geode::utils::web::WebResponse res)
		{
			if (!weak.lock())
				return;

			if (res.ok())
			{
				auto test = geode::utils::string::replace(m_fileName, ".gdicon", ".zip");
				if (res.into(fmt::format("{}/{}", Mod::get()->getConfigDir(), test)))
				{
					isDownloadSuccesful = true;
					isDownloading = false;

					m_zipfile = Mod::get()->getConfigDir() / test;

					auto popup = createQuickPopup(
						"Icon Downloaded!",
						"Do you want to unzip the files of the icon?",
						"No",
						"Yes",
						[this](auto, bool btn)
						{
							if (btn)
							{
								unpackIcon();
							}
						});

					if (m_iconCell)
					{
						m_iconCell->updateDownload();
					}
				};
			}
			else
			{
				Notification::create("There was an error", NotificationIcon::Error)->show();
				log::error("Failed on loading data - {}", res.errorMessage());
			}
		});
}

void Icon::unpackIcon()
{
	log::debug("EXISTS? {}", std::filesystem::exists(m_zipfile));
	log::debug("PATH {}", m_zipfile);

	if (!std::filesystem::exists(m_zipfile))
	{
		Notification::create("There was an error...", NotificationIcon::Error)->show();
		log::error("ZIP file not found");
		return;
	}

	std::vector<std::string> gamemodeFile = {
		"icon",
		"ship",
		"ball",
		"ufo",
		"wave",
		"robot",
		"spider",
		"swing",
		"jetpack"
	};

	auto gamemode = gamemodeFile[(int)m_gamemode];
	auto zipfilePath = m_zipfile;
	auto unzipDir = Loader::get()->getInstalledMod("hiimjustin000.more_icons")->getConfigDir() / gamemode;
	auto result = utils::file::Unzip::intoDir(zipfilePath, unzipDir, true);

	log::debug("End of Unpacking?");
};