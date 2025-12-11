# this script shows all print options in python
# author: Salah Eddine Ghamri

# *-- print a string
print(" 1 - Hello, world !")
print(' 2 - Hello, world !')
print(" 3 - Hello," + " world !")

# *-- String fromating
print(" 4 - %s " % "Hello, world !") # %s %r %d \n \t
print( " %d - " % 5)
print( " %r - " % 6)
print( " %d %s %s %s " % (7, "-", "Hello,", "world !"))

# *-- the best
print( ' %(subject)s %(verb)s %(thing)s.' % {'subject': "You", 'verb': "are", 'thing': "not a thing"})
print(" 8 - {} ".format("Hello, world !"))

#*-----
print(" 9 - Hello,", end='')
print(" world !")
#-------
print(" 10- This is ", 2**32, ", it's a magical number.", sep="-*****-")