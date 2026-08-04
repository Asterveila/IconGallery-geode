#include <algorithm>
#include "../nodes/Icon.hpp"
#include "GalleryLayer.hpp"
#include "IconCell.hpp"

const int CELL_HEIGHT = 70;

bool GalleryLayer::init()
{
	if (!CCLayer::init())
		return false;

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto background = CCSprite::create("background.png"_spr);

	// Background
	background->setPosition(winSize / 2);
	background->setScaleY(winSize.height / background->getContentSize().height);
	background->setScaleX(winSize.width / background->getContentSize().width);
	addChild(background, -2);

	//	Frame
	auto frame = NineSlice::create("Frame.png"_spr);
	frame->setContentSize({400, 256});
	addChildAtPosition(frame, Anchor::Center, ccp(0, 0), false);

	//	Title
	auto title = CCSprite::create("GalleryLabel.png"_spr);
	addChildAtPosition(title, Anchor::Top, ccp(0, -30), false);

	m_pageLabel = CCLabelBMFont::create("", "goldFont.fnt");
	m_pageLabel->limitLabelWidth(200.0f, 0.5f, 0.5f);
	m_pageLabel->setAnchorPoint({1, 0.5});
	m_pageLabel->setVisible(false);
	addChildAtPosition(m_pageLabel, Anchor::TopRight, ccp(-10, -10), false);

	//  Back Button
	auto backMenu = CCMenu::create();
	backMenu->setID("back-menu");
	addChildAtPosition(backMenu, Anchor::TopLeft, ccp(24, -24), false);

	auto backBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(GalleryLayer::onBack));
	backBtn->setID("back-button");
	backBtn->setSizeMult(1.2f);
	backMenu->addChild(backBtn);

	m_pagesMenu = CCMenu::create();
	m_pagesMenu->setID("pages-menu");
	addChildAtPosition(m_pagesMenu, Anchor::BottomLeft, ccp(0, 0), false);

	//  Page Navigation Buttons
	m_prevBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(GalleryLayer::onPage));
	m_prevBtn->setID("prev-page-button");
	m_prevBtn->setVisible(false);
	m_prevBtn->setTag(-1);

	auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	nextSpr->setFlipX(true);

	m_nextBtn = CCMenuItemSpriteExtra::create(
		nextSpr,
		this,
		menu_selector(GalleryLayer::onPage));
	m_nextBtn->setID("next-page-button");
	m_nextBtn->setVisible(false);
	m_nextBtn->setTag(1);

	m_pagesMenu->addChildAtPosition(m_prevBtn, Anchor::Center, ccp(-216, 0), false);
	m_pagesMenu->addChildAtPosition(m_nextBtn, Anchor::Center, ccp(216, 0), false);

	//	Gamemodes Menu
	m_modesMenu = CCMenu::create();
	m_modesMenu->setID("gamemodes-menu");
	m_modesMenu->setLayout(RowLayout::create()
							   ->setGap(2.f));
	addChildAtPosition(m_modesMenu, Anchor::Bottom, ccp(0, 30), false);

	for (int ii = 0; ii < 10; ii++)
		createModeButton(ii, ii == 0);

	//	Buttons Menu
	auto buttonMenu = CCMenu::create();
	buttonMenu->setID("button-menu");
	addChildAtPosition(buttonMenu, Anchor::BottomLeft, ccp(0, 0), false);

	m_pagesBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"),
		this,
		menu_selector(GalleryLayer::onFindPage));
	buttonMenu->addChildAtPosition(m_pagesBtn, Anchor::TopRight, ccp(-30, -40), false);
	m_pagesBtn->setVisible(false);

	auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	settingsSpr->setScale(0.85f);

	auto settingsBtn = CCMenuItemSpriteExtra::create(
		settingsSpr,
		this,
		menu_selector(GalleryLayer::onSettings));
	buttonMenu->addChildAtPosition(settingsBtn, Anchor::BottomLeft, ccp(30, 30), false);

	auto folderBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
		this,
		menu_selector(GalleryLayer::onFolder));
	buttonMenu->addChildAtPosition(folderBtn, Anchor::BottomRight, ccp(-30, 30), false);

	//	Scroll Layer
	m_scrollLayer = ScrollLayer::create({356, 220});
	m_scrollLayer->setZOrder(-2);
	this->addChildAtPosition(m_scrollLayer, Anchor::Center, ccp(-178, -110), false);

	//	Loading Circle
	m_loading = LoadingCircleSprite::create(1);
	m_loading->setID("loading");
	m_loading->setVisible(false);
	this->addChildAtPosition(m_loading, Anchor::Center, ccp(0, 0), false);

	fetchGallery();

	setKeyboardEnabled(true);
	setKeypadEnabled(true);

	this->setID("icon-gallery-layer");
	return true;
};

void GalleryLayer::fetchGallery()
{
	if (m_scrollLayer && m_scrollLayer->m_contentLayer->getChildrenCount() > 0)
		m_scrollLayer->m_contentLayer->removeAllChildren();

	if (m_pageLabel)
		m_pageLabel->setVisible(false);

	if (m_pagesBtn)
		m_pagesBtn->setVisible(false);

	if (m_loading)
		m_loading->setVisible(true);

	std::string url = "https://expiration-hit-supplier-manufacturer.trycloudflare.com/api/index";

	auto order = Mod::get()->getSettingValue<std::string>("sort-order");
	if (utils::string::equalsIgnoreCase(order, "Recent"))
	{
		url = fmt::format("{}?order=Recent", url);
	}
	else
	{
		url = fmt::format("{}?order=Downloads", url);
	}

	url = fmt::format("{}&page={}", url, m_page + 1);

	if (m_mode != IconType::Item)
		url = fmt::format("{}&mode={}", url, (int)m_mode);

	log::debug("URL = {}", url);

	auto req = web::WebRequest();

	m_listener.spawn(
		req.get(url),
		[this](web::WebResponse res)
		{
			if (res.ok() && res.json().isOk())
			{
				m_fetchedData = res.json().unwrap();
				loadGallery();
			}
			else
			{
				Notification::create("There was an error fetching the data", NotificationIcon::Error)->show();
				log::error("Failed on loading data");
			}
		});
};

void GalleryLayer::loadGallery()
{
	if (m_loading)
		m_loading->setVisible(false);

	m_maxPage = m_fetchedData["totalPages"].asInt().unwrapOr(999) - 1;
	auto totalIcons = m_fetchedData["totalIcons"].asInt().unwrapOr(1);
	auto offset = (m_page * 10);

	if (m_pageLabel)
	{
		m_pageLabel->setCString(fmt::format("{} to {} of {}", offset + 1, offset + 10, totalIcons).c_str());
		m_pageLabel->setVisible(true);
	}

	if (m_pagesBtn)
	{
		m_pagesBtn->setVisible(true);
	}

	auto fetchedIcons = m_fetchedData["icons"];

	std::vector<Icon *> icons = {};
	int ii = 0;

	for (auto &value : fetchedIcons)
	{
		auto iconData = value;

		Icon *newIcon = Icon::createNew(
			iconData["iconName"].asString().unwrap(),
			iconData["author"].asString().unwrap(),
			iconData["filename"].asString().unwrap(),
			iconData["previewUrl"].asString().unwrap(),
			iconData["gamemode"].asInt().unwrap(),
			iconData["description"].asString().unwrapOr(""),
			iconData["downloads"].asInt().unwrapOr(0),
			iconData["format"].asString().unwrapOr("More Icons"));

		icons.push_back(newIcon);

		IconCell *cell = IconCell::create(newIcon, ii % 2 == 0);
		m_scrollLayer->m_contentLayer->addChild(cell);
		cell->setPosition(0, (CELL_HEIGHT * fetchedIcons.size()) - CELL_HEIGHT * (ii + 1));
		ii++;
	}

	//	Fixes the scroll layer
	int iconCount = m_scrollLayer->m_contentLayer->getChildrenCount();
	m_scrollLayer->m_contentLayer->setContentSize(ccp(m_scrollLayer->m_contentLayer->getContentSize().width, (CELL_HEIGHT * fetchedIcons.size())));
	m_scrollLayer->moveToTop();

	m_prevBtn->setVisible(m_page > 0);
	m_nextBtn->setVisible(m_page < m_maxPage);
}

void GalleryLayer::refreshGallery()
{
	fetchGallery();
}

void GalleryLayer::createModeButton(int tag, bool active)
{
	std::vector<const char *> sprites = {
		"GamemodeAll.png"_spr,
		"GamemodeCube.png"_spr,
		"GamemodeShip.png"_spr,
		"GamemodeBall.png"_spr,
		"GamemodeUFO.png"_spr,
		"GamemodeWave.png"_spr,
		"GamemodeRobot.png"_spr,
		"GamemodeSpider.png"_spr,
		"GamemodeSwing.png"_spr,
		"GamemodeJetpack.png"_spr,
	};

	//  Base color based on whenever the current page is on
	auto buttonColor = (active) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
	auto buttonSpr = IconSelectButtonSprite::createWithSprite(sprites[tag], 1.75f, buttonColor);
	buttonSpr->setScale(0.9f);

	//  Button
	auto button = CCMenuItemSpriteExtra::create(
		buttonSpr,
		this,
		menu_selector(GalleryLayer::onNavButton));
	button->setID(fmt::format("gamemode-button-{:02}", tag + 1));
	button->setTag(tag);

	//  Adds button to menu and updates layout.
	m_modesMenu->addChild(button);
	m_modesMenu->updateLayout();
};

void GalleryLayer::onNavButton(CCObject *sender)
{
	auto tag = sender->getTag();
	auto m_prevBtn = m_activeBtn;
	m_activeBtn = tag;

	//  Updates the sprite of the button of the previous page
	if (auto oldNavButton = static_cast<CCMenuItemSpriteExtra *>(m_modesMenu->getChildByTag(m_prevBtn)))
	{
		static_cast<CCSprite *>(oldNavButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Unselected.png"));
		oldNavButton->updateSprite();
		m_modesMenu->updateLayout();
	}

	//  Updates the sprite of the button for the current page
	if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(sender))
	{
		static_cast<CCSprite *>(navButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Selected.png"));
		navButton->updateSprite();
		m_modesMenu->updateLayout();
	}

	m_isFilterActive = tag != 0;
	m_mode = tag != 0 ? IconType{tag - 1} : IconType::Item;
	m_page = 0;

	fetchGallery();
}

void GalleryLayer::onPage(CCObject *sender)
{
	m_page += sender->getTag();
	fetchGallery();
}

void GalleryLayer::onFindPage(CCObject *)
{
	log::debug("Page = {} - Max Page = {}", m_page, m_maxPage);

	auto popup = SetIDPopup::create(m_page + 1, 1, m_maxPage + 1, "Go to page", "Go", true, 1, 0, false, true);
	popup->setTag(3);

	popup->m_delegate = this;
	popup->show();
};

void GalleryLayer::setIDPopupClosed(SetIDPopup *popup, int value)
{
	if (!popup || popup->m_cancelled)
		return;

	log::debug("Changed Page = {}", value);

	int newPage = value;
	if (m_page == newPage - 1)
		return;

	m_page = newPage - 1;
	fetchGallery();
};

void GalleryLayer::onSettings(CCObject *)
{
	geode::openSettingsPopup(Mod::get());
}

void GalleryLayer::onFolder(CCObject *)
{
	utils::file::openFolder(Mod::get()->getConfigDir());
}

void GalleryLayer::onBack(CCObject *)
{
	keyBackClicked();
};

void GalleryLayer::keyBackClicked()
{
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
};

GalleryLayer *GalleryLayer::create()
{
	auto ret = new GalleryLayer();

	if (ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
};

CCScene *GalleryLayer::scene()
{
	auto layer = GalleryLayer::create();
	auto scene = CCScene::create();
	scene->addChild(layer);
	return scene;
};