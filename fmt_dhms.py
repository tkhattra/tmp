#!/usr/bin/env python3

# format seconds value as days/hours/minutes/seconds (non-compact output)
def fmt_dhms(s):
    s = int(s)
    d = s//86400
    s -= d*86400
    h = s//3600
    s -= h*3600
    m = s//60
    s -= m*60
    if d: return f'{d}d{h}h{m}m{s}s'
    if h: return f'{h}h{m}m{s}s'
    if m: return f'{m}m{s}s'
    return f'{s}s'

# format seconds value as days/hours/minutes/seconds (rounded compact output)
def fmt_dhms_compact(s):
    s = int(s)
    d = s//86400
    s -= d*86400
    if d:
        h = round(s/3600)
        if h >= 24:
            d += 1
            h -= 24
        if not h:
            return f'{d}d'
        return f'{d}d{h}h'

    h = s//3600
    s -= h*3600
    if h:
        m = round(s/60)
        if m >= 60:
            h += 1
            m -= 60
        if not m:
            return f'{h}h'
        return f'{h}h{m}m'

    m = s//60
    s -= m*60
    if m:
        if not s:
            return f'{m}m'
        return f'{m}m{s}s'
    return f'{s}s'

if __name__ == '__main__':
    import sys
    print(fmt_dhms(sys.argv[1]))
    print(fmt_dhms_compact(sys.argv[1]))
