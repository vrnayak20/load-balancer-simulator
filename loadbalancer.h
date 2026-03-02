#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <string>
#include "webserver.h"
#include "request.h"

/**
 * @brief Manages a queue of requests and a pool of web servers.
 * Distributes requests, scales the server pool dynamically, and blocks malicious IPs.
 */
class LoadBalancer {
private:
    std::queue<Request> requestqueue; /**< The queue of incoming requests. */
    std::vector<WebServer> servers;   /**< The active pool of web servers. */
    
    int system_time;                  /**< The current time in clock cycles. */
    int scaling_cooldown;             /**< The 'n' clock cycles to wait after scaling. */
    int cooldown_timer;               /**< Timer to track the scaling cooldown. */
    
    std::string blocked_ip_prefix;    /**< The IP subnet to block (Firewall feature). */

    int requests_discarded;           /**< Statistic: Number of requests blocked by firewall. */
    int total_processed;              /**< Statistic: Number of requests successfully finished. */

    /**
     * @brief Evaluates queue size and adds/removes servers based on thresholds.
     */
    void manage_scale();

public:
    /**
     * @brief Constructs a LoadBalancer with an initial number of servers.
     * @param initial_servers The starting number of web servers.
     * @param cooldown The number of cycles to wait after scaling before checking again.
     */
    LoadBalancer(int initial_servers, int cooldown);

    /**
     * @brief Adds a new request to the queue, subject to firewall rules.
     * @param req The request to be added.
     */
    void add_request(Request req);

    /**
     * @brief Advances the simulation by one clock cycle.
     * Ticks all servers, assigns pending requests to free servers, and manages scaling.
     */
    void process_cycle();

    /**
     * @brief Sets the firewall rule to block a specific IP prefix.
     * @param ip_prefix The IP prefix to block (e.g., "192.168.").
     */
    void set_firewall_rule(std::string ip_prefix);

    /**
     * @name Statistics and state getters
     * @brief Accessors used for reporting simulation status.
     * @return Integer metric for the corresponding load balancer state.
     */
    ///@{
    int get_queue_size() const;
    int get_server_count() const;
    int get_system_time() const;
    int get_discarded_count() const;
    int get_processed_count() const;
    int get_active_servers() const;
    int get_inactive_servers() const;
    ///@}
};

#endif // LOADBALANCER_H