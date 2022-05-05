#!/usr/bin/env python3

# varint encode/decode

def enc(v):
    l = []
    while v >= 128:
        l.append((v & 127) | 128)
        v >>= 7
    l.append(v)
    return bytes(l)

def dec(b):
    v = 0
    for i in range(len(b)):
        v |= (b[i] & 127) << 7*i
    return v
