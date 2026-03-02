#include "webserver.h"

WebServer::WebServer() {
    time_remaining = 0;
    server_busy = false;
}

void WebServer::assign_request(Request req) {
    current_request = req;
    time_remaining = req.process_time;
    server_busy = true;
}

bool WebServer::process_cycle() {
    if (server_busy) {
        time_remaining--;
        if (time_remaining <= 0) {
            server_busy = false;
            return true;
        }
    }
    return false;
}

bool WebServer::is_busy() const {
    return server_busy;
}