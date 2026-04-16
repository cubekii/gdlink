#include <url.h>
#include <Windows.h>
#include <string>
#include <Geode/modify/MenuLayer.hpp>
#include "../../src/openurl/searchtab.h"

CustomUrl::CustomUrl(const std::string& scheme) : custom_url(scheme) {
    const std::string baseKey = "SOFTWARE\\Classes\\" + custom_url;
    const std::string cmdKey  = baseKey + "\\shell\\open\\command";

    auto writeKeys = [&](const std::string& keyPath, auto&& fn) -> bool {
        HKEY hKey = nullptr;
        if (RegCreateKeyExA(HKEY_CURRENT_USER, keyPath.c_str(), 0, nullptr,
                REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr) != ERROR_SUCCESS)
            return false;

        fn(hKey);
        RegCloseKey(hKey);
        return true;
    };

    writeKeys(baseKey, [](HKEY hKey) {
        static constexpr char desc[]    = "Custom geometry dash links";
        static constexpr char proto[]   = "";

        RegSetValueExA(hKey, nullptr, 0, REG_SZ, (const BYTE*)desc,  sizeof(desc));
        RegSetValueExA(hKey, "GDLink Protocol", 0, REG_SZ, (const BYTE*)proto, sizeof(proto));
    });

    writeKeys(cmdKey, [](HKEY hKey) {
        static constexpr char cmd[] = "curl.exe -X POST http://localhost:6767/api/echo -d \"%1\"";
        RegSetValueExA(hKey, nullptr, 0, REG_SZ, (const BYTE*)cmd, sizeof(cmd));
    });
}
CustomUrl::~CustomUrl() {}

std::string CustomUrl::GetLink() {
    const std::string_view cmdLine = GetCommandLineA();
    const size_t pos = cmdLine.find("gdlink://");

    if (pos == std::string_view::npos)
        return "";

    std::string result(cmdLine.substr(pos));

    if (!result.empty() && result.back() == '"')
        result.pop_back();

    return result;
}

void CustomUrl::Redirect(const std::string& url) {
    static constexpr std::string_view SCHEME = "gdlink://GD/";
    static constexpr std::string_view LEVEL_ACTION = "level/";

    if (url.size() <= SCHEME.size()) return;

    std::string_view view(url);

    if (view.substr(0, SCHEME.size()) != SCHEME) {
        std::cout << "Invalid scheme\n";
        return;
    }

    view = view.substr(SCHEME.size());

    // Strip query string
    if (auto q = view.find('?'); q != std::string_view::npos)
        view = view.substr(0, q);

    if (view.substr(0, LEVEL_ACTION.size()) != LEVEL_ACTION) return;

    view = view.substr(LEVEL_ACTION.size());

    int id = 0;
    for (char c : view) {
        if (c < '0' || c > '9') {
            geode::prelude::log::warn("wrong id");
            return;
        }
        id = id * 10 + (c - '0');
    }

    if (!view.empty())
        gdloader::loadlevel(id);
}