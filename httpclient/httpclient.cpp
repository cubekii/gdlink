#include "httpclient.h"
#include <httplib.h>
#include <Geode/Geode.hpp>
#include <thread>
#include <mutex>

namespace httpclient {

    http_headers parseResponse(const std::string& body) {
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

    BoomlingsLevel::~BoomlingsLevel() {
        if (gclient.joinable()) {
            gclient.join();
        }
    }

    void BoomlingsLevel::fetch(std::string levelId) {
        // Join any previous thread before starting a new one
        if (gclient.joinable()) {
            gclient.join();
        }

        this->level_id = levelId;
        this->ready = false;

        this->gclient = std::thread([this, levelId]() {
            try {
                httplib::Client cli("www.boomlings.com", 80);
                cli.set_default_headers({
                    { "User-Agent", "" }
                });
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
                        this->fields = parseResponse(res->body);
                    } else {
                        this->fields.clear();
                    }
                }
            } catch (const std::exception& e) {
                geode::log::error("HTTP fetch failed: {}", e.what());
                std::lock_guard<std::mutex> lock(this->fields_mutex);
                this->fields.clear();
            }

            // Mark as ready after fields are written
            this->ready = true;
        });
    }

    bool BoomlingsLevel::is_ready() const {
        return this->ready.load();
    }

    std::string BoomlingsLevel::get_result(const FieldType& field) {
        std::lock_guard<std::mutex> lock(this->fields_mutex);
        auto it = this->fields.find(std::to_string(static_cast<int>(field)));
        if (it != this->fields.end()) {
            return it->second;
        }
        return "-1";
    }

}