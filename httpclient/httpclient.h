#pragma once

#include <httplib.h>
#include <thread>
#include <string>

struct ParsedResponse {
    std::unordered_map<std::string,std::string> level_fields;  // key:value pairs from level data
    std::string author_name;    // parsed from creators section
};

class HttpClient {
private:
    const std::string host = "www.boomlings.com";
    std::string fields;
    std::thread gclient;
    mutable std::mutex glock;

    void fetch_level(const std::string& level_id);
    ParsedResponse parseResponse();
public:
    HttpClient();
    std::string get_user_name();
    std::string get_level_name();
    std::string get_song_id();
};