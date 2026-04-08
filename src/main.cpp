#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
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
		auto searchObj = GJSearchObject::create(SearchType::Search, std::to_string(90475473));


		auto scene = LevelBrowserLayer::scene(searchObj);
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
	}
};