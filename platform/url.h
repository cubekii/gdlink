#pragma once
#include <string>

class CustomUrl {
private:
    std::string custom_url = "gdlink";
public:
    CustomUrl();
    virtual ~CustomUrl() = default;

    /**
     *
     * @return GeometryDash executable argument with gdlink://
     */
    static std::string GetLink();
};