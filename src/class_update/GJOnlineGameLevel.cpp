#include "GJOnlineGameLevel.h"
#include <Geode/Geode.hpp>
#include <httpclient/httpclient.h>
#include <thread>
#include <chrono>

GJOnlineGameLevel* GJOnlineGameLevel::createWithLevelID(const int& id) {
    auto level = new GJOnlineGameLevel();
    
    const auto str_id = std::to_string(id);
    httpclient::fetch(str_id, httpclient::FieldType::LevelID);
    httpclient::fetch(str_id, httpclient::FieldType::LevelName);
    
    // Wait for results (with timeout to prevent infinite loop)
    int timeout = 100; // 10 seconds max
    while (timeout > 0) {
        auto id_result = httpclient::get_result(str_id, httpclient::FieldType::LevelID);
        if (id_result != "-1") break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        timeout--;
    }
    
    if (level->init()) {
        level->m_levelID = id;
        level->m_levelName = httpclient::get_result(str_id, httpclient::FieldType::LevelName);
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}