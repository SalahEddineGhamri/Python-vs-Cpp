#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct person{
    int age; string name;
    person( int Age, string Name){
        age = Age;
        name = Name;
    }
};

vector<person> persones;

int main(){

    persones.emplace_back(20, "Salah");
    persones.emplace_back(40, "Eddine");

    for(int i=0; i<persones.size(); ++i)
        cout << persones[i].name << endl;

    return EXIT_SUCCESS;
}