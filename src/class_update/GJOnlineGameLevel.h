#pragma once
#include <Geode/Geode.hpp>

class GJOnlineGameLevel : public GJGameLevel {
public:
    static GJGameLevel* GJOnlineGameLevel::create(const int& id);
};