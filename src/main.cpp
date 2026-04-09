#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "openurl/searchtab.h"
#include <Geode/loader/Dirs.hpp>
#include <url.h>

using namespace geode::prelude;

$on_mod(Loaded) {
	CustomUrl gdlink("gdlink");
}
class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		log::debug("success");
		return true;
	}
};