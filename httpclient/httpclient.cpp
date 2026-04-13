#include "httpclient.h"
#include <httplib.h>
#include <Geode/Geode.hpp>
#include <thread>
#include <mutex>
#include <unordered_map>

static std::mutex g_results_mutex;
static std::unordered_map<std::string, std::string> g_results;

static std::string make_key(const std::string& id, const httpclient::FieldType& field) {
    return id + "_" + std::to_string(static_cast<int>(field));
}

httpclient::http_headers httpclient::parseResponse(const std::string& body) {
    std::string levelSection = body.substr(0, body.find('#'));

    std::string firstLevel = levelSection.substr(0, levelSection.find('|'));

    http_headers fields;
    std::istringstream stream(firstLevel);
    std::string key, value;

    while (std::getline(stream, key, ':') && std::getline(stream, value, ':')) {
        fields[key] = value;
    }
    return fields;
}

void httpclient::fetch(const std::string& levelId, const FieldType& field) {
    std::thread([levelId, field]() {
        try {
            httplib::Client cli("www.boomlings.com", 80);

            cli.set_default_headers({
                { "User-Agent", "" }
            });

            cli.set_connection_timeout(10, 0);
            cli.set_read_timeout(10, 0);

            httplib::Params params = {
                { "gameVersion", "22"         },
                { "binaryVersion", "42"       },
                { "gdw",         "0"          },
                { "str",         levelId      },
                { "type",        "19"         },
                { "secret",      "Wmfd2893gb7"}
            };

            auto res = cli.Post("/database/getGJLevels21.php", params);

            std::string result = "-1";
            if (res && res->status == 200 && res->body != "-1") {
                auto fields = parseResponse(res->body);
                switch (field) {
                    case LevelID:
                        result = fields["1"];
                        break;
                    case LevelName:
                        result = fields["2"];
                        break;
                    default:
                        result = "-1";
                        break;
                }
            }

            std::lock_guard<std::mutex> lock(g_results_mutex);
            g_results[make_key(levelId, field)] = result;
        } catch (const std::exception& e) {
            geode::log::error("HTTP fetch failed: {}", e.what());
            std::lock_guard<std::mutex> lock(g_results_mutex);
            g_results[make_key(levelId, field)] = "-1";
        }
    }).detach();
}

std::string httpclient::get_result(const std::string& id, const FieldType& field) {
    std::lock_guard<std::mutex> lock(g_results_mutex);
    auto it = g_results.find(make_key(id, field));
    if (it != g_results.end()) {
        return it->second;
    }
    return "-1";
}

namespace httpclient {
    void BoomlingsLevel::fetch(std::string levelId) {
        this->gclient = std::thread([this, levelId]() {
            httplib::Client cli("www.boomlings.com", 80);
            cli.set_default_headers({
            { "User-Agent", "" }
            });

            cli.set_connection_timeout(10, 0);
            cli.set_read_timeout(10, 0);

            httplib::Params params = {
            { "gameVersion", "22"         },
            { "binaryVersion", "42"       },
            { "gdw",         "0"          },
            { "str",         levelId      },
            { "type",        "19"         },
            { "secret",      "Wmfd2893gb7"}
            };

            auto res = cli.Post("/database/getGJLevels21.php", params);
            if (res && res->status == 200 && res->body != "-1") {
                this->fields = parseResponse(res->body);
            }
        });
    }

    std::string BoomlingsLevel::get_result(const FieldType &field) {
        return this->fields[std::to_string(field)];
    }
}
