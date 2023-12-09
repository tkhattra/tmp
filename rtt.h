#ifndef RTT_H
#define RTT_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct RTT RTT;

// note: all RTT times are in microseconds

struct RTT
{
    float   rtt;
    float   rto;
    float   srtt;
    float   rttvar;
};

extern void     rtt_init_minmax_rto(uint64_t min_rto, uint64_t max_rto);
extern void     rtt_init(RTT *rtt);
extern void     rtt_stop(RTT *rtt, uint64_t usec);
extern float    rtt_timeout_rto(const RTT *rtt, uint32_t nresend);

#if defined(__cplusplus)
}
#endif

#endif
