"""
Factory is a class or entity that creates other objects.
"""

class Dog:
    def __init__(self):
        pass
    def speak(self):
        print("woof")

class Cat:
    def __init__(self):
        pass
    def speak(self):
        print("meaw")


def Factory(pet="dog"):
    pets = dict(dog=Dog(), cat=Cat())
    return pets[pet]

a = Factory("dog")
a.speak()
b = Factory("cat")
b.speak()
