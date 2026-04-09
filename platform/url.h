#pragma once
#include "string_view"

class Notify {
public:
    Notify();
    virtual ~Notify();
    void sendNotification(const char* title, const char* text);
};