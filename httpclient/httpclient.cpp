#include "httpclient.h"
#include <httplib.h>

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
}