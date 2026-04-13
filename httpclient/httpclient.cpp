#include "httpclient.h"
#include <httplib.h>
#include <Geode/Geode.hpp>
#include <thread>

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

void httpclient::fetch(const std::string& levelId, const FieldType& field, FetchCallback callback) {
    std::thread([levelId, field, callback]() {
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

            if (callback) {
                geode::queueInMainThread([callback, result]() {
                    callback(result);
                });
            }
        } catch (const std::exception& e) {
            //geode::log::error("HTTP fetch failed: {}", e.what());
            if (callback) {
                geode::queueInMainThread([callback]() {
                    callback("-1");
                });
            }
        }
    }).detach();
}