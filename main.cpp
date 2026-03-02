/**
 * @mainpage Load Balancer Simulation Project
 * @section intro_sec Introduction
 * This project simulates a load balancer handling incoming web requests.
 * It maintains request queues, processes jobs over time, and scales server pools.
 * @section features_sec Key Features
 * - Uses a Switch class to route jobs by type ('P' or 'S').
 * - Runs processing and streaming load balancers concurrently.
 * - Dynamic allocation and deallocation of web servers.
 * - Firewall implementation to block specific incoming IP ranges.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "switch.h"
#include "request.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

std::string generate_ip() {
    return std::to_string(rand() % 256) + "." + 
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." + 
           std::to_string(rand() % 256);
}

Request generate_request(int min_time, int max_time) {
    Request req;
    req.ip_in = generate_ip();
    req.ip_out = generate_ip();
    req.process_time = min_time + (rand() % (max_time - min_time + 1));
    req.job_type = (rand() % 2 == 0) ? 'P' : 'S';
    return req;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int initial_p_servers, initial_s_servers, total_cycles;

    std::cout << CYAN << "--- Switch & Multi-Load Balancer Setup ---" << RESET << "\n";
    std::cout << "Enter initial servers for processing jobs: ";
    std::cin >> initial_p_servers;
    std::cout << "Enter initial servers for streaming jobs: ";
    std::cin >> initial_s_servers;
    std::cout << "Enter total clock cycles to simulate: ";
    std::cin >> total_cycles;

    const int MIN_TASK_TIME = 5;
    const int MAX_TASK_TIME = 500;
    const int SCALING_COOLDOWN = 10; 

    Switch main_switch(initial_p_servers, initial_s_servers, SCALING_COOLDOWN);
    
    main_switch.set_firewall_rule("192.168.");
    std::cout << YELLOW << "Firewall activated: Blocking IP subnet 192.168.*.*" << RESET << "\n\n";

    int total_initial_servers = initial_p_servers + initial_s_servers;
    int initial_queue_target = total_initial_servers * 100;
    for (int i = 0; i < initial_queue_target; ++i) {
        main_switch.route_request(generate_request(MIN_TASK_TIME, MAX_TASK_TIME));
    }

    int start_p_queue = main_switch.get_processing_lb().get_queue_size();
    int start_s_queue = main_switch.get_streaming_lb().get_queue_size();
    
    std::cout << "Starting Simulation...\n";
    std::cout << "Initial Processing Queue: " << start_p_queue << "\n";
    std::cout << "Initial Streaming Queue:  " << start_s_queue << "\n\n";

    int prev_p_servers = main_switch.get_processing_lb().get_server_count();
    int prev_s_servers = main_switch.get_streaming_lb().get_server_count();
    int randomly_generated_p = 0;
    int randomly_generated_s = 0;

    for (int cycle = 0; cycle < total_cycles; ++cycle) {
        if (rand() % 10 < 3) { 
            Request new_req = generate_request(MIN_TASK_TIME, MAX_TASK_TIME);
            main_switch.route_request(new_req);
            if (new_req.job_type == 'P') randomly_generated_p++;
            else randomly_generated_s++;
        }

        main_switch.process_cycle();

        int curr_p_servers = main_switch.get_processing_lb().get_server_count();
        if (curr_p_servers > prev_p_servers) {
            std::cout << GREEN << "[Cycle " << cycle << "] [PROCESSING] Scale Up: Total servers: " << curr_p_servers << RESET << "\n";
            prev_p_servers = curr_p_servers;
        } else if (curr_p_servers < prev_p_servers) {
            std::cout << RED << "[Cycle " << cycle << "] [PROCESSING] Scale Down: Total servers: " << curr_p_servers << RESET << "\n";
            prev_p_servers = curr_p_servers;
        }

        int curr_s_servers = main_switch.get_streaming_lb().get_server_count();
        if (curr_s_servers > prev_s_servers) {
            std::cout << CYAN << "[Cycle " << cycle << "] [STREAMING] Scale Up: Total servers: " << curr_s_servers << RESET << "\n";
            prev_s_servers = curr_s_servers;
        } else if (curr_s_servers < prev_s_servers) {
            std::cout << MAGENTA << "[Cycle " << cycle << "] [STREAMING] Scale Down: Total servers: " << curr_s_servers << RESET << "\n";
            prev_s_servers = curr_s_servers;
        }
    }

    std::cout << YELLOW << "\n--- Final Concurrent Statistics ---" << RESET << "\n";
    std::cout << "Task Time Range for both: " << MIN_TASK_TIME << " to " << MAX_TASK_TIME << " cycles\n";
    std::cout << "Total Clock Cycles Run:   " << total_cycles << "\n\n";

    std::cout << CYAN << ">> Processing Load Balancer <<" << RESET << "\n";
    std::cout << "Starting Queue Size:      " << start_p_queue << "\n";
    std::cout << "Ending Queue Size:        " << main_switch.get_processing_lb().get_queue_size() << "\n";
    std::cout << "Final Server Count:       " << main_switch.get_processing_lb().get_server_count() << "\n";
    std::cout << "Total Requests Processed: " << main_switch.get_processing_lb().get_processed_count() << "\n";
    std::cout << "Random Requests Gen:      " << randomly_generated_p << "\n";
    std::cout << "Active Servers:           " << main_switch.get_processing_lb().get_active_servers() << "\n";
    std::cout << "Inactive Servers:         " << main_switch.get_processing_lb().get_inactive_servers() << "\n";
    std::cout << "Requests Blocked by FW:   " << main_switch.get_processing_lb().get_discarded_count() << "\n\n";

    std::cout << CYAN << ">> Streaming Load Balancer <<" << RESET << "\n";
    std::cout << "Starting Queue Size:      " << start_s_queue << "\n";
    std::cout << "Ending Queue Size:        " << main_switch.get_streaming_lb().get_queue_size() << "\n";
    std::cout << "Final Server Count:       " << main_switch.get_streaming_lb().get_server_count() << "\n";
    std::cout << "Total Requests Processed: " << main_switch.get_streaming_lb().get_processed_count() << "\n";
    std::cout << "Random Requests Gen:      " << randomly_generated_s << "\n";
    std::cout << "Active Servers:           " << main_switch.get_streaming_lb().get_active_servers() << "\n";
    std::cout << "Inactive Servers:         " << main_switch.get_streaming_lb().get_inactive_servers() << "\n";
    std::cout << "Requests Blocked by FW:   " << main_switch.get_streaming_lb().get_discarded_count() << "\n";

    return 0;
}