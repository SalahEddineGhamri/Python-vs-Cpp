#ifndef MYCLASS_HPP // 
#define MYCLASS_HPP // 

#include <iostream>
#include <string>


class Birthday{
public:
	int day;
	int month;
	int year;
	Birthday(int day, int month, int year):day(day), month(month), year(year){
	}
	
	void display(){
		printf("Birthday is: %d - %d - %d \n", day, month, year);
	}
};

class person
{
public:
	
	std::string name;
	int age;
	Birthday bd;

	person( std::string name, int age, Birthday bd);
	std::string getName();
	int sepAge();
	int getAge();

	person(const person &obj);
	
	//friend function
	friend void changeAge(person &obj, int age);
	
	//Operator overloading
	person operator+(person &obj);
	
	~person();

private:
	int halfAge();

protected:
	void displayBirthday();
	int doubleAge();

};


class Citizen : public person 
{
public:
	
	Citizen(std::string name, int age, std::string country, Birthday bd);
	void showBirthday();
	~Citizen();

private:
	std::string country;
};

#endif
