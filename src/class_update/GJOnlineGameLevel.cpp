#include "GJOnlineGameLevel.h"
#include <Geode/Geode.hpp>
#include <httpclient/httpclient.h>
#include <thread>
#include <chrono>

HttpClient fetch_level;

GJOnlineGameLevel* GJOnlineGameLevel::createWithLevelID(const int& id) {
    auto level = new GJOnlineGameLevel();
    //Fetchlevel.fetch(std::to_string(id));

    level->m_levelID = id;
    //while (!Fetchlevel.is_ready()) {}
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
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}