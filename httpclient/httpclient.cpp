#include "httpclient.h"
#include "httplib.h"

HttpClient::HttpClient() {}

HttpClient::~HttpClient() {
    if (gclient.joinable())
        gclient.join();
}

void HttpClient::fetch_level(const std::string &level_id) {

    if (gclient.joinable())
        gclient.join();

    gclient = std::thread([this, level_id]() {
        httplib::Client cli(this->host,80);
        cli.set_default_headers({ { "User-Agent", "" } });
        cli.set_connection_timeout(10, 0);
        cli.set_read_timeout(10, 0);

        httplib::Params params = {
            { "gameVersion",   "22"          },
            { "binaryVersion", "42"          },
            { "gdw",           "0"           },
            { "str",           level_id      },
            { "type",          "19"          },
            { "secret",        "Wmfd2893gb7" }
        };
        auto res = cli.Post("/database/getGJLevels21.php", params);
        if (res && res->status == 200) {
            std::lock_guard<std::mutex> lock(glock);
            fields = res->body;
            parseResponse();
        }

        ready.store(true);
    });
}

void HttpClient::parseResponse() {
    if (fields.empty()) return;

    // Split the response into its 3 pipe-separated sections
    // Section 0: level data (key:value:key:value...)
    // Section 1: creator info
    // Section 2: song info (unused here)
    std::vector<std::string> sections;
    std::stringstream ss(fields);
    std::string section;
    while (std::getline(ss, section, '|'))
        sections.push_back(section);

    // --- Parse level fields (section 0) ---
    // Format: "1:LevelName:2:12345:5:3:..."
    if (!sections.empty()) {
        std::stringstream ls(sections[0]);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ls, token, ':'))
            tokens.push_back(token);

        // Pair up key:value
        for (size_t i = 0; i + 1 < tokens.size(); i += 2)
            lvl.level_fields[tokens[i]] = tokens[i + 1];
    }

    if (sections.size() > 1) {
        std::stringstream cs(sections[1]);
        std::string part;
        std::vector<std::string> creator_parts;

        while (std::getline(cs, part, ':'))
            creator_parts.push_back(part);

        // Index 1 is the username
        if (creator_parts.size() >= 2)
            lvl.author_name = creator_parts[1];
    }
}

std::string HttpClient::get_creator() {
    if (gclient.joinable())
        gclient.join();
    return lvl.author_name;
}

std::string HttpClient::get_level_name() {
    if (gclient.joinable())
        gclient.join();
    return lvl.level_fields["2"];
}

std::string HttpClient::get_song_id() {
    if (gclient.joinable())
        gclient.join();
    auto custom = lvl.level_fields.find("35");
    if (custom != lvl.level_fields.end() && custom->second != "0")
        return custom->second; // Newgrounds ID

    auto official = lvl.level_fields.find("12");
    if (official != lvl.level_fields.end())
        return official->second; // official track index

    return "";
}
