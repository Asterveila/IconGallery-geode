#include "GalleryCell.hpp"

bool GalleryCell::init(GalleryObject *data, bool even)
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setContentSize(m_size);
	this->m_data = data;

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

	//	Icon
	auto GM = GameManager::sharedState();
	auto iconSpr = GJItemIcon::createBrowserItem(
		GM->iconTypeToUnlockType(data->m_gamemode),
		1);
	this->addChildAtPosition(iconSpr, Anchor::Left, ccp(40, 00), false);

	//	Name of the Icon
	auto m_iconName = CCLabelBMFont::create(data->m_iconName.c_str(), "bigFont.fnt");
	m_iconName->setScale(0.75f);
	m_iconName->setAnchorPoint({0, 0.5});
	addChildAtPosition(m_iconName, Anchor::Left, ccp(80, 25), false);

	//	Author of the Icon
	auto m_iconAuthor = CCLabelBMFont::create(fmt::format("By {}", data->m_iconAuthor).c_str(), "goldFont.fnt");
	m_iconAuthor->setScale(0.5f);
	m_iconAuthor->setAnchorPoint({0, 0.5});
	addChildAtPosition(m_iconAuthor, Anchor::Left, ccp(80, 5), false);

	//	Description
	auto m_iconDesc = CCLabelBMFont::create(data->m_description.c_str(), "chatFont.fnt", 320.0f);
	m_iconDesc->setAnchorPoint({0, 0.5});
	m_iconDesc->setScale(0.5f);
	addChildAtPosition(m_iconDesc, Anchor::Left, ccp(80, -10), false);

	auto m_downloadCount = CCLabelBMFont::create(fmt::format("{}", data->m_downloads).c_str(), "bigFont.fnt");
	m_downloadCount->setScale(0.25);
	m_downloadCount->setAnchorPoint({0, 0.5});
	m_downloadCount->setColor({100, 255, 100});
	addChildAtPosition(m_downloadCount, Anchor::Center, ccp(0, -25), false);

	auto m_downloadIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	m_downloadIcon->setScale(0.25);
	m_downloadIcon->setAnchorPoint({1, 0.5});
	// m_downloadIcon->setPosition(ccp(m_downloadCount->getPosition().x - .3f, 11));
	addChildAtPosition(m_downloadIcon, Anchor::Center, ccp(0, -25), false);

	return true;
};

GalleryCell *GalleryCell::create(GalleryObject *data, bool even)
{
	{
		auto ret = new GalleryCell();

		if (ret->init(data, even))
		{
			ret->autorelease();
			return ret;
		}

		delete ret;
		return nullptr;
	}
}