#include <iostream>
#include <string>

class Color{
  std::string name;
  public:
  Color(std::string name): name(name){};
  virtual std::string getName(){
    return this->name;
  }
};

class Shape{
  std::string name;
public:
  Shape(std::string name): name(name){};
  virtual std::string getName(){
    return this->name;
  }
};


class Object:public Shape, public Color
{
  std::string name;
  public:
  Object(std::string name, std::string shape, std::string color): name(name), Shape(shape), Color(color){
    std::cout << "an object is born" << std::endl;
  }

  virtual std::string getName() override {
    return this->name;
  }

  std::string getShape(){
    return Shape::getName();
  }

  std::string getColor(){
    return Color::getName();
  }
};

void ITakeShape(Shape &a){ //virtuals work only for reference and pointers
  std::cout << a.getName() << std::endl;
}

void ITakeColor(Color a){
  std::cout << a.getName() << std::endl;
}

int main(){
  Object a("door", "square", "black");
  ITakeShape(a); // will get the name of a
  ITakeColor(a); // will get color instead of a

  return 0;
}
