#pragma once
#include <Geode/Geode.hpp>

class GJOnlineGameLevel : public GJGameLevel {
public:
    static GJOnlineGameLevel* createWithLevelID(const int& id);
};