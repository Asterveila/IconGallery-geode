#pragma once
#include "GalleryObject.hpp"
using namespace geode::prelude;

class GalleryCell : public CCLayer
{
protected:
	GalleryObject *m_data;
	CCLayerColor *m_background;
	CCSize m_size = {356.0f, 80.0f};

	bool init(GalleryObject *data, bool even);

	void onDownload(CCObject *);

public:
	static GalleryCell *create(GalleryObject *data, bool even);

	CCLabelBMFont *getGamemodeLabel(std::string label);
};