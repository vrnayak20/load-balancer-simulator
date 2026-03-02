#ifndef SWITCH_H
#define SWITCH_H

#include "loadbalancer.h"
#include "request.h"

/**
 * @brief A higher-level router that distributes requests to specific load balancers.
 * Routes 'P' (Processing) jobs to one load balancer and 'S' (Streaming) jobs to another.
 */
class Switch {
private:
    LoadBalancer processing_lb; /**< Load balancer dedicated to processing tasks. */
    LoadBalancer streaming_lb;  /**< Load balancer dedicated to streaming tasks. */

public:
    /**
     * @brief Constructs a Switch with specified initial server counts for each load balancer.
     * @param initial_processing_servers Starting servers for the processing load balancer.
     * @param initial_streaming_servers Starting servers for the streaming load balancer.
     * @param cooldown The scaling cooldown applied to both load balancers.
     */
    Switch(int initial_processing_servers, int initial_streaming_servers, int cooldown);

    /**
     * @brief Routes an incoming request to the appropriate load balancer based on job type.
     * @param req The request to be routed.
     */
    void route_request(Request req);

    /**
     * @brief Advances the simulation by one clock cycle for all connected load balancers.
     */
    void process_cycle();

    /**
     * @brief Sets the firewall rule for both load balancers.
     * @param ip_prefix The IP prefix to block.
     */
    void set_firewall_rule(std::string ip_prefix);

    /**
     * @name Load balancer accessors
     * @brief Returns references to the managed load balancer instances.
     * @return Reference to either the processing or streaming load balancer.
     */
    ///@{
    LoadBalancer& get_processing_lb();
    LoadBalancer& get_streaming_lb();
    ///@}
};

#endif // SWITCH_H