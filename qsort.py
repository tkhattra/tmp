#!/usr/bin/env python3

def qsort(l):
    if len(l) <= 1:
        return l
    pivot = l[0]
    return qsort([lt for lt in l[1:] if lt < pivot]) + \
           [pivot] + \
           qsort([ge for ge in l[1:] if ge >= pivot])

if __name__ == '__main__':
    import random, sys
    l = list(range(int(sys.argv[1])))
    random.shuffle(l)
    qsort(l)
    #lsorted = qsort(l)
    #assert sorted(l) == lsorted
