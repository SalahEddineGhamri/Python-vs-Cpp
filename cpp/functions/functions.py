#!/usr/bin/python

# *args and **kwargs
def func(*args, **kwargs):
    if args or kwargs:
        if args:
            print("an argument is presentd")
            print("the argument is %r" % args[0])
        if kwargs:
            print(kwargs)
    else:
        print("no arg argument")

if __name__ == "__main__":
    func(22)

