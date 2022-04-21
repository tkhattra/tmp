#!/usr/bin/env python3

# https://arxiv.org/pdf/2110.01111.pdf

def sort(l):
    n = len(l)
    for i in range(0, n):
        for j in range(0, n):
            if l[i] < l[j]:
                l[i], l[j] = l[j], l[i]

if __name__ == '__main__':
    import random, sys
    l = list(range(int(sys.argv[1])))
    random.shuffle(l)
    sort(l)
    #sort(lsorted := l.copy())
    #assert sorted(l) == lsorted
