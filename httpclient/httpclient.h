#pragma once

#include <httplib.h>

namespace httpclient {
    using http_headers = std::unordered_map<std::string, std::string>;
    http_headers parseResponse(const std::string& body);
    void fetch(const std::string& id);
}
