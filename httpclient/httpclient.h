#pragma once

#include <httplib.h>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

namespace httpclient {
    enum FieldType {
        LevelID = 1,
        LevelName = 2,
        LevelDescription = 3,
        LevelAuthorID = 6,  // userID inside level data, to match against creator
        LevelSongID, // use this enum
    };

    using http_headers = std::unordered_map<std::string, std::string>;

    struct ParsedResponse {
        http_headers level_fields;  // key:value pairs from level data
        std::string author_name;    // parsed from creators section
    };

    ParsedResponse parseResponse(const std::string& body);

    class BoomlingsLevel {
    private:
        std::thread gclient;
        mutable std::mutex fields_mutex;
        std::atomic<bool> ready{ false };
        http_headers fields;
        std::string author_name;
        std::string level_id;
        std::function<void()> on_ready;

    public:
        BoomlingsLevel() = default;
        ~BoomlingsLevel();

        void fetch(std::string level_id, std::function<void()> callback = nullptr);
        bool is_ready() const;
        std::string get_result(const FieldType& field) const;
        std::string get_author() const;
        std::string get_song_id() const;   // ADD THIS
    };

} // namespace httpclient