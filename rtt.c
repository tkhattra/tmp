#include <stdint.h>
#include <assert.h>
#include "rtt.h"

// Unix Network Programming, W. Richard Stevens
// Congestion Avoidance and Control, Van Jacobson and Michael J. Karels
// https://tools.ietf.org/html/rfc6298

// note: all RTT times are in microseconds

enum
{
    RTT_INIT_VAR = 250000,                  // 250ms
};

static float rtt_min_rto = 1000000;         // 1s
static float rtt_max_rto = 60000000;        // 1m

static float
rtt_rto(const RTT *rtt)
{
    return rtt->srtt + 4*rtt->rttvar;
}

static float
rto_minmax(float rto)
{
    if (rto < rtt_min_rto)
        rto = rtt_min_rto;
    else if (rto > rtt_max_rto)
        rto = rtt_max_rto;
    return rto;
}

void
rtt_init_minmax_rto(uint64_t min_rto, uint64_t max_rto)
{
    assert(min_rto <= max_rto);
    rtt_min_rto = min_rto;
    rtt_max_rto = max_rto;
}

void
rtt_init(RTT *rtt)
{
    rtt->rtt = rtt->srtt = 0;
    rtt->rttvar = RTT_INIT_VAR;
    rtt->rto = rto_minmax(rtt_rto(rtt));        // initial rto = 1s
}

void
rtt_stop(RTT *rtt, uint64_t usec)
{
    float delta;

    if (usec <= 0)
        return;

    if (!rtt->rtt) {
        // first rtt
        rtt->rtt = rtt->srtt = usec;
        rtt->rttvar = usec/2;
        rtt->rto = rto_minmax(rtt_rto(rtt));
        return;
    }

    rtt->rtt = usec;
    delta = rtt->rtt - rtt->srtt;

    rtt->srtt += delta/8;                       // g = 1/8
    assert(rtt->srtt >= 0);

    if (delta < 0)
        delta = -delta;
    rtt->rttvar += (delta - rtt->rttvar)/4;     // h = 1/4
    assert(rtt->rttvar >= 0);

    rtt->rto = rto_minmax(rtt_rto(rtt));
}

// return rtt->rto * (1 << nresend), but capped at rtt->max_rto
float
rtt_timeout_rto(const RTT *rtt, uint32_t nresend)
{
    float rto = rtt->rto;

    assert(rto >= rtt_min_rto);
    assert(rto <= rtt_max_rto);

    // use a loop to prevent arithmetic overflow
    while (nresend-- && (rto < rtt_max_rto))
        rto *= 2;

    return rto_minmax(rto);
}
