// header file for humanClas file h
// we put only declarations here 
#ifndef HUMANCLASS_H // if humanClass.h is not already defined
#define HUMANCLASS_H // Define it

#include<iostream>
#include<string>
using namespace std;

class human{
    private:
        string name;
        int age;
    public:
        // this is public data
        human(string name="unknown", int age=0);
        void sayName();
        void sayAge();
        ~human();
    protected:
        // this is protected data 
};

#endif // end of #ifndef