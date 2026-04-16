#pragma once

#include <httplib.h>


class HttpClient {
private:
    httplib::Client cli;
public:
    HttpClient();
};