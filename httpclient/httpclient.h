#pragma once

#include <httplib.h>

namespace httpclient {
    enum FieldType {
        LevelID,
        LevelName,
        LevelDescription,
    };
    using http_headers = std::unordered_map<std::string, std::string>;
    http_headers parseResponse(const std::string& body);
    std::string fetch(const std::string& id, const FieldType& field);
}
