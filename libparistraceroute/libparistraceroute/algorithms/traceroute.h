#ifndef ALGORITHMS_TRACEROUTE_H
#define ALGORITHMS_TRACEROUTE_H


extern const unsigned min_ttl[];
extern const unsigned max_ttl[];

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
    uint8_t      min_ttl;    /**< Minimum ttl at which to send probes */
    uint8_t      max_ttl;    /**< Maximum ttl at which to send probes */
    size_t       num_probes; /**< Number of probes per hop            */
    const char * dst_ip;     /**< The target IP */
} traceroute_options_t;

struct opt_spec * traceroute_get_cl_options();
// Default options

traceroute_options_t traceroute_get_default_options(void);

//--------------------------------------------------------------------
// Custom-events raised by traceroute algorithm
//--------------------------------------------------------------------

typedef enum {
    // event_type                      | data (type)     | data (meaning)
    // --------------------------------+-----------------+--------------------------------------------
    TRACEROUTE_DESTINATION_REACHED, // | NULL            | N/A
    TRACEROUTE_PROBE_REPLY,         // | probe_reply_t * | The probe and its corresponding reply
    TRACEROUTE_ICMP_ERROR,          // | probe_t *       | The probe which has provoked the ICMP error
    TRACEROUTE_STAR,                // | probe_t *       | The probe which has been lost
    TRACEROUTE_MAX_TTL_REACHED,     // | NULL            | N/A
    TRACEROUTE_TOO_MANY_STARS       // | NULL            | N/A
} traceroute_event_type_t;

// TODO since this structure should exactly match with a standard event_t, define a macro allowing to define custom events
// CREATE_EVENT(traceroute) uses traceroute_event_type_t and defines traceroute_event_t
typedef struct {
    traceroute_event_type_t type;
    void                  * data;
    void                 (* data_free)(void *); /**< Called in event_free to release data. Ignored if NULL. */
    void                  * zero;
} traceroute_event_t;

typedef struct {
    bool         destination_reached; /**< True iif the destination has been reached at least once for the current TTL */
    uint8_t      ttl;                 /**< TTL currently explored                   */
    size_t       num_replies;         /**< Total of probe sent for this instance    */
    size_t       num_undiscovered;    /**< Number of consecutive undiscovered hops  */
    size_t       num_stars;           /**< Number of probe lost for the current hop */
    dynarray_t * probes;              /**< Probe instances allocated by traceroute  */
} traceroute_data_t;

#endif
