#include "httpclient.h"
#include "httplib.h"

HttpClient::HttpClient() {}

void HttpClient::fetch_level(const std::string &level_id) {
    this->gclient = std::thread([this, level_id]() {
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
        std::lock_guard<std::mutex> lock(glock);
        this->fields = cli.Post("/database/getGJLevels21.php", params)->body;
    });
}

ParsedResponse HttpClient::parseResponse() {
        ParsedResponse result;

        std::vector<std::string> sections;
        std::istringstream section_stream(this->fields);
        std::string section;
        while (std::getline(section_stream, section, '#')) {
            sections.push_back(section);
        }

        if (!sections.empty()) {
            std::string firstLevel = sections[0].substr(0, sections[0].find('|'));

            std::istringstream stream(firstLevel);
            std::string key, value;
            while (std::getline(stream, key, ':') && std::getline(stream, value, ':')) {
                result.level_fields[key] = value;
            }
        }

        if (sections.size() > 1 && !sections[1].empty()) {
            std::string level_user_id = "";
            auto it = result.level_fields.find("6");
            if (it != result.level_fields.end()) {
                level_user_id = it->second;
            }

            std::istringstream creators_stream(sections[1]);
            std::string creator_entry;
            while (std::getline(creators_stream, creator_entry, '|')) {
                std::vector<std::string> parts;
                std::istringstream entry_stream(creator_entry);
                std::string part;
                while (std::getline(entry_stream, part, ':')) {
                    parts.push_back(part);
                }

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
