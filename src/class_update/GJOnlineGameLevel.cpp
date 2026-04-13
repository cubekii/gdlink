#include "GJOnlineGameLevel.h"
#include <Geode/Geode.hpp>
#include <httpclient/httpclient.h>

GJOnlineGameLevel* GJOnlineGameLevel::createWithLevelID(const int& id) {
    auto level = new GJOnlineGameLevel();
    const auto str_id = std::to_string(id);
    if (level->init()) {
        httpclient::fetch(str_id, httpclient::FieldType::LevelName);
        httpclient::get_result(str_id, httpclient::FieldType::LevelName);
        level->m_levelID = id;
        level->m_levelName = httpclient::get_result(str_id, httpclient::FieldType::LevelName);
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}