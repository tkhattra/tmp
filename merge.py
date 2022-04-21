#!/usr/bin/env python3

def merge(l1, l2):
    i = j = 0
    l = []
    while i < len(l1) and j < len(l2):
        if l1[i] <= l2[j]:
            l.append(l1[i])
            i += 1
        else:
            l.append(l2[j])
            j += 1
    l.extend(l1[i:])
    l.extend(l2[j:])
    return l

if __name__ == '__main__':
    assert merge([], []) == []
    assert merge([1], []) == [1]
    assert merge([], [1]) == [1]
    assert merge([1,1,1], [2,2,2]) == [1,1,1,2,2,2]
    assert merge([1, 4, 5, 6], [2, 3, 5]) == [1, 2, 3, 4, 5, 5, 6]
