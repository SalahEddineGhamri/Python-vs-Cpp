"""
Abstract Factory is a class or entity that houses a factory of object.
"""

class Dog:
    """Concret Dog class"""
    def __init__(self):
        pass
    def speak(self):
        return "woof"

class Cat:
    """Concrete Cat class"""
    def __init__(self):
        pass
    def speak(self):
        return "meaw"


class DogFactory():
    """ Concrete dog Factory"""
    def __init__(self):
        self.dog = Dog()

    def type(self):
        return "dog"

    def speak(self):
        return self.dog.speak()
        
    def food(self):
        return "dog food"

class CatFactory():
    """ Concrete cat Factory"""
    def __init__(self):
        self.cat = Cat()

    def type(self):
        return "cat"

    def speak(self):
        return self.cat.speak()
        
    def food(self):
        return "cat food"

class PetStore():
    """PetStore is abstract factory """
    def __init__(self, pet_factory):
        self.pet_factory = pet_factory

    def show(self):
        print("we are selling ", self.pet_factory.type())
        print("our sound is ", self.pet_factory.speak())
        print("our food is ", self.pet_factory.food())


dogfactory = DogFactory()
catfactory = CatFactory()
ps = PetStore(dogfactory)
ps.show()
ps = PetStore(catfactory)
ps.show()


