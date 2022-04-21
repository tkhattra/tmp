#!/usr/bin/env python3

# Accurately computing running variance
# http://www.johndcook.com/standard_deviation.html

import math

class Welford(object):
    def __init__(self):
        self.num = 0
        self.min = self.max = 0

    def add(self, x):
        self.num += 1
        if self.num == 1:
            self.oldS = 0
            self.oldM = self.newM = self.min = self.max = x
        else:
            self.newM = self.oldM + (x - self.oldM) / self.num
            self.newS = self.oldS + (x - self.oldM) * (x - self.newM)
            self.oldM = self.newM
            self.oldS = self.newS
            self.min = min(self.min, x)
            self.max = max(self.max, x)

    def mean(self):
        if self.num == 0:
            return 0
        return self.newM

    def var(self):
        if self.num <= 1:
            return 0
        return self.newS / (self.num - 1)

    def sdev(self):
        return math.sqrt(self.var())
