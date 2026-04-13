#pragma once

#include <httplib.h>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

namespace httpclient {
    enum FieldType {
        LevelID,
        LevelName,
        LevelDescription,
    };

    using http_headers = std::unordered_map<std::string, std::string>;

    http_headers parseResponse(const std::string& body);

    class BoomlingsLevel {
    private:
        std::thread gclient;
        std::mutex fields_mutex;
        std::atomic<bool> ready{ false };  // <-- add this
        http_headers fields;
        std::string level_id;

    public:
        BoomlingsLevel() = default;
        ~BoomlingsLevel();

        void fetch(std::string level_id);
        bool is_ready() const;
        std::string get_result(const FieldType& field);
    };

}