#include "GJOnlineGameLevel.h"
#include <Geode/Geode.hpp>
#include <httpclient/httpclient.h>
#include <thread>
#include <chrono>


GJOnlineGameLevel* GJOnlineGameLevel::createWithLevelID(const int& id) {
    auto level = new GJOnlineGameLevel();
    HttpClient fetch_level;
    fetch_level.fetch_level(std::to_string(id));

    level->m_levelID = id;
    if (level->init()) {
        level->m_levelID = id;
        level->autorelease();
        /*
        level->m_levelName = Fetchlevel.get_result(httpclient::FieldType::LevelName);
        level->m_creatorName = Fetchlevel.get_author();
        level->m_songID = std::stoi(Fetchlevel.get_song_id());
        if (Fetchlevel.get_author()!="-1") //strange way to verify account :)
            level->m_accountID = 1;
        */
        level->m_levelName = fetch_level.get_level_name();
        level->m_creatorName = fetch_level.get_creator();
        level->m_songID = std::stoi(fetch_level.get_song_id());
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}