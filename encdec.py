#!/usr/bin/env python3

# little endian encode/decode

def dec32(b):
    return b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24)

def dec64(b):
    return dec32(b[:4]) | (dec32(b[4:]) << 32)

def enc32(v):
    b0 = v & 0xff
    b1 = (v >> 8) & 0xff
    b2 = (v >> 16) & 0xff
    b3 = (v >> 24) & 0xff
    return bytes([b0, b1, b2, b3])

def enc64(v):
    b0 = v & 0xff
    b1 = (v >> 8) & 0xff
    b2 = (v >> 16) & 0xff
    b3 = (v >> 24) & 0xff
    b4 = (v >> 32) & 0xff
    b5 = (v >> 40) & 0xff
    b6 = (v >> 48) & 0xff
    b7 = (v >> 56) & 0xff
    return bytes([b0, b1, b2, b3, b4, b5, b6, b7])

if __name__ == '__main__':
    for i in [0x0, 0x7f, 0x80, 0xff, 0xffff, 0xff00ff00, 0xffffffff]:
        assert dec32(enc32(i)) == i
        assert dec64(enc64(i)) == i
    for i in [0xff00ff00ff00ff00, 0xffffffffffffffff]:
        assert dec64(enc64(i)) == i
