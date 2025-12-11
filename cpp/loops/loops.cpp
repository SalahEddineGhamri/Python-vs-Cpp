#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int main(){
    printf("\n;");
    // for loop
    
    for(int i=0; i<10; i++){
        printf("for now is %d\t", i);
    }
    printf("\n");

    for (size_t j = 0; j < 10; j++) {
      std::cout << "do" << j << std::endl;
    }

    //
    int i = 0;
    while( i<=10){
        printf("while now is %d\t", i);
        i += 1;
    }
    printf("\n");

    // while...do
    i = 0;
    do{
        printf("whiledo now is %d\t", i);
        i+=1;
    }while(i < 8);
    printf("\n");

    // playing with continue and break
    i = 0;
    while(i<10){
        i+=1;
        if( i==4 ){
            continue;
        }
        cout << i << "\t";
        if( i==5 )
            break;
    }
    cout<<endl;
    return 0;
}