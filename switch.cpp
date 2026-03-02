#include "switch.h"

Switch::Switch(int initial_processing_servers, int initial_streaming_servers, int cooldown) 
    : processing_lb(initial_processing_servers, cooldown), 
      streaming_lb(initial_streaming_servers, cooldown) {}

void Switch::route_request(Request req) {
    if (req.job_type == 'P') {
        processing_lb.add_request(req);
    } else if (req.job_type == 'S') {
        streaming_lb.add_request(req);
    }
}

void Switch::process_cycle() {
    processing_lb.process_cycle();
    streaming_lb.process_cycle();
}

void Switch::set_firewall_rule(std::string ip_prefix) {
    processing_lb.set_firewall_rule(ip_prefix);
    streaming_lb.set_firewall_rule(ip_prefix);
}

LoadBalancer& Switch::get_processing_lb() {
    return processing_lb;
}

LoadBalancer& Switch::get_streaming_lb() {
    return streaming_lb;
}