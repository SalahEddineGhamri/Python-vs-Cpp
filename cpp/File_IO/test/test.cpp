#include <bits/stdc++.h>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    int index;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Write Your Code Here
    int nbr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    string words[10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    /*
    for(int i=0; i<10; i++){
        cout<< nbr[i] << " " << words[i] << endl; 
    }
    */
    if(n<=9){
    index =  distance(nbr, find(nbr, nbr + 10, n));
    cout << words[index] << endl;
    }else{
        cout << "Greater than 9" << endl;
    }

    return 0;
}