#!/usr/bin/python3
# -*- coding: utf-8 -*-

first = 134.975565

files = [10, 50, 100, 200, 400, 600]

data = [ [] for i in files]

for (ind,k) in enumerate(files):
    f = open("time_bs_" + str(k))
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

#        print(a, b, (first/1)/b)
        data[ind].append((a, b, (first/1)/b))

    f.close()

#for i in data:
#    print(i)

for k,i in enumerate(data):
   print(files[k],end="") 
   for k in range(3):
       print(" ", i[k][1]) 

