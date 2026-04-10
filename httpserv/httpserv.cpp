#include "httpserv.h"
#include "httplib.h"

inline void httpserv::startserver() {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello, World!", "text/plain");
    });

    svr.Get("/api/data", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("{\"status\": \"ok\"}", "application/json");
    });

    svr.listen("localhost", 6767);
}
