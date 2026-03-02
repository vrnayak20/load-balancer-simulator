#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief A structure representing a network request.
 * Contains the source IP, destination IP, the time required to process,
 * and the type of job (Processing or Streaming).
 */
struct Request {
    std::string ip_in;     /**< The source IP address of the requester. */
    std::string ip_out;    /**< The destination IP address for the results. */
    int process_time;      /**< The time required to process the request (in clock cycles). */
    char job_type;         /**< The type of job: 'P' for Processing, 'S' for Streaming. */
};

#endif // REQUEST_H