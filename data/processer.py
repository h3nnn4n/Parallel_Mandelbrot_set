#!/usr/bin/env python
# -*- coding: utf-8 -*-

first = -1

gg = open("time_serial.dat","r")
for j in gg.read().split('\n'):
    a, b = 0, 0
    for d in j.split(' '):
        try:
            a = int(d)
        except:
            try:
                b = float(d)
                if first < 0:
                    first = b
            except:
                pass
            pass
    if a == 0 and b == 0:
        break

    print(a, b, first/b, b/first)

gg.close()

name = "times.dat"

f = open(name)

for j in f.read().split('\n'):
    a, b = 0, 0
    for d in j.split(' '):
        try:
            a = int(d)
        except:
            try:
                b = float(d)
                if first < 0:
                    first = b
            except:
                pass
            pass
    if a == 0 and b == 0:
        break

    print(a, b, first/b, b/first)

f.close()
