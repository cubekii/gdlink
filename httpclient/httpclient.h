#pragma once

#include <httplib.h>
#include <string>
#include <unordered_map>

namespace httpclient {
    enum FieldType {
        LevelID,
        LevelName,
        LevelDescription,
    };

    using http_headers = std::unordered_map<std::string, std::string>;

    http_headers parseResponse(const std::string& body);
    void fetch(const std::string& id, const FieldType& field);
    std::string get_result(const std::string& id, const FieldType& field);
}
