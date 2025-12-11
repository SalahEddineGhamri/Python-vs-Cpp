#include "myclass.hpp"
#include <sstream>

//this is a function friend to person class
void changeAge(person &obj, int age){
	obj.age = age;
}

int i = 0;

int main(){
	
	Birthday birthday(03,05,1988);
	//Citizen SalahEddine("SalahEddine", 30, "Algeria", birthday);
		
	person p1("Salah", 15, birthday);
	person p2(p1);
	
	/*
	person p2(" Eddine", 15, birthday);
	
	person p3 = p1 + p2;
	
	p3.displayBirthday();
	std::cout << p3.getAge() << std::endl;
	std::cout << p3.getName() << std::endl;
	*/
	
	std::cout << p2.getName() << std::endl;
	
	return EXIT_SUCCESS;
}
