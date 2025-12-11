#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int nbr = 1, nbr1 = 0, nbr2 = 0, result=0;
int main()
{
    fstream file("The_file.txt", ios::in | ios::out);
    string line;
    if( file.is_open()){
        while (getline(file, line))
        {
            cout << nbr << " " << line << endl;
            if( nbr == 2 ){ nbr1 = stoi(line);}
            if( nbr == 4 ){ nbr2 = stoi(line);}
            if( nbr==5 && nbr1 && nbr2 ){
                result = nbr1 * nbr2;
                file << result;
                cout << result << endl;
            }
            nbr += 1;
        }
    }else{ cout << " Errors in opening file "<< endl; }
    file.close();
    return 0;
}