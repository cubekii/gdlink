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
    ParsedResponse lvl;
    const std::string host = "www.boomlings.com";
    std::string fields;
    std::thread gclient;
    mutable std::mutex glock;
    std::atomic<bool> ready = false;

    void parseResponse();
public:
    HttpClient();
    ~HttpClient();
    void fetch_level(const std::string& level_id);
    std::string get_creator();
    std::string get_level_name();
    std::string get_song_id();
};