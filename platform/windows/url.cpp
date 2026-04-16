#include <url.h>
#include <Windows.h>
#include <string>
#include <Geode/modify/MenuLayer.hpp>
#include "../../src/openurl/searchtab.h"

CustomUrl::CustomUrl() {
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
        static constexpr char cmd[] = "curl.exe -X POST http://localhost:6767/api/loadurl -d \"%1\"";
        RegSetValueExA(hKey, nullptr, 0, REG_SZ, (const BYTE*)cmd, sizeof(cmd));
    });
}

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