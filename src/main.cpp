#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "openurl/searchtab.h"
#include <Geode/loader/Dirs.hpp>
#include <url.h>
#include "../httpserv/httpserv.h"

using namespace geode::prelude;


$on_mod(Loaded) {
	//CustomUrl gdlink("gdlink");
}
class $modify(LoadingLayer) {
	void loadAssets() {
		LoadingLayer::loadAssets(); // call original first

		if (m_loadStep >= 14) {
			httpserv::startserver();
		}
	}
};
