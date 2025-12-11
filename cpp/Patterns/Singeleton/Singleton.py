"""
class Borg:
    _shared_state = {}

    def __init__(self):
        self.__dict__ = self._shared_state # use dict as an object

class Singleton(Borg):
    def __init__(self, **kwargs):
        Borg.__init__(self)
        self._shared_state.update(kwargs)
    def __str__(self):
        return str(self._shared_state)


a = Singleton(A = 1)
b = Singleton(B = 2)
c = Singleton(C = 3)
d = Singleton(4

print(a.B)
print(b.B)
print(c.B)
print("a and b and c and d point to the same object")
"""
class Borg:
    _shared_data = [0]

    def __init__(self):
        pass

class Singleton(Borg):
    def __init__(self, *args):
        Borg.__init__(self)
        if args:
            self._shared_data[0] = args[0]
    def __str__(self):
        return str(self._shared_data[0])

a = Singleton(1)
b = Singleton(2)
c = Singleton(3)
d = Singleton()

print(a)
print(b)
print(c)