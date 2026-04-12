#include "searchtab.h"
#include <Geode/Geode.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>
using namespace cocos2d;

void gdloader::searchtab(const gd::string& searchQuery) {
    auto searchObj = GJSearchObject::create(SearchType::Search, searchQuery);

    auto scene = LevelBrowserLayer::scene(searchObj);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}

void gdloader::loadlevel(const int& levelID) {
    auto level = EditorIDs::getLevelByID(levelID);

    auto scene = LevelInfoLayer::scene(level, false);
    CCDirector::sharedDirector()->pushScene(
        CCTransitionFade::create(0.5f, scene)
    );
}
