#include "GalleryHelp.hpp"

bool GalleryHelpPopup::init()
{
	if (!Popup::init(410.f, 240.f, "GJ_square02.png"))
		return false;

	//	Background
	auto background = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
	background->setContentSize({200, 340});
	background->setAnchorPoint({0, 0.5});
	background->setColor({0, 0, 0});
	background->setOpacity(50);
	background->setScale(0.5);
	background->setID("background");
	m_mainLayer->addChildAtPosition(background, Anchor::Left, ccp(10, 5), false);

	auto label = CCLabelBMFont::create("Icon Gallery - Help", "goldFont.fnt");
	label->setAnchorPoint({0.5, 0});
	label->setScale(0.75);
	label->setID("title");
	m_mainLayer->addChildAtPosition(label, Anchor::Top, ccp(0, -25), false);

	m_selectMenu = CCMenu::create();
	m_selectMenu->setContentSize({100, 170});
	m_selectMenu->setAnchorPoint({0, 0.5});
	m_selectMenu->setLayout(ColumnLayout::create()
								->setCrossAxisLineAlignment(AxisAlignment::Start)
								->setCrossAxisOverflow(false)
								->setAutoScale(false)
								->setAxisReverse(true)
								->setGap(12));
	m_mainLayer->addChildAtPosition(m_selectMenu, Anchor::Left, ccp(10, 5), false);

	std::vector<gd::string> text = {
		"About", "How to Use", "Downloading", "Uploading", "FAQ", "Support"};

	for (int ii = 0; ii < text.size(); ii++)
	{
		auto buttonSpr = CCLabelBMFont::create(text[ii].c_str(), "bigFont.fnt", 100);
		buttonSpr->limitLabelWidth(100, 0.4, 0.3);
		buttonSpr->setAnchorPoint({0, 0.5});
		buttonSpr->setID("label");

		if (ii == 0)
			buttonSpr->setColor({255, 255, 0});

		auto button = CCMenuItemSpriteExtra::create(
			buttonSpr,
			this,
			menu_selector(GalleryHelpPopup::onPage));

		button->m_scaleMultiplier = 1.1f;
		button->setTag(ii);

		m_selectMenu->addChild(button);
		m_selectMenu->updateLayout();
	}

	m_textArea = MDTextArea::create("[Insert Text Here]", {280, 200});
	m_textArea->setAnchorPoint({0.5, 0.5});
	m_textArea->setID("text-area");
	m_mainLayer->addChildAtPosition(m_textArea, Anchor::Center, ccp(52.5, -10), false);

	//	Language Button
	auto offSpr = CCLabelBMFont::create("Language: ENG", "goldFont.fnt");

	auto onSpr = CCLabelBMFont::create("Language: ESP", "goldFont.fnt");

	auto m_languageButton = CCMenuItemToggler::create(
		CCLabelBMFont::create("Language: ENG", "goldFont.fnt"),
		CCLabelBMFont::create("Language: ESP", "goldFont.fnt"),
		this,
		menu_selector(GalleryHelpPopup::onLanguage));
	m_languageButton->setID("language-button");
	m_languageButton->toggle(m_spanish);
	m_buttonMenu->addChildAtPosition(m_languageButton, Anchor::Bottom, ccp(0, 0), false);

	//	Calls the function to set the info
	setInfo();

	this->setID("gallery-help-popup");
	return true;
}

void GalleryHelpPopup::onLanguage(CCObject *sender)
{
	auto button = static_cast<CCMenuItemToggler *>(sender);
	m_spanish = !m_spanish;
	button->toggle(m_spanish);

	setInfo();
}

void GalleryHelpPopup::onPage(CCObject *sender)
{
	auto tag = sender->getTag();

	if (m_page == tag)
		return;

	auto prevPage = m_page;
	m_page = tag;

	if (auto oldBtn = static_cast<CCMenuItemSpriteExtra *>(m_selectMenu->getChildByTag(prevPage)))
	{
		auto text = static_cast<CCLabelBMFont *>(oldBtn->getChildByID("label"));
		text->setColor({255, 255, 255});
	}

	if (auto btn = static_cast<CCMenuItemSpriteExtra *>(m_selectMenu->getChildByTag(m_page)))
	{
		auto text = static_cast<CCLabelBMFont *>(btn->getChildByID("label"));
		text->setColor({255, 255, 0});
	}

	setInfo();
}

void GalleryHelpPopup::setInfo()
{
	if (!m_textArea)
		return;

	gd::string info;

	switch (m_page)
	{
	case 0:
		info = fmt::format(
			"{}{}{}",
			"# About This",
			"\nThe <co>**Icon Gallery**</c> started as a website made to act as a museum of public <cp>custom icons</c>, so people could download them and add them in their games.",
			"\n\n<cc>*Please be aware, that the servers of this mod might not be stable enough to handle many requests at once, so if Icons don't load properly in the gallery, this is the reason*</c>");
		break;

	case 1:
		info = fmt::format(
			"{}{}{}{}{}{}",
			"# How to Use",
			"\nThe mod will automatically set up a <cc>Texture Pack</c> upon start-up, where all the icons downloaded from the gallery will be saved by default.",
			"\n\nThe mod does **NOT** automatically apply the texture pack, so you have to do that yourself inside the <cg>Texture Loader</c> manager.",
			"\n***\n### Other Locations:",
			"\nIn the settings, you can enable special locations to download icons if you don't want a texture pack, such as the More Icons folder (for general) or the original icons (for vanilla).",
			"\n\n<cr>Be advised, that you could override icons that are already there</c>.");
		break;

	case 2:
		info = fmt::format(
			"{}{}{}{}{}{}{}{}",
			"# Downloading Icons",
			"\nOnce you find the icons you wish to download, simply click the \"<co>Download</c>\" button at the right side of the icon.",
			"\n***\n### Formats:",
			"\n* **Icons with the \"<cf>Vanilla</c>\" format**: will be added inside an \"Icons\" folder of the texture pack.",
			"\n* **Icons with the \"<cp>More Icons</c>\" format:** will be added in their specified gamemode folder of <cl>More Icons</c>.",
			"\n***\n### Auto-unpack:",
			"\n* If \"<cy>**Automatic Unpacking**</c>\" is <cg>enabled</c>, the downloaded zip file will be unpacked and the sprites of the icon will be added to the destined Icon Pack folder.",
			"\n* If \"<cy>**Automatic Unpacking**</c>\" is <cr>disabled</c>, the mod will prompt you to unpack it or not.");
		break;

	case 3:
		info = fmt::format(
			"{}{}{}",
			"# Uploading Icons",
			"\nIf you wish to upload your own icons in here, please go to the <cg>Website version of the Icon Gallery</c> by clicking the globe button at the bottom right of the Gallery, then the upload button inside the website and follow the instructions on how to submit your icons",
			"\n\n<cc>*Please* ***read the guidelines*** *and make sure your icon follows the right format. Any icon that breaks either will have higher chances of rejection.*</c>");
		break;

	case 4:
		info = fmt::format(
			"{}{}{}{}{}{}{}{}{}{}",
			"# FAQ",
			"\nThis is a list of frequently asked questions, just in case you need to look for more help about the mod, join the discord server.",
			"\n***\n### How do I equip the icons?",
			"\nIf you downloaded icons via the mod, you should already have a Icon pack folder assigned it, go to the <cg>Texture Loader</c> and move the pack named \"<co>Downloaded Icons</c>\" into the applied column and press the apply button to refresh the textures.",
			"\n***\n### I can't find an Icon I downloaded",
			"\nReload your textures if you don't see any changes. Some icons might have the \"<cf>Vanilla</c>\" format, thus they take the place of existing vanilla designs, be sure to look closer into what you have.",
			"\n***\n### An Icon fails to load",
			"\nIf you came across with an icon that doesn't load properly, make sure to report it to the admins of the Gallery via the <cd>discord server</c>.",
			"\n***\n### I'm having connection issues",
			"\nIf you're having connection issues with the mod, please be patient. This can either mean that you have internet connection issues, rate limitations or it's a server-side issue.");
		break;

	case 5:
		info = fmt::format(
			"{}{}",
			"# Support",
			"\nIf you come across bugs with the mod itself or wish to support the development of the gallery in any way, make sure to contact us through the [GitHub repository](https://github.com/MasterL500/IconGallery-geode) or through [Asterveila's Discord Server](https://discord.gg/dceY3uvGzD).");
		break;
	}

	m_textArea->setString(info.c_str());
};

GalleryHelpPopup *GalleryHelpPopup::create()
{
	auto ret = new GalleryHelpPopup;

	if (ret->init())
	{
		ret->autorelease();
		return ret;
	};

	delete ret;
	return nullptr;
}