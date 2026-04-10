#include <url.h>
#include <Windows.h>
#include <string>
#include <Geode/modify/MenuLayer.hpp>
#include "../../src/openurl/searchtab.h"

const auto executionPath = (geode::prelude::dirs::getGameDir() / "GeometryDash.exe").string();

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

    // Set the command to execute
    std::string cmdKey = baseKey + "\\shell\\open\\command";
    RegCreateKeyExA(HKEY_CURRENT_USER, cmdKey.c_str(), 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

    std::string cmd = "\"" + executionPath + "\" \"%1\"";
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

void CustomUrl::Redirect() {
    std::string url = GetLink();
    if (url.empty())
        return;
    const std::string scheme = "gdlink://";

    // Проверяем схему
    if (url.rfind(scheme, 0) != 0) {
        std::cout << "Invalid scheme\n";
        return;
    }

    // Убираем "gdlink://"
    std::string path = url.substr(scheme.length());

    // Убираем query (всё после '?')
    size_t queryPos = path.find('?');
    if (queryPos != std::string::npos) {
        path = path.substr(0, queryPos);
    }

    // Теперь path = "level/90475473"
    std::cout << "Path: " << path << std::endl;

    // Разбиваем путь
    size_t slashPos = path.find('/');
    if (slashPos != std::string::npos) {
        std::string action = path.substr(0, slashPos);
        std::string id = path.substr(slashPos + 1);


        if (action == "level") {
            try {
                gdloader::loadlevel(std::stoi(id));
            } catch (const std::exception& e) {
                geode::prelude::log::warn("wrong id");
            }
        }
    }
}
