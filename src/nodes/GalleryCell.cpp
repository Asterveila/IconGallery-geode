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
	this->addChildAtPosition(iconSpr, Anchor::Left, ccp(35, 0), false);

	//	Name of the Icon
	auto m_iconName = CCLabelBMFont::create(data->m_name.c_str(), "bigFont.fnt");
	m_iconName->setScale(0.65f);
	m_iconName->setAnchorPoint({0, 0.5});
	this->addChildAtPosition(m_iconName, Anchor::Left, ccp(70, 25), false);

	std::string authors = data->m_author;

	if(data->m_collaborators.size() != 0){
		if(data->m_collaborators.size() > 1){
			authors = fmt::format("By {} and More", data->m_author);
		} else {
			authors = fmt::format("By {} and {}", data->m_author, data->m_collaborators[0]);
		}
	};

	//	Author of the Icon
	auto m_iconAuthor = CCLabelBMFont::create(fmt::format("By {}", data->m_author).c_str(), "goldFont.fnt");
	m_iconAuthor->setScale(0.5f);
	m_iconAuthor->setAnchorPoint({0, 0.5});
	this->addChildAtPosition(m_iconAuthor, Anchor::Left, ccp(70, 5), false);

	//	Description
	auto m_iconDesc = CCLabelBMFont::create(data->m_description.c_str(), "chatFont.fnt", 360.0f);
	m_iconDesc->setAnchorPoint({0, 0.5});
	m_iconDesc->setScale(0.5f);
	this->addChildAtPosition(m_iconDesc, Anchor::Left, ccp(70, -10), false);

	auto m_downloadIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	m_downloadIcon->setScale(0.4f);
	m_downloadIcon->setAnchorPoint({0, 0.5});
	// m_downloadIcon->setPosition(ccp(m_downloadCount->getPosition().x - .3f, 11));
	this->addChildAtPosition(m_downloadIcon, Anchor::Left, ccp(70, -30), false);

	auto m_downloadCount = CCLabelBMFont::create(fmt::format("{}", data->m_downloads).c_str(), "bigFont.fnt");
	m_downloadCount->setScale(0.4f);
	m_downloadCount->setAnchorPoint({0, 0.5});
	m_downloadCount->setColor({100, 255, 100});
	this->addChildAtPosition(m_downloadCount, Anchor::Left, ccp(85, -30), false);

	auto m_label = getGamemodeLabel(data->m_gamemodeString);
	this->addChildAtPosition(m_label, Anchor::Left, ccp(125, -30), false);

	return true;
};

CCLabelBMFont *GalleryCell::getGamemodeLabel(std::string text)
{
	auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
	label->setScale(0.4f);
	label->setAnchorPoint({0, 0.5});

	if (utils::string::equalsIgnoreCase(text, "Cube"))
	{
		label->setColor({120, 255, 120});
	}
	else if (utils::string::equalsIgnoreCase(text, "Ship"))
	{
		label->setColor({255, 120, 255});
	}
	else if (utils::string::equalsIgnoreCase(text, "Ball"))
	{
		label->setColor({255, 120, 120});
	}

	return label;
}

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