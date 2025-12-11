#!/usr/bin/python

#range is a list generator

# starts from 0 and gives 10 so it will reach only 9
for i in range(10):
    print("for now is %d" % i, end=' => ')
else: print('')

# starts from 1 and till 10 and step 1
for i in range(1, 11, 1):
    print("for now is %d" % i, end=' => ')
else: print('')

#while
i = 0
while(i<10):
    print("for now is %d" % i, end=' => ')
    i+=1
else: print('')

# playing with break and continue
i = 0
while(i<10):
    i+=1
    if(i == 2): continue
    print("for now is %d" % i, end=' => ')
    if(i == 4): break
else: print('')