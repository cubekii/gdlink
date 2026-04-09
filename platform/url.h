#pragma once
#include <string>

class CustomUrl {
public:
    CustomUrl(const std::string& scheme, const std::string& exePath);
    virtual ~CustomUrl();
    void sendNotification(const char* title, const char* text);
};