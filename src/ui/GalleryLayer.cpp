#include "GalleryLayer.hpp"
#include "../nodes/GalleryObject.hpp"
#include "../nodes/GalleryCell.hpp"

const int CELL_HEIGHT = 80;

bool GalleryLayer::init()
{
	if (!CCLayer::init())
		return false;

	fetchGalleryIndex();

	m_background = CCSprite::create("background.png"_spr);
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	// Background
	m_background->setPosition(winSize / 2);
	m_background->setScaleY(winSize.height / m_background->getContentSize().height);
	m_background->setScaleX(winSize.width / m_background->getContentSize().width);
	addChild(m_background, -2);

	//	Frame
	m_frame = NineSlice::create("Frame.png"_spr);
	m_frame->setContentSize({400, 256});
	addChildAtPosition(m_frame, Anchor::Center, ccp(0, 0), false);

	//	Title
	m_title = CCSprite::create("GalleryLabel.png"_spr);
	addChildAtPosition(m_title, Anchor::Top, ccp(0, -30), false);

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

	//	Button Menu
	auto buttonMenu = CCMenu::create();
	buttonMenu->setID("button-menu");
	addChildAtPosition(buttonMenu, Anchor::BottomLeft, ccp(0, 0), false);

	//	Folder Button
	auto folderBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
		this,
		menu_selector(GalleryLayer::onFolder));
	buttonMenu->addChildAtPosition(folderBtn, Anchor::BottomRight, ccp(-30, 30), false);

	m_navMenu = CCMenu::create();
	m_navMenu->setID("navigation-menu");
	m_navMenu->setLayout(RowLayout::create()
							 ->setGap(2.f));
	addChildAtPosition(m_navMenu, Anchor::Bottom, ccp(0, 30), false);

	for (int ii = 0; ii < 10; ii++)
		createNavButton(ii, ii == 0);

	//	Scroll Layer
	m_scrollLayer = ScrollLayer::create({356, 220});
	m_scrollLayer->setZOrder(-2);
	this->addChildAtPosition(m_scrollLayer, Anchor::Center, ccp(-178, -110), false);

	auto m_scrollBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
	m_scrollBG->setContentSize(m_scrollLayer->getContentSize());
	m_scrollBG->setColor({0, 0, 0});
	m_scrollBG->setOpacity(120);
	m_scrollBG->setZOrder(-2);
	m_scrollLayer->addChildAtPosition(m_scrollBG, Anchor::Center, ccp(0, 0), false);

	//	Loading Circle
	m_loading = LoadingCircleSprite::create(1);
	m_loading->setID("loading");
	m_loading->setVisible(false);
	this->addChildAtPosition(m_loading, Anchor::Center, ccp(0, 0), false);

	setKeyboardEnabled(true);
	setKeypadEnabled(true);
	return true;
};

void GalleryLayer::fetchGalleryIndex()
{
	if (m_loading)
		m_loading->setVisible(true);

	auto req = web::WebRequest();

	m_indexListener.spawn(
		req.get("https://raw.githubusercontent.com/MasterL500/GD-Mods-Data/main/TestGallery.json"),
		[this](web::WebResponse res)
		{
			if (res.ok() && res.json().isOk())
			{
				Notification::create("Data Loaded", NotificationIcon::Success)->show();
				m_fetchedData = res.json().unwrap();
				m_loading->setVisible(false);
				loadIndex();
			}
			else
			{
				Notification::create("There was an error", NotificationIcon::Error)->show();
				log::error("Failed on loading data");
			}
		});
};

void GalleryLayer::loadIndex(int page, IconType type)
{
	std::vector<GalleryObject *> icons = {};

	if (m_scrollLayer && m_scrollLayer->m_contentLayer->getChildrenCount() > 0)
		m_scrollLayer->m_contentLayer->removeAllChildren();

	for (auto &value : m_fetchedData)
	{
		auto iconObject = value;

		GalleryObject *iconData = GalleryObject::create(
			iconObject["name"].asString().unwrapOr("Unnamed"),
			iconObject["author"].asString().unwrapOr("John Doe"),
			iconObject["file"].asString().unwrap(),
			iconObject["description"].asString().unwrap(),
			iconObject["gamemode"].asString().unwrap(),
			iconObject["downloads"].asInt().unwrapOr(0),
			iconObject["isVanilla"].asBool().unwrapOr(false),
			iconObject["hasProjectFiles"].asBool().unwrapOr(false));

		icons.push_back(iconData);
	}

	std::vector<GalleryObject *> filtered = icons;
	log::debug("Count {}", icons.size());

	if (m_activeFilter)
	{
		filtered = utils::ranges::filter(icons, [type](GalleryObject *icon)
										 { return icon->m_gamemode == type; });

		log::debug("Filtered Count {}", filtered.size());
	}

	int count = 0;
	int ii = 0;

	for (auto &value : filtered)
	{
		auto iconData = value;

		GalleryCell *cell = GalleryCell::create(iconData, ii % 2 == 0);
		m_scrollLayer->m_contentLayer->addChild(cell);
		cell->setPosition(0, (CELL_HEIGHT * 10) - CELL_HEIGHT * (ii + 1));
		ii++;
	}

	//	Fixes the scroll layer
	int iconCount = m_scrollLayer->m_contentLayer->getChildrenCount();
	m_scrollLayer->m_contentLayer->setContentSize(ccp(m_scrollLayer->m_contentLayer->getContentSize().width, (CELL_HEIGHT * 10)));
	m_scrollLayer->moveToTop();
}

void GalleryLayer::createNavButton(int tag, bool active)
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
	button->setID(fmt::format("gamemode-btn-{:02}", tag + 1));
	button->setTag(tag);

	//  Adds button to menu and updates layout.
	m_navMenu->addChild(button);
	m_navMenu->updateLayout();
};

void GalleryLayer::onNavButton(CCObject *sender)
{
	auto tag = sender->getTag();
	auto m_prevBtn = m_activeBtn;
	m_activeBtn = tag;

	//  Updates the sprite of the button of the previous page
	if (auto oldNavButton = static_cast<CCMenuItemSpriteExtra *>(m_navMenu->getChildByTag(m_prevBtn)))
	{
		static_cast<CCSprite *>(oldNavButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Unselected.png"));
		oldNavButton->updateSprite();
		m_navMenu->updateLayout();
	}

	//  Updates the sprite of the button for the current page
	if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(sender))
	{
		static_cast<CCSprite *>(navButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Selected.png"));
		navButton->updateSprite();
		m_navMenu->updateLayout();
	}

	m_activeFilter = tag != 0;

	if (m_activeFilter)
	{
		loadIndex(0, IconType{tag - 1});
	}
	else
	{
		loadIndex(0);
	}
}

void GalleryLayer::onFolder(CCObject *)
{
	utils::file::openFolder(Mod::get()->getConfigDir());
};

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