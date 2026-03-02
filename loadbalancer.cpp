/**
 * @mainpage Load Balancer Simulation Project
 * @section intro_sec Introduction
 * Implements the core load balancer logic used by the simulation.
 * The class manages a request queue, server pool, firewall filtering, and scaling.
 * @section features_sec Key Features
 * - Queues incoming requests and assigns them to available servers.
 * - Blocks requests that match a configured IP prefix.
 * - Scales server count up or down based on queue pressure.
 * - Tracks processed and discarded request counts.
 */

#include "loadbalancer.h"
#include <iostream>

LoadBalancer::LoadBalancer(int initial_servers, int cooldown) {
    system_time = 0;
    scaling_cooldown = cooldown;
    cooldown_timer = 0;
    blocked_ip_prefix = "";
    requests_discarded = 0;
    total_processed = 0;

    for (int i = 0; i < initial_servers; ++i) {
        servers.push_back(WebServer());
    }
}

void LoadBalancer::set_firewall_rule(std::string ip_prefix) {
    blocked_ip_prefix = ip_prefix;
}

void LoadBalancer::add_request(Request req) {
    if (!blocked_ip_prefix.empty() && req.ip_in.find(blocked_ip_prefix) == 0) {
        requests_discarded++;
        return; 
    }
    requestqueue.push(req);
}

void LoadBalancer::manage_scale() {
    if (cooldown_timer > 0) {
        return; 
    }

    int current_queue_size = requestqueue.size();
    int num_servers = servers.size();

    if (num_servers == 0) return;

    if (current_queue_size < (50 * num_servers) && num_servers > 1) {
        servers.pop_back(); 
        cooldown_timer = scaling_cooldown;
    } 
    else if (current_queue_size > (80 * num_servers)) {
        servers.push_back(WebServer());
        cooldown_timer = scaling_cooldown;
    }
}

void LoadBalancer::process_cycle() {
    system_time++;

    if (cooldown_timer > 0) {
        cooldown_timer--;
    }

    for (size_t i = 0; i < servers.size(); ++i) {
        if (!servers[i].is_busy() && !requestqueue.empty()) {
            servers[i].assign_request(requestqueue.front());
            requestqueue.pop();
        }
    }

    for (size_t i = 0; i < servers.size(); ++i) {
        if (servers[i].process_cycle()) {
            total_processed++;
        }
    }

    manage_scale();
}

int LoadBalancer::get_queue_size() const { return requestqueue.size(); }
int LoadBalancer::get_server_count() const { return servers.size(); }
int LoadBalancer::get_system_time() const { return system_time; }
int LoadBalancer::get_discarded_count() const { return requests_discarded; }
int LoadBalancer::get_processed_count() const { return total_processed; }

int LoadBalancer::get_active_servers() const {
    int count = 0;
    for (size_t i = 0; i < servers.size(); ++i) {
        if (servers[i].is_busy()) {
            count++;
        }
    }
    return count;
}

int LoadBalancer::get_inactive_servers() const {
    return servers.size() - get_active_servers();
}