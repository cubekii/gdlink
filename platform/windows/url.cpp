#include <url.h>
#include <Windows.h>
#include <string>
#include <Geode/modify/MenuLayer.hpp>
#include "../../src/openurl/searchtab.h"

CustomUrl::CustomUrl(const std::string& scheme) {
    std::string baseKey = "SOFTWARE\\Classes\\" + scheme;

    // Create root key
    HKEY hKey;
    RegCreateKeyExA(HKEY_CURRENT_USER, baseKey.c_str(), 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

    const char* desc = "URL:MyApp Protocol";
    RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)desc, strlen(desc) + 1);

    const char* urlProto = "";
    RegSetValueExA(hKey, "URL Protocol", 0, REG_SZ, (BYTE*)urlProto, 1);
    RegCloseKey(hKey);

    // Set the command to execute curl with the URL as POST body
    std::string cmdKey = baseKey + "\\shell\\open\\command";
    RegCreateKeyExA(HKEY_CURRENT_USER, cmdKey.c_str(), 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

    // curl.exe -X POST http://localhost:6767/api/echo -d "%1"
    // %1 will be substituted with the full gdlink:// URL by Windows
    std::string cmd = "curl.exe -X POST http://localhost:6767/api/echo -d \"%1\"";
    RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)cmd.c_str(), cmd.size() + 1);
    RegCloseKey(hKey);
}
CustomUrl::~CustomUrl() {}

void CustomUrl::sendNotification(const char* title, const char* text) {

}

std::string CustomUrl::GetLink() {

    std::optional<std::string> pendingUrl = std::nullopt;
    std::string cmdLine = GetCommandLineA();

    size_t pos = cmdLine.find("gdlink://");
    if (pos != std::string::npos) {
        pendingUrl = cmdLine.substr(pos);

        if (!pendingUrl->empty() && pendingUrl->back() == '"')
            pendingUrl->pop_back();
    }

    if (pendingUrl.has_value()) {
        return *pendingUrl;
    }
    return "";
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

    // Parse ID without exceptions
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