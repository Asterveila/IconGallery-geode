#include "IconCell.hpp"

bool IconCell::init(Icon *icon, bool even)
{
	if (!CCLayer::init())
	{
		return false;
	}

	m_icon = icon;
	m_icon->m_cell = this;
	this->setContentSize(m_size);

	//	Background of the Cell
	auto m_background = CCLayerColor::create();
	m_background->setContentSize(m_size);
	m_background->setOpacity(255);
	m_background->setZOrder(-2);
	addChildAtPosition(m_background, Anchor::BottomLeft, ccp(0, 0), false);

	//	Set Color of Background
	if (even)
		m_background->setColor({194, 114, 62});
	else
		m_background->setColor({161, 88, 44});

	//	Preview
	int attempts = 0;
	m_preview = LazySprite::create({200, 200});
	m_preview->setLoadCallback(
		[this, &attempts, icon](Result<> result)
		{
			if (!result.isOk())
			{
				if (attempts < 3)
				{
					log::info("failed to load preview, please refresh Icon Gallery to try again");
					m_preview->initWithFile("Placeholder.png"_spr);
				}
				else
				{
					m_preview->loadFromUrl(m_icon->m_previewURL, geode::LazySprite::Format::kFmtPng);
					attempts += 1;
				}
			}
		});
	this->addChildAtPosition(m_preview, Anchor::Left, ccp(35, 0), false);
	m_preview->loadFromUrl(m_icon->m_previewURL, geode::LazySprite::Format::kFmtPng);

	//	Name of the Icon
	auto m_iconName = CCLabelBMFont::create(m_icon->m_name.c_str(), "bigFont.fnt");
	m_iconName->limitLabelWidth(180.0f, 0.5f, 0.25f);
	m_iconName->setAnchorPoint({0, 0.5});
	this->addChildAtPosition(m_iconName, Anchor::Left, ccp(70, 15), false);

	std::string authors = m_icon->m_author;

	if (m_icon->m_collaborators.size() != 0)
	{
		if (m_icon->m_collaborators.size() > 1)
		{
			authors = fmt::format("By {} and More", m_icon->m_author);
		}
		else
		{
			authors = fmt::format("By {} and {}", m_icon->m_author, m_icon->m_collaborators[0]);
		}
	};

	//	Author of the Icon
	auto m_iconAuthor = CCLabelBMFont::create(fmt::format("By {}", m_icon->m_author).c_str(), "goldFont.fnt");
	m_iconAuthor->setScale(0.5f);
	m_iconAuthor->setAnchorPoint({0, 0.5});
	this->addChildAtPosition(m_iconAuthor, Anchor::Left, ccp(70, 0), false);

	//	Download Count
	auto m_downloadIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	m_downloadIcon->setScale(0.4f);
	m_downloadIcon->setAnchorPoint({0, 0.5});
	// m_downloadIcon->setPosition(ccp(m_downloadCount->getPosition().x - .3f, 11));
	this->addChildAtPosition(m_downloadIcon, Anchor::Left, ccp(70, -15), false);

	auto m_downloadCount = CCLabelBMFont::create(fmt::format("{}", m_icon->m_downloads).c_str(), "bigFont.fnt");
	m_downloadCount->setScale(0.3f);
	m_downloadCount->setAnchorPoint({0, 0.5});
	m_downloadCount->setColor({100, 255, 100});
	this->addChildAtPosition(m_downloadCount, Anchor::Left, ccp(82.5, -15), false);

	//	Gamemode Label
	auto m_gamemodeLabel = getGamemodeLabel(m_icon->m_gamemode);
	this->addChildAtPosition(m_gamemodeLabel, Anchor::Left, ccp(m_downloadCount->getPositionX() + m_downloadCount->getScaledContentWidth() + 5, -15), false);

	//	Format Label
	const char *formatText = m_icon->m_format == IconFormat::Vanilla ? "Vanilla" : "More Icons";
	auto m_formatLabel = CCLabelBMFont::create(formatText, "bigFont.fnt");
	m_formatLabel->setAnchorPoint({0, 0.5});
	m_formatLabel->setColor({150, 150, 255});
	m_formatLabel->setScale(0.3f);

	if (m_icon->m_format == IconFormat::Vanilla)
		m_formatLabel->setColor({150, 255, 150});

	this->addChildAtPosition(m_formatLabel, Anchor::Left, ccp(m_gamemodeLabel->getPositionX() + m_gamemodeLabel->getScaledContentWidth() + 5, -15), false);

	//	Main Menu
	auto m_menu = CCMenu::create();
	m_menu->setContentSize(m_size);
	this->addChildAtPosition(m_menu, Anchor::BottomLeft, ccp(0, 0), false);

	//	Info Button
	auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
	infoSpr->setScale(0.775f);

	auto m_infoBtn = CCMenuItemSpriteExtra::create(
		infoSpr,
		this,
		menu_selector(IconCell::onInfo));
	m_menu->addChildAtPosition(m_infoBtn, Anchor::Right, ccp(-70, 0), false);

	//	Download Button
	auto downloadSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	m_downloadBtn = CCMenuItemSpriteExtra::create(
		downloadSpr,
		this,
		menu_selector(IconCell::onDownload));
	m_menu->addChildAtPosition(m_downloadBtn, Anchor::Right, ccp(-30, 0), false);

	//	SLIDER
	m_icon->m_downloadBar = Slider::create(this, nullptr);
	m_icon->m_downloadBar->setContentSize(m_size);
	this->addChild(m_icon->m_downloadBar);

	m_icon->m_downloadBar->m_groove->setScale(0.75f);
	// m_icon->m_downloadBar->m_groove->setAnchorPoint({1, 0.5});
	m_icon->m_downloadBar->m_touchLogic->setVisible(false);
	m_icon->m_downloadBar->m_groove->setPosition({m_size.width / 2, 7.5});

	updateStatus();
	return true;
}

void IconCell::updateStatus()
{
	if (!m_icon)
		return;

	log::debug("Update called");

	if (m_icon->isDownloading)
	{
		m_downloadBtn->setVisible(false);
	}

	if (m_icon->m_downloadBar)
	{
		m_icon->m_downloadBar->setVisible(m_icon->isDownloading);
	}

	if (m_icon->isDownloadSuccesful)
	{
		m_downloadBtn->setVisible(false);
		log::debug("Succesfull Called");
	}
}

CCLabelBMFont *IconCell::getGamemodeLabel(IconType gamemode)
{
	const char *text = "????";
	ccColor3B color = {120, 120, 120};

	switch (gamemode)
	{
	case IconType::Cube:
		text = "Cube";
		color = {120, 255, 120};
		break;

	case IconType::Ship:
		text = "Ship";
		color = {255, 120, 255};
		break;

	case IconType::Ball:
		text = "Ball";
		color = {255, 120, 120};
		break;

	case IconType::Ufo:
		text = "UFO";
		color = {255, 180, 120};
		break;

	case IconType::Wave:
		text = "Wave";
		color = {120, 220, 255};
		break;

	case IconType::Robot:
		text = "Robot";
		color = {180, 180, 180};
		break;

	case IconType::Spider:
		text = "Spider";
		color = {180, 90, 255};
		break;

	case IconType::Swing:
		text = "Swing";
		color = {255, 255, 80};
		break;

	case IconType::Jetpack:
		text = "Jetpack";
		color = {255, 120, 255};
		break;

	default:
		break;
	}

	auto label = CCLabelBMFont::create(text, "bigFont.fnt");
	label->setAnchorPoint({0, 0.5});
	label->setColor(color);
	label->setScale(0.3f);

	return label;
}

void IconCell::onDownload(CCObject *)
{
	auto noPackExists = Mod::get()->getSettingValue<std::filesystem::path>("icon-pack-folder").empty();
	if (noPackExists)
	{
		auto warning = createQuickPopup(
		"Set Folder",
		"Please set a Texture Pack folder in the settings of the mod to download icons",
		"Ok",
		nullptr,
		[](auto, auto) {});

		return;
	};

	auto popup = createQuickPopup(
		"Download Icon?",
		"Are you sure you want to download <cy>" + m_icon->m_name + "</c>?",
		"No",
		"Yes",
		[this](auto, bool btn)
		{
			if (btn)
			{
				m_icon->downloadIcon();
				updateStatus();
			}
		});
}

void IconCell::onInfo(CCObject *)
{
	if (!m_icon)
		return;

	std::string data =
		"### Author: " + m_icon->m_author +
		"\n***" +
		"\n\n" + m_icon->m_description;

	MDPopup::create(
		m_icon->m_name,
		data,
		"Ok")
		->show();
}

IconCell *IconCell::create(Icon *m_icon, bool even)
{
	{
		auto ret = new IconCell();

		if (ret->init(m_icon, even))
		{
			ret->autorelease();
			return ret;
		}

		delete ret;
		return nullptr;
	}
}