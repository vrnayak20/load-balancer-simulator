#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "request.h"

/**
 * @brief A class representing a single web server.
 * The web server takes requests from the load balancer, processes them
 * over a set number of clock cycles, and signals when it is free to take another.
 */
class WebServer {
private:
    Request current_request; /**< The request currently being processed. */
    int time_remaining;      /**< Clock cycles remaining for the current request. */
    bool server_busy;        /**< Flag indicating if the server is currently processing a request. */

public:
    /**
     * @brief Default constructor for WebServer.
     * Initializes the server as free with no current request.
     */
    WebServer();

    /**
     * @brief Assigns a new request to the server.
     * @param req The request to be processed.
     */
    void assign_request(Request req);

    /**
     * @brief Simulates one clock cycle of processing.
     * Decrements the time remaining if the server is busy.
     * @return True if a request finished in this exact cycle, false otherwise.
     */
    bool process_cycle();

    /**
     * @brief Checks if the server is currently busy.
     * @return True if processing a request, false if free.
     */
    bool is_busy() const;
};

#endif // WEBSERVER_H