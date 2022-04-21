#ifndef WELFORD_H
#define WELFORD_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <math.h>

// Accurately computing running variance
// http://www.johndcook.com/standard_deviation.html

typedef struct Welford Welford;

struct Welford
{
    double      num;
    double      min;
    double      max;
    double      oldM, newM;
    double      oldS, newS;
};

static inline void
welford_add(Welford *w, double x)
{
    if (++w->num == 1) {
        w->oldS = 0;
        w->oldM = w->newM = w->min = w->max = x;
    } else {
        w->newM = w->oldM + (x - w->oldM)/w->num;
        w->newS = w->oldS + (x - w->oldM)*(x - w->newM);
        w->oldM = w->newM;
        w->oldS = w->newS;
        if (x < w->min) w->min = x;
        if (x > w->max) w->max = x;
    }
}

static inline double welford_mean(const Welford *w)   { return w->num == 0 ? 0 : w->newM; }
static inline double welford_var(const Welford *w)    { return w->num > 1 ? w->newS/(w->num - 1) : 0; }
static inline double welford_sdev(const Welford *w)   { return sqrt(welford_var(w)); }

#if defined(__cplusplus)
}
#endif

#endif
