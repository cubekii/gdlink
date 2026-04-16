#include "httpclient.h"
#include "httplib.h"

HttpClient::HttpClient() : cli("",80) {
    cli.set_default_headers({ { "User-Agent", "" } });
    cli.set_connection_timeout(10, 0);
    cli.set_read_timeout(10, 0);
}
