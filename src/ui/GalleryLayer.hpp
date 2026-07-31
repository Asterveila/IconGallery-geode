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

	virtual bool init();
	virtual void keyBackClicked();
	void onBack(CCObject *);

	void onFolder(CCObject *);

	LoadingCircleSprite *m_loading;

	//	Online Features
	TaskHolder<web::WebResponse> m_listener;
	void fetchGalleryData();

	matjson::Value m_fetchedData;
	void loadIcons();

	//	Test
	TaskHolder<web::WebResponse> m_getIconsListener;
	TaskHolder<web::WebResponse> m_getCountListener;
	void fetchDataTest();

public:
	static GalleryLayer *create();
	CCScene *scene();
};