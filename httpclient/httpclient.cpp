#include "httpclient.h"
#include <httplib.h>
#include <Geode/Geode.hpp>
#include <sstream>

namespace httpclient {

    ParsedResponse parseResponse(const std::string& body) {
        ParsedResponse result;

        // Split body into sections by '#'
        // Format: <levels>#<creators>#<songs>#<page_info>#<hash>
        std::vector<std::string> sections;
        std::istringstream section_stream(body);
        std::string section;
        while (std::getline(section_stream, section, '#')) {
            sections.push_back(section);
        }

        // --- Parse level fields (section 0) ---
        if (!sections.empty()) {
            // Take only the first level (before '|')
            std::string firstLevel = sections[0].substr(0, sections[0].find('|'));

            std::istringstream stream(firstLevel);
            std::string key, value;
            while (std::getline(stream, key, ':') && std::getline(stream, value, ':')) {
                result.level_fields[key] = value;
            }
        }

        // --- Parse creators section (section 1) ---
        // Format: userID:username:accountID (multiple creators separated by '|')
        // We match by userID found in level field "6"
        if (sections.size() > 1 && !sections[1].empty()) {
            std::string level_user_id = "";
            auto it = result.level_fields.find("6");
            if (it != result.level_fields.end()) {
                level_user_id = it->second;
            }

            std::istringstream creators_stream(sections[1]);
            std::string creator_entry;
            while (std::getline(creators_stream, creator_entry, '|')) {
                // Each entry: userID:username:accountID
                std::vector<std::string> parts;
                std::istringstream entry_stream(creator_entry);
                std::string part;
                while (std::getline(entry_stream, part, ':')) {
                    parts.push_back(part);
                }

                // parts[0] = userID, parts[1] = username, parts[2] = accountID
                if (parts.size() >= 2) {
                    if (level_user_id.empty() || parts[0] == level_user_id) {
                        result.author_name = parts[1];
                        break;
                    }
                }
            }
        }

        return result;
    }

    BoomlingsLevel::~BoomlingsLevel() {
        if (gclient.joinable()) gclient.join();
    }

    void BoomlingsLevel::fetch(std::string levelId, std::function<void()> callback) {
        if (gclient.joinable()) gclient.join();

        this->level_id = levelId;
        this->ready = false;
        this->on_ready = callback;

        this->gclient = std::thread([this, levelId]() {
            try {
                httplib::Client cli("www.boomlings.com", 80);
                cli.set_default_headers({ { "User-Agent", "" } });
                cli.set_connection_timeout(10, 0);
                cli.set_read_timeout(10, 0);

                httplib::Params params = {
                    { "gameVersion",   "22"          },
                    { "binaryVersion", "42"          },
                    { "gdw",           "0"           },
                    { "str",           levelId       },
                    { "type",          "19"          },
                    { "secret",        "Wmfd2893gb7" }
                };

                auto res = cli.Post("/database/getGJLevels21.php", params);

                {
                    std::lock_guard<std::mutex> lock(this->fields_mutex);
                    if (res && res->status == 200 && res->body != "-1") {
                        auto parsed = parseResponse(res->body);
                        this->fields = parsed.level_fields;
                        this->author_name = parsed.author_name;
                    } else {
                        this->fields.clear();
                        this->author_name = "-1";
                    }
                }
            } catch (const std::exception& e) {
                geode::log::error("HTTP fetch failed: {}", e.what());
                std::lock_guard<std::mutex> lock(this->fields_mutex);
                this->fields.clear();
                this->author_name = "-1";
            }

            this->ready = true;

            if (this->on_ready) {
                this->on_ready();
            }
        });
    }

    bool BoomlingsLevel::is_ready() const {
        return this->ready.load();
    }

    std::string BoomlingsLevel::get_result(const FieldType& field) const {
        std::lock_guard<std::mutex> lock(this->fields_mutex);
        auto it = this->fields.find(std::to_string(static_cast<int>(field)));
        if (it != this->fields.end()) return it->second;
        return "-1";
    }

    std::string BoomlingsLevel::get_author() const {
        std::lock_guard<std::mutex> lock(this->fields_mutex);
        return author_name.empty() ? "-1" : author_name;
    }
    std::string BoomlingsLevel::get_song_id() const {
        std::lock_guard<std::mutex> lock(this->fields_mutex);

        // Field 35 = custom/Newgrounds song ID (non-zero means a custom song)
        auto custom_it = this->fields.find("35");
        if (custom_it != this->fields.end() && !custom_it->second.empty()
            && custom_it->second != "0") {
            return custom_it->second;
            }

        // Field 12 = official in-game song index (0–21 etc.)
        auto official_it = this->fields.find("12");
        if (official_it != this->fields.end() && !official_it->second.empty()) {
            return official_it->second;
        }

        return "-1";
    }
}
