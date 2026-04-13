#pragma once

#include <httplib.h>
#include <string>
#include <functional>

namespace httpclient {
    enum FieldType {
        LevelID,
        LevelName,
        LevelDescription,
    };

    using http_headers = std::unordered_map<std::string, std::string>;
    using FetchCallback = std::function<void(const std::string& result)>;

    http_headers parseResponse(const std::string& body);
    void fetch(const std::string& id, const FieldType& field, FetchCallback callback = nullptr);
}
