#include <url.h>
#include <Windows.h>

Notify::Notify() {}
Notify::~Notify() {}

void Notify::sendNotification(const char* title, const char* text) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);  // ОБЯЗАТЕЛЬНО

    ZeroMemory(&pi, sizeof(pi));

    CreateProcess(
        NULL,
        (LPSTR)"notepad.exe",
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
}
