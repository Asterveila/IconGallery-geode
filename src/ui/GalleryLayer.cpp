#include "GalleryLayer.hpp"
#include "../nodes/GalleryObject.hpp"
#include "../nodes/GalleryCell.hpp"

const int CELL_HEIGHT = 80;

bool GalleryLayer::init()
{
	if (!CCLayer::init())
		return false;

	fetchGalleryData();

	auto SFC = CCSpriteFrameCache::get();
	SFC->addSpriteFramesWithFile("GJ_ShopSheet01.plist");

	m_background = CCSprite::createWithSpriteFrameName("shopBG_001.png");
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto size = m_background->getContentSize();

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
	auto menuBack = CCMenu::create();
	menuBack->setID("back-menu"_spr);
	addChildAtPosition(menuBack, Anchor::TopLeft, ccp(24, -24), false);

	auto backBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(GalleryLayer::onBack));
	backBtn->setID("back-button"_spr);
	backBtn->setSizeMult(1.2f);
	menuBack->addChild(backBtn);

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

	m_loading = LoadingCircleSprite::create(1);
	m_loading->setID("loading");
	m_loading->setVisible(false);
	this->addChildAtPosition(m_loading, Anchor::Center, ccp(0, 0), false);

	setKeyboardEnabled(true);
	setKeypadEnabled(true);
	return true;
};

void GalleryLayer::fetchGalleryData()
{
	if (m_loading)
		m_loading->setVisible(true);

	auto req = web::WebRequest();

	m_listener.spawn(
		req.get("https://raw.githubusercontent.com/MasterL500/GD-Mods-Data/main/TestGallery.json"),
		[this](web::WebResponse res)
		{
			if (res.ok() && res.json().isOk())
			{
				Notification::create("Data Loaded", NotificationIcon::Success)->show();
				m_fetchedData = res.json().unwrap();
				m_loading->setVisible(false);
				loadIcons();
			}
			else
			{
				Notification::create("There was an error", NotificationIcon::Error)->show();
				log::error("Failed on loading data");
			}
		});
};

void GalleryLayer::loadIcons()
{
	std::vector<GalleryObject *> icons = {};
	int ii = 0;

	//	For Loop to add the icons
	for (auto &value : m_fetchedData)
	{
		if(ii >= 10) break;

		auto iconObject = value;

		GalleryObject *iconData = GalleryObject::create(
			iconObject["name"].asString().unwrapOr("Unnamed"),
			iconObject["author"].asString().unwrapOr("John Doe"),
			iconObject["description"].asString().unwrap(),
			iconObject["gamemode"].asString().unwrap(),
			iconObject["downloads"].asInt().unwrapOr(0),
			iconObject["isVanilla"].asBool().unwrapOr(false),
			iconObject["hasProjectFiles"].asBool().unwrapOr(false));

		icons.push_back(iconData);

		GalleryCell *cell = GalleryCell::create(iconData, ii % 2 == 0);
		m_scrollLayer->m_contentLayer->addChild(cell);
		cell->setPosition(0, (CELL_HEIGHT * 10) - CELL_HEIGHT * (ii + 1));
		ii++;
	};

	//	Fixes the scroll layer
	int iconCount = m_scrollLayer->m_contentLayer->getChildrenCount();
	m_scrollLayer->m_contentLayer->setContentSize(ccp(m_scrollLayer->m_contentLayer->getContentSize().width, (CELL_HEIGHT * 10)));
	m_scrollLayer->moveToTop();
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