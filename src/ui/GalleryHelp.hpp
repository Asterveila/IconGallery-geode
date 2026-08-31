#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Mod.hpp>
using namespace geode::prelude;

class GalleryHelpPopup : public Popup
{
protected:
	MDTextArea *m_textArea = nullptr;
	CCMenu *m_selectMenu = nullptr;

	bool init();

	void onPage(CCObject *);
	void onLanguage(CCObject *);
	void setInfo();

public:
	unsigned int m_page = 0;
	bool m_spanish = false;

	static GalleryHelpPopup *create();
};