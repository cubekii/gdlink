#pragma once
#include <string>

class CustomUrl {
private:
    std::string custom_url;
public:
    CustomUrl(const std::string& scheme);
    virtual ~CustomUrl();
    static std::string GetLink();
    static void Redirect(const std::string& url);
};