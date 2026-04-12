#include "httpclient.h"
#include <httplib.h>

httpclient::http_headers httpclient::parseResponse(const std::string& body) {
    std::string levelSection = body.substr(0, body.find('#'));

    std::string firstLevel = levelSection.substr(0, levelSection.find('|'));

    std::unordered_map<std::string, std::string> fields;
    std::istringstream stream(firstLevel);
    std::string key, value;

    while (std::getline(stream, key, ':') && std::getline(stream, value, ':')) {
        fields[key] = value;
    }
    return fields;
}

void httpclient::fetch(const std::string& levelId) {
    httplib::Client cli("https://www.boomlings.com");

    cli.set_default_headers({
        { "User-Agent", "" }
    });

    cli.set_connection_timeout(10);
    cli.set_read_timeout(10);

    httplib::Params params = {
        { "gameVersion", "22"         },
        { "binaryVersion", "42"       },
        { "gdw",         "0"          },
        { "str",         levelId      },  // the level ID to look up
        { "type",        "19"         },  // 19 = search by level ID
        { "secret",      "Wmfd2893gb7"}
    };

    auto res = cli.Post("/database/getGJLevels21.php", params);

    if (!res) {
        return;
    }

    if (res->status != 200) {
        return;
    }

    if (res->body == "-1") {
        return;
    }
    auto fields = parseResponse(res->body);
}