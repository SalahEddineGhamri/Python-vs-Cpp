#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void getNumber(int& number1, int& number2, ifstream& file);
void pushResults(int& result, ofstream& file);

int main(){
    int nbr1, nbr2, result = 0;
    ifstream myfile_in("The_file.txt");
    getNumber(nbr1, nbr2, myfile_in);
    myfile_in.close();
    result = nbr1 * nbr2;
    cout << "The result is " << result << endl;
    ofstream myfile_out("The_file.txt", std::ios::app);
    pushResults(result, myfile_out);
    myfile_out.close();
    return 0;
}


void getNumber(int& number1, int& number2, ifstream& file){
    int i= 0;
    string line;
    if (file.is_open()){
        while ( getline(file, line) ){
            if ( i == 1 ){
                number1 = stoi(line);
                cout << "The first numbr is " 
                     << number1 << endl;
            } else if( i == 3) {
                number2 = stoi(line);
                cout << "The second numbr is " 
                     << number2 << endl;
            }
            i += 1;
        }
    }
}

void pushResults(int& result, ofstream& file){
    if (file.is_open()) {
        file << result << ".\n";
    }
}