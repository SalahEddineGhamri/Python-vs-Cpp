/* 
This scripts shows the print opetions in C++
author : Salah Eddine Ghamri
*/

#include <iostream> 

using namespace std;

int main(){
    int a = 5;
    int b = 6;
    int c = 7;

    // cout
    cout <<"Hello, world !" << flush;
    cout << "This is this."<< endl;
    
    // printf
    printf ("Characters: %c %c \n", 'a', 65);
    printf ("Decimals: %d %ld\n", 1977, 650000L);
    printf ("Preceding with blanks: %10d \n", 1977);
    printf ("Preceding with zeros: %010d \n", 1977);
    printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
    printf ("Width trick: %*d \n", 5, 10);
    printf ("%s \n", "A string");
    
    return 0;
}