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

    // Split by '#' not '|'
    std::vector<std::string> sections;
    std::stringstream ss(fields);
    std::string section;
    while (std::getline(ss, section, '#'))
        sections.push_back(section);

    // --- Parse level fields (section 0) ---
    if (!sections.empty()) {
        std::stringstream ls(sections[0]);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ls, token, ':'))
            tokens.push_back(token);

        for (size_t i = 0; i + 1 < tokens.size(); i += 2)
            lvl.level_fields[tokens[i]] = tokens[i + 1];
    }

    // --- Parse creator (section 1) ---
    // Format: "userID:username:accountID"
    if (sections.size() > 1) {
        std::stringstream cs(sections[1]);
        std::string part;
        std::vector<std::string> creator_parts;

        while (std::getline(cs, part, ':'))
            creator_parts.push_back(part);

        if (creator_parts.size() >= 2)
            lvl.author_name = creator_parts[1]; // index 1 is username
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
    return (lvl.level_fields["35"]!="0" ? lvl.level_fields["35"] : lvl.level_fields["12"]);
}
