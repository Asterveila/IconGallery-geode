#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Mod.hpp>
using namespace geode::prelude;

enum GallerySort
{
	Recent,
	MostDownloaded
};

class GalleryLayer : public CCLayer, public SetIDPopupDelegate
{
protected:
	CCLabelBMFont *m_pageLabel = nullptr;

	CCMenu *m_pagesMenu = nullptr;
	CCMenu *m_modesMenu = nullptr;

	CCMenuItemSpriteExtra *m_prevBtn = nullptr;
	CCMenuItemSpriteExtra *m_nextBtn = nullptr;
	CCMenuItemSpriteExtra *m_pagesBtn = nullptr;

	LoadingCircleSprite *m_loading = nullptr;

	ScrollLayer *m_scrollLayer = nullptr;

	virtual bool init() override;
	virtual void keyBackClicked() override;

	CCSprite * getModeSprite(int tag, bool active);
	void createModeButton(int tag, bool active = 0);
	void setIDPopupClosed(SetIDPopup *popup, int value) override;

	//	Web Requesting
	TaskHolder<web::WebResponse> m_listener;
	void fetchGallery();
	void loadGallery();
	matjson::Value m_fetchedData;

	

public:
	unsigned int m_page = 0;
	unsigned int m_maxPage = 999;
	GallerySort m_sort = GallerySort::MostDownloaded;
	IconType m_mode = IconType::Item;

	unsigned int m_activeBtn = 0;
	bool m_isFilterActive = false;

	void onBack(CCObject *sender);
	void onPage(CCObject *sender);
	void onFolder(CCObject *sender);
	void onFindPage(CCObject *sender);
	void onSettings(CCObject *sender);
	void onNavButton(CCObject *sender);

	void refreshGallery();

	static GalleryLayer *create();
	CCScene *scene();
};