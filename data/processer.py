#!/usr/bin/env python
# -*- coding: utf-8 -*-

name = "times.dat"

f = open(name)

first = -1
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
