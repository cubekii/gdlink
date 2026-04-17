#include "httpserv.h"
#include "httplib.h"
#include <url.h>
#include <thread>
#include <atomic>
#include "src/openurl/searchtab.h"

static std::atomic<bool> g_running{false};
static std::thread g_server_thread;

void httpserv::startserver() {
    if (g_running.exchange(true))
        return;

    g_server_thread = std::thread([]() {
        httplib::Server svr;

        svr.Post("/api/loadurl", [](const httplib::Request& req, httplib::Response& res) {
            std::string received = req.body;

            res.set_content(received, "text/plain");
            return "";
        });
        svr.listen("127.0.0.1", 6767);
        g_running = false;
    });

    g_server_thread.detach();
}
