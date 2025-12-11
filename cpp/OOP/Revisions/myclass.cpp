#include "myclass.hpp"

// constructor & destructor
person::person( std::string name, int age, Birthday bd): bd(bd){
		printf("Person is created\n");
		this -> name = name;
		this -> age = age;
	}

person::person(const person &obj):bd(obj.bd){
	printf("Copied\n");
	this -> name = obj.name;
	this -> age = obj.age;
}

person::~person(){
	printf("person is destroyed\n");
}

std::string person::getName(){
	return this -> name;
}

int person::getAge(){
	return this -> age;
}

int person::halfAge(){
	return (this -> age)/2;
}

int person::doubleAge(){
	return 2 * (this -> age);
}

int person::sepAge(){
	return this -> doubleAge()/this -> halfAge();
}

void person::displayBirthday(){
	this -> bd.display();
}

person person::operator+(person& obj){
	Birthday db(0, 0, 0);
	person result(this->name + obj.name, this->age + obj.age, db);
	return result;
}

Citizen::Citizen(std::string name, int age, std::string country, Birthday bd):person(name, age, bd)
{
	printf("Citizen is constructed\n");
	this -> country = country;
}

void Citizen::showBirthday(){
	this -> displayBirthday();
}


Citizen::~Citizen(){
	printf("Citizen is destoryed\n");
}
