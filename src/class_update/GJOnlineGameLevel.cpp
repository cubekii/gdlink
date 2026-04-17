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
        level->m_levelName = fetch_level.get_level_name();
        level->m_creatorName = fetch_level.get_creator();
        level->m_accountID = fetch_level.is_creator_known();
        level->m_songID = std::stoi(fetch_level.get_song_id());
        return level;
    }
    return nullptr;
}