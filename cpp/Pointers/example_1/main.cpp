#include <iostream>
#include <vector>
#include <functional>

class Object {
  int n;

public:
  Object(int a) : n{a} {};

  int getNumber(){
    return this->n;
  }

  void setNumber(int a){
    this->n = a;
  }
};

int main() { 
  Object a(1);
  Object b(2);
  Object c(3);

  // array of references
  std::reference_wrapper<Object> ap[] = {a, b, c};

  // this will change only ap elements not a and b and c
  for (auto p: ap){
    p.get().setNumber(10);
  }

  // vector of pointers
  std::vector<Object*> Ap{&a, &b, &c};
  
  for (auto p: ap){
    std::cout << p.get().getNumber() << std::endl;
  }

  for (auto p: Ap){
    std::cout << p->getNumber() << std::endl;
  }

  return 0; }

