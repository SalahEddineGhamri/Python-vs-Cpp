// the humanClass implementation cpp file
// we put the implementation here
#include"humanClass.h" // in the same folder

human::human( string name, int age){
    cout <<"Constructor initiated" << endl;
    this -> name = name;
    this -> age = age;
}
void human::sayName(){
    cout << "Name is : " << name <<endl;
}

void human::sayAge(){
    cout << "Age is : " << age << endl;
}
human::~human(){
    cout << "Human destrector exec"<<endl;
}