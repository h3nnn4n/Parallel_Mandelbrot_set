#!/usr/bin/env python
# -*- coding: utf-8 -*-

names = ["omp_static", "omp_dynamic", "omp_guided", "threads"]

f = open("time_main")
x = f.read().split('\n')[0]
try:
    main = float(x)
except Exception:
    print(x)

f.close()

for n in names:
    w = open("time_" + n, 'w')
    for i in range(16):
        name = "time_" + str(i + 1) + "_main_" + n
        #print(i+1)
        #print(name)

        f = open(name)

        x = 0
        k = 0
        for j in f.read().split('\n'):
            try:
                x += float(j)
                k += 1
            except Exception:
                pass

        w.write("%d %f %f\n" %(i+1, x/(k), main/(x/(k))))

        f.close()

    w.close()
