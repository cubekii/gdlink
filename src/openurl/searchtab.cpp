#include "searchtab.h"
#include <Geode/Geode.hpp>
using namespace cocos2d;

void gdloader::searchtab(const gd::string& searchQuery) {
    auto searchObj = GJSearchObject::create(SearchType::Search, searchQuery);

    auto scene = LevelBrowserLayer::scene(searchObj);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}