#include "GJOnlineGameLevel.h"
#include <Geode/Geode.hpp>

GJGameLevel* GJOnlineGameLevel::create(const int& id) {
    auto level = GJGameLevel::create();
    level->m_levelID = id;
    return level;
}