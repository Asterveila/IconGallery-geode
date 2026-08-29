#include "GalleryHelp.hpp"

bool GalleryHelpPopup::init()
{
	if (!Popup::init(410.f, 240.f, "GJ_square02.png"))
		return false;

	//	Background
	auto background = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
	background->setContentSize({200, 400});
	background->setAnchorPoint({0, 0.5});
	background->setColor({0, 0, 0});
	background->setOpacity(50);
	background->setScale(0.5);
	background->setID("background");
	m_mainLayer->addChildAtPosition(background, Anchor::Left, ccp(10, -10), false);

	auto label = CCLabelBMFont::create("Icon Gallery - Help", "goldFont.fnt");
	label->setAnchorPoint({0.5, 0});
	label->setScale(0.75);
	label->setID("title");
	m_mainLayer->addChildAtPosition(label, Anchor::Top, ccp(0, -25), false);

	m_buttonMenu = CCMenu::create();
	m_buttonMenu->setContentSize({100, 200});
	m_buttonMenu->setAnchorPoint({0, 0.5});
	m_buttonMenu->setLayout(ColumnLayout::create()
								->setCrossAxisLineAlignment(AxisAlignment::Start)
								->setCrossAxisOverflow(false)
								->setAutoScale(false)
								->setAxisReverse(true)
								->setGap(12));
	m_mainLayer->addChildAtPosition(m_buttonMenu, Anchor::Left, ccp(10, -10), false);

	std::vector<gd::string> text = {
		"About", "How to Use", "Downloading", "Uploading", "Support"};

	for (int ii = 0; ii < 5; ii++)
	{
		auto buttonSpr = CCLabelBMFont::create(text[ii].c_str(), "bigFont.fnt", 100);
		buttonSpr->limitLabelWidth(100, 0.4, 0.3);
		buttonSpr->setAnchorPoint({0, 0.5});
		buttonSpr->setID("label");

		auto button = CCMenuItemSpriteExtra::create(
			buttonSpr,
			this,
			menu_selector(GalleryHelpPopup::onPage));

		button->m_scaleMultiplier = 1.1f;
		button->setTag(ii);

		m_buttonMenu->addChild(button);
		m_buttonMenu->updateLayout();
	}

	m_textArea = MDTextArea::create("[Insert Text Here]", {280, 200});
	m_textArea->setAnchorPoint({0.5, 0.5});
	m_textArea->setID("text-area");
	m_mainLayer->addChildAtPosition(m_textArea, Anchor::Center, ccp(52.5, -10), false);

	setInfo();

	this->setID("gallery-help-popup");
	return true;
}

void GalleryHelpPopup::onPage(CCObject *sender)
{
	auto tag = sender->getTag();

	if (m_page == tag)
		return;

	auto prevPage = m_page;
	m_page = tag;

	if (auto oldBtn = static_cast<CCMenuItemSpriteExtra *>(m_buttonMenu->getChildByTag(prevPage)))
	{
		auto text = static_cast<CCLabelBMFont *>(oldBtn->getChildByID("label"));
		text->setColor({255, 255, 255});
	}

	if (auto btn = static_cast<CCMenuItemSpriteExtra *>(m_buttonMenu->getChildByTag(m_page)))
	{
		auto text = static_cast<CCLabelBMFont *>(btn->getChildByID("label"));
		text->setColor({0, 255, 255});
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
		info = "# About this project\nThe <co>Icon Gallery</c> started as a website created for the <cg>Geometry Dash community</c>, made to act as a museum of public <cc>Custom Icons</c> people could download and then install in their clients.\n\n<cc>*Please be aware, that the servers of this mod might not be stable enough to handle many requests at once, so if Icons don't load properly in the gallery, this is the reason*</c>";
		break;

	case 1:
		info = "# How to Use\nUpon start-up, the mod will ask you if you want to create a <cc>Texture Pack</c>, this is where the icons downloaded from the gallery will be saved <cg>by default</c>.\n\nIf accepted, the pack will be created inside the <cg>Texture Loader</c> manager, which you can select and apply it.\n***\n### Other Locations:\nIn the settings, you can enable Special Locations if you don't want to deal with the hastle of having a texture pack, although <cr>be advised that any icons downloaded could replace existing icons in there.</c>";
		break;

	case 2:
		info = "# Downloading Icons\nOnce you find the icons you wish to download, simply click the \"<co>Download</c>\" button at the right side of the icon.\n\nIf \"<cy>**Automatic Unpacking**</c>\" is <cg>enabled</c>, the downloaded zip file will be unpacked and the sprites of the icon will be added to the destined icon pack folder.\n\nIf \"<cy>**Automatic Unpacking**</c>\" is <cr>disabled</c>, the mod will prompt you to unpack it or not.\n***\n### Formats:\n* **Icons with the \"<cb>Vanilla</c>\" format**: will be added inside an \"Icons\" folder of the texture pack.\n`[pack folder]\\icons`\n* **Icons with the \"<cp>More Icons</c>\" format:** will be added in their specified gamemode folder of <cl>More Icons</c>.\n`[pack folder]\\config\\hiimjustin000.more_icons\\(gamemode)`";
		break;

	case 3:
		info = "# Uploading Icons\nIf you wish to upload an icon here, please go to the [**Website version of the Icon Gallery**](https://iconsgallery.pages.dev/), click on the <cg>bottom right button</c> and follow the instructions on how to submit your icons.\n\n<cc>*Please* ***read the guidelines*** *and make sure your icon follows the right format. Any icon that breaks either will have higher chances of rejection.*</c>";
		break;

	case 4:
		info = "# Bugs or crashes\nIf you have found bugs with the mod itself, or discovered that an icon doesn't properly load or cause crashes, make sure to report it to the admins of the gallery via [Asterveila's Discord Server](https://discord.gg/dceY3uvGzD).";
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