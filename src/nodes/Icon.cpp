#include "Icon.hpp"
#include "../ui/IconCell.hpp"

IconFormat formatFromString(std::string format)
{
	log::debug("CHECKING STRING FORMAT");

	if (std::string_view(format) == std::string_view("Vanilla"))
	{
		return IconFormat::Vanilla;
	};

	return IconFormat::MoreIcons;
}

Icon *Icon::createIcon(std::string name, std::string author, std::string filename, std::string previewURL, int gamemode, std::string desc, int downloads, std::string format)
{
	auto icon = new Icon();

	//	Feels the Filling
	icon->m_name = name;
	icon->m_author = author;
	icon->m_fileName = filename;
	icon->m_previewURL = previewURL;
	icon->m_description = desc;
	icon->m_gamemode = IconType{gamemode};
	icon->m_format = formatFromString(format);
	icon->m_downloads = downloads;

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
	auto downloadURL = fmt::format("https://expiration-hit-supplier-manufacturer.trycloudflare.com/api/sprites/{}", m_fileName);

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

					log::debug("Download completed");

					if (m_iconCell)
					{
						log::debug("Cell Called");
						m_iconCell->updateDownload();
					}

					auto popup = createQuickPopup(
						"Icon Downloaded!",
						"Do you want to unzip the files of the icon? (Note: This is still experimental and could crash)",
						"No",
						"Yes",
						[this](auto, bool btn)
						{
							if (btn)
							{
								unpackIcon();
							}
						});
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
		"jetpack"};

	auto gamemode = gamemodeFile[(int)m_gamemode];
	auto zipfilePath = m_zipfile;
	auto unzipDir = Loader::get()->getInstalledMod("hiimjustin000.more_icons")->getConfigDir() / gamemode;
	auto result = utils::file::Unzip::intoDir(zipfilePath, unzipDir, true);

	auto popup = createQuickPopup(
		"Icon unpacked!",
		"Icon succesfully unpacked. Please refresh the textures to load the icon via <cy>More Icons</c> mod!",
		"Ok",
		nullptr, [](auto, auto) {});
};