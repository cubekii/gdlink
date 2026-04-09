#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "openurl/searchtab.h"
#include <Geode/loader/Dirs.hpp>
#include <url.h>

using namespace geode::prelude;

std::filesystem::path gdDir = dirs::getGameDir();
#ifdef GEODE_IS_WINDOWS
	auto exePath = gdDir / "GeometryDash.exe";
#elif defined(GEODE_IS_MACOS)
	auto exePath = gdDir; // on macOS, getGameDir() returns the .app bundle itself
#endif


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
		//gdloader::searchtab("90475473");
		//CustomUrl example("gdlink",exePath.string());
		//example.sendNotification("example","notification");
		gdloader::loadlevel(90475473);
	}
};