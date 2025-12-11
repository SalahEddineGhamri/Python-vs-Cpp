#include<vector>
#include<iostream>
#include<string>
using namespace std;

vector<string> names;

int main(){
    names.push_back("Salah");
    names.push_back("Eddine");
    names.push_back("Ghamri");
    names.pop_back();
    cout << names.size() << endl;
    // like array
    for (int i=0 ; i < names.size(); ++i){
        cout << i << ' ';
        cout << names[i] << endl;
    }
    // like python list
    for (auto element = names.begin(); element != names.end(); ++element)
        cout << *element << endl;
    
    // front and back
    cout << names.front() << endl; // a reference
    cout << names.back() << endl; // a reference

    // point to the firt element
    string * pos = names.data();
    cout << *pos << ' ';
    cout << *++pos <<endl;

    return EXIT_SUCCESS;
}