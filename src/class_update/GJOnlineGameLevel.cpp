#include "GJOnlineGameLevel.h"
#include <Geode/Geode.hpp>

GJOnlineGameLevel* GJOnlineGameLevel::createWithLevelID(const int& id) {
    auto level = new GJOnlineGameLevel();
    if (level->init()) {
        level->m_levelID = id;
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}