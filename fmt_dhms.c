#include <stdio.h>
#include "fmt_dhms.h"

// format seconds value as days/hours/minutes/seconds (non-compact output)
int
fmt_dhms(int s, char *str, size_t len)
{
    int d, h, m;

    d = s/86400;
    s -= d*86400;
    h = s/3600;
    s -= h*3600;
    m = s/60;
    s -= m*60;

    if (d) return snprintf(str, len, "%dd%dh%dm%ds", d, h, m, s);
    if (h) return snprintf(str, len, "%dh%dm%ds", h, m, s);
    if (m) return snprintf(str, len, "%dm%ds", m, s);
    return snprintf(str, len, "%ds", s);
}

// format seconds value as days/hours/minutes/seconds (rounded compact output)
int
fmt_dhms_compact(int s, char *str, size_t len)
{
    int d, h, m;

    d = s/86400;
    s -= d*86400;
    if (d) {
        // round hours value and print days/hours
        h = (s + 1800)/3600;
        if (h >= 24) {
            d += 1;
            h -= 24;
        }
        if (!h) return snprintf(str, len, "%dd", d);
        return snprintf(str, len, "%dd%dh", d, h);
    }

    h = s/3600;
    s -= h*3600;
    if (h) {
        // round minutes value and print hours/minutes
        m = (s + 30)/60;
        if (m >= 60) {
            h += 1;
            m -= 60;
        }
        if (!m) return snprintf(str, len, "%dh", h);
        return snprintf(str, len, "%dh%dm", h, m);
    }

    // else print minutes/seconds, or just seconds (if minutes == 0)
    m = s/60;
    s -= m*60;
    if (m) {
        if (!s) return snprintf(str, len, "%dm", m);
        return snprintf(str, len, "%dm%ds", m, s);
    }
    return snprintf(str, len, "%ds", s);
}
