#!/usr/bin/env python3

# return legacy subnet mask for given cidr prefix length

def cidrmask(n):
    assert n >= 0 and n <= 32
    i = (0xffffffff >> (32-n)) << (32-n)
    ip0 = (i >> 24) & 0xff
    ip1 = (i >> 16) & 0xff
    ip2 = (i >> 8)  & 0xff
    ip3 = (i >> 0)  & 0xff
    return f'{ip0}.{ip1}.{ip2}.{ip3}'

if __name__ == '__main__':
    assert cidrmask(32) == '255.255.255.255'
    assert cidrmask(24) == '255.255.255.0'
    assert cidrmask(16) == '255.255.0.0'
    assert cidrmask(8) == '255.0.0.0'
    assert cidrmask(0) == '0.0.0.0'
