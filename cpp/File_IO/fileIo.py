#!/usr/bin/python

#read entire file
"""
try:
    file = open("The_file.txt", 'r')
    print(file.read())
finally:
    file.close()
"""
#read specific lin
"""
try:
    number1, number2 = None, None
    file = open("The_file.txt", 'r')
    for i, line in enumerate(file):
        if i==1 : number1 = int(line)
        if i==3 : number2 = int(line)
        if number1 and number2:
            break

    file = open("The_file.txt", 'a')
    file.write(str(number1 * number2))
finally:
    file.close()
"""
# the correct way
"""
def Doit(filename):
    file = open(filename, 'r+')
    content = file.readlines()
    nbr1, nbr2 = int(content[1]), int(content[3])
    result = str(nbr1*nbr2)
    file.write(result)
    file.close()

Doit("The_file.txt")
"""

def Doit(filename):
    file = open(filename, 'r+')
    content = file.readlines()
    nbr1, nbr2 = int(content[1]), int(content[3])
    result = str(nbr1*nbr2)
    content.append(result)
    file = open(filename, 'w')
    file.writelines(content)
    file.close()

Doit("The_file.txt")