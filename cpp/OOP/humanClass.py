class human:
    def __init__(self, name="unknown", age=0):
        print("Human object is created ")
        self.name = name
        self.age = age
    def sayName(self):
        print("Name is : %s" % self.name)
    def sayAge(self):
        print("Age is : %i" % self.age)
    def __del__(self):
        print("human object is deleted ")