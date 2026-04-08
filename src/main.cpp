#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "openurl/searchtab.h"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {

		if (!MenuLayer::init()) {
			return false;
		}

		log::debug("Hello from my MenuLayer::init hook! This layer has {} children.", this->getChildrenCount());

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		return true;
	}

	void onMyButton(CCObject*) {
		gdloader::searchtab("90475473");
	}
};