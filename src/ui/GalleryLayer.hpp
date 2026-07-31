#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Mod.hpp>
using namespace geode::prelude;

class GalleryLayer : public CCLayer
{
protected:
	CCSprite *m_background = nullptr;
	CCSprite *m_title = nullptr;
	NineSlice *m_frame = nullptr;
	ScrollLayer *m_scrollLayer = nullptr;
	LoadingCircleSprite *m_loading = nullptr;

	CCMenu *m_navMenu = nullptr;

	virtual bool init();
	virtual void keyBackClicked();
	void onBack(CCObject *);
	void onFolder(CCObject *);

	void createNavButton(int tag, bool active = 0);
	void onNavButton(CCObject *sender);

	//	Webrequests
	TaskHolder<web::WebResponse> m_indexListener;
	void fetchGalleryIndex();
	void loadIndex(int page = 0, IconType gamemode = IconType::Item);
	matjson::Value m_fetchedData;

public:
	unsigned int m_activeBtn = 0;
	bool m_activeFilter = false;

	static GalleryLayer *create();
	CCScene *scene();
};