#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "openurl/searchtab.h"
#include <Geode/loader/Dirs.hpp>
#include <url.h>

using namespace geode::prelude;

std::optional<std::string> pendingUrl = std::nullopt;

$on_mod(Loaded) {
	std::string cmdLine = GetCommandLineA();
	log::info("Full command line: {}", cmdLine);

	// find gdlink:// in it
	size_t pos = cmdLine.find("gdlink://");
	if (pos != std::string::npos) {
		pendingUrl = cmdLine.substr(pos);
		// trim trailing quote if present: "gdlink://level/42"
		if (!pendingUrl->empty() && pendingUrl->back() == '"')
			pendingUrl->pop_back();
	}
}
class $modify(LoadingLayer) {
	void loadAssets() {
		LoadingLayer::loadAssets(); // call original first

		if (m_loadStep >= 14) { // 14 = last step in GD 2.2
			//gdloader::loadlevel(90475473);

			if (!pendingUrl->empty())
				Notification::create(pendingUrl->c_str())->show();
			else
				Notification::create("it's not working!")->show();
		}
	}
};
