#include "httpserv.h"
#include "httplib.h"
#include <thread>
#include <atomic>

static std::atomic<bool> g_running{false};
static std::thread g_server_thread;

void httpserv::startserver() {
    if (g_running.exchange(true)) {
        return; // already running
    }

    g_server_thread = std::thread([]() {
        httplib::Server svr;

        svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
            res.set_content("Hello, World!", "text/plain");
        });

        svr.Get("/api/data", [](const httplib::Request& req, httplib::Response& res) {
            res.set_content("{\"status\": \"ok\"}", "application/json");
        });

        svr.listen("0.0.0.0", 6767);
        g_running = false;
    });

    g_server_thread.detach();
}
