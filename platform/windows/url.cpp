#include <url.h>
#include <Windows.h>
#include <string>
#include <Geode/modify/MenuLayer.hpp>

const auto executionPath = (geode::prelude::dirs::getGameDir() / "GeometryDash.exe").string();

CustomUrl::CustomUrl(const std::string& scheme, const std::string& exePath = executionPath) {
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

    std::string cmd = "\"" + exePath + "\" \"%1\"";
    RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)cmd.c_str(), cmd.size() + 1);
    RegCloseKey(hKey);
}
CustomUrl::~CustomUrl() {}

void CustomUrl::sendNotification(const char* title, const char* text) {

}
