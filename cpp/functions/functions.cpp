#include <iostream>
using namespace std;

int func(){
    cout << "func is called " << endl;
    return 0;
}

int func(int a, int b){
    return (a+b);
}

int main(){
    printf("func is %d \n", func());
    printf("func is %d \n", func( 5, 6));
    return EXIT_SUCCESS;
}