#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include "ui/GalleryLayer.hpp"

using namespace geode::prelude;

class $modify(GarageLayer, GJGarageLayer)
{
	bool init()
	{
		if (!GJGarageLayer::init())
			return false;

		auto menu = this->getChildByID("shards-menu");

		auto spr = CircleButtonSprite::createWithSprite("dialogIcon_028.png", 1, CircleBaseColor::DarkPurple, CircleBaseSize::Small);
		auto button = CCMenuItemSpriteExtra::create(
			spr,
			this,
			menu_selector(GarageLayer::onIconGallery));
		button->setID("Icon-Gallery-Button");

		menu->addChild(button);
		menu->updateLayout();

		return true;
	}

	void onIconGallery(CCObject *)
	{
		auto scene = CCScene::create();
		scene->addChild(GalleryLayer::create());

		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
	}
};

$execute{
	listenForAllSettingChanges([](const std::string_view key, std::shared_ptr<SettingV3> setting){
		if(auto layer = static_cast<GalleryLayer *>(CCScene::get()->getChildByID("icon-gallery-layer"))){
			layer->refreshGallery();
		};
	});
};