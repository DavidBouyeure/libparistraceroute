#ifndef ALGORITHMS_PING_H
#define ALGORITHMS_PING_H

#include <stdbool.h>     // bool
#include <stdint.h>      // uint*_t
#include <stddef.h>      // size_t
#include <limits.h>      // INT_MAX

#include "../address.h"  // address_t
#include "../pt_loop.h"  // pt_loop_t
#include "../dynarray.h" // dynarray_t
#include "../options.h"  // option_t
#include "traceroute.h"

#define OPTIONS_TRACEROUTE_MIN_TTL_DEFAULT            1
#define OPTIONS_TRACEROUTE_MAX_TTL_DEFAULT            30
#define OPTIONS_TRACEROUTE_MAX_UNDISCOVERED_DEFAULT   3
#define OPTIONS_TRACEROUTE_NUM_QUERIES_DEFAULT        3
#define OPTIONS_TRACEROUTE_DO_RESOLV_DEFAULT          true

#define OPTIONS_PING_PACKET_SIZE_DEFAULT              0
#define OPTIONS_PING_SHOW_TIMESTAMP_DEFAULT           false
#define OPTIONS_PING_IS_QUIET_DEFAULT                 false
#define OPTIONS_PING_COUNT_DEFAULT                    INT_MAX

#define OPTIONS_TRACEROUTE_MIN_TTL          {OPTIONS_TRACEROUTE_MIN_TTL_DEFAULT,          1, 255}
#define OPTIONS_TRACEROUTE_MAX_TTL          {OPTIONS_TRACEROUTE_MAX_TTL_DEFAULT,          1, 255}
#define OPTIONS_TRACEROUTE_MAX_UNDISCOVERED {OPTIONS_TRACEROUTE_MAX_UNDISCOVERED_DEFAULT, 1, 255}
#define OPTIONS_TRACEROUTE_NUM_QUERIES      {OPTIONS_TRACEROUTE_NUM_QUERIES_DEFAULT,      1, 255}

#define OPTIONS_PING_PACKET_SIZE            {OPTIONS_PING_PACKET_SIZE_DEFAULT,      0, INT_MAX}
#define OPTIONS_PING_COUNT                  {OPTIONS_PING_COUNT_DEFAULT, 1, OPTIONS_PING_COUNT_DEFAULT}

#define HELP_c      "Stop after sending count ECHO_REQUEST packets. With deadline option, ping waits for 'count' ECHO_REPLY packets, until the timeout expires."
#define HELP_D      "Print timestamp (unix time + microseconds as in gettimeofday) before each line."
#define HELP_n      "Do not resolve IP addresses to their domain names"
#define HELP_q_ping "Quiet output. Nothing is displayed except the summary lines at startup time and when finished."
#define HELP_t      "Set the IP Time to Live."
#define HELP_v      "Verbose output."

typedef traceroute_event_type_t ping_event_type_t;

// Get the different values of traceroute options
uint8_t options_ping_get_min_ttl(); // NOT NEEDED FOR PING
uint8_t options_ping_get_max_ttl();
uint8_t options_ping_get_num_queries(); // NOT NEEDED FOR PING
uint8_t options_ping_get_max_undiscovered(); // NOT NEEDED FOR PING
bool    options_ping_get_do_resolv();

/*
 * Principle: (from man page)
 *
 * traceroute - print the route packets trace to network host
 *
 * traceroute  tracks the route packets taken from an IP network on
 * their way to a given host. It utilizes the IP protocol's time to
 * live (TTL) field and * attempts to elicit an ICMP TIME_EXCEEDED
 * response from each gateway along the path to the host.
 *
 * Algorithm:
 *
 *     INIT:
 *         cur_ttl = min_ttl
 *         SEND
 *
 *     SEND:
 *         send num_probes probes with TTL = cur_ttl
 *
 *     PROBE_REPLY:
 *         if < num_probes
 *             continue waiting
 *             if all_stars or destination_reached or stopping ICMP error
 *                 EXIT
 *             cur_ttl += 1
 *             SEND
 */

//--------------------------------------------------------------------
// Options
//--------------------------------------------------------------------

typedef struct {
    uint8_t           min_ttl;          /**< Minimum ttl at which to send probes */ // NOT NEEDED FOR PING 
    uint8_t           max_ttl;          /**< Maximum ttl at which to send probes */ 
    unsigned int      count;            /**< Number of probes to be sent         */
    size_t            num_probes;       /**< Number of probes per hop            */ // NOT NEEDED FOR PING
    size_t            max_undiscovered; /**< Maximum number of consecutives undiscovered hops */ // NOT NEEDED FOR PING
    const address_t * dst_addr;         /**< The target IP */
    bool              do_resolv;        /**< Resolv each discovered IP hop */
} ping_options_t;

const option_t * ping_get_options();

ping_options_t ping_get_default_options();

void options_ping_init(ping_options_t * traceroute_options, address_t * address);

//--------------------------------------------------------------------
// Custom-events raised by traceroute algorithm
//--------------------------------------------------------------------


// TODO since this structure should exactly match with a standard event_t, define a macro allowing to define custom events
// CREATE_EVENT(traceroute) uses ping_event_type_t and defines ping_event_t
typedef struct {
    ping_event_type_t type;
    void                  * data;
    void                 (* data_free)(void *); /**< Called in event_free to release data. Ignored if NULL. */
    void                  * zero;
} ping_event_t;

typedef struct {
    bool          destination_reached; /**< True iif the destination has been reached at least once for the current TTL */
    uint8_t       ttl;                 /**< TTL currently explored                   */
    size_t        num_replies;         /**< Total of probe sent for this instance    */
    size_t        num_undiscovered;    /**< Number of consecutive undiscovered hops  */
    size_t        num_stars;           /**< Number of probe lost for the current hop */
    dynarray_t  * probes;              /**< Probe instances allocated by traceroute  */
} ping_data_t;

//-----------------------------------------------------------------
// Traceroute default handler
//-----------------------------------------------------------------

/**
 * \brief Handle raised ping_event_t events.
 * \param loop The main loop.
 * \param traceroute_event The handled event.
 * \param traceroute_options Options related to this instance of traceroute .
 * \param traceroute_data Data related to this instance of traceroute.
 */

void ping_handler(
    pt_loop_t            * loop,
    ping_event_t         * traceroute_event,
    const ping_options_t * traceroute_options,
    const ping_data_t    * traceroute_data
);

#endif