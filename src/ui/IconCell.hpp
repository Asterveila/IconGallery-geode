#pragma once
#include "../nodes/Icon.hpp"
using namespace geode::prelude;

class IconCell : public CCLayer
{
protected:
	Icon *m_icon;
	CCLayerColor *m_background;
	CCSize m_size = {356.0f, 70.0f};

	bool init(Icon *data, bool even);
	CCLabelBMFont *getGamemodeLabel(IconType gamemode);

public:
	CCMenuItemSpriteExtra *m_downloadBtn = nullptr;

	static IconCell *create(Icon *data, bool even);

	void onDownload(CCObject *);
	void onInfo(CCObject *);
	void updateDownload();
};