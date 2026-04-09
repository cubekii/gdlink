#include "searchtab.h"
#include <Geode/Geode.hpp>
using namespace cocos2d;

void gdloader::searchtab(const gd::string& searchQuery) {
    auto searchObj = GJSearchObject::create(SearchType::Search, searchQuery);

    auto scene = LevelBrowserLayer::scene(searchObj);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}

void gdloader::loadlevel(const int& levelID) {
    auto level = GJGameLevel::create();
    level->m_levelID = levelID;
    level->m_creatorName = "Cubekii";
    level->m_levelName = "It's my level";
    level->m_dislikes = 999999;

    auto scene = LevelInfoLayer::scene(level, false);
    CCDirector::sharedDirector()->pushScene(
        CCTransitionFade::create(0.5f, scene)
    );
}
