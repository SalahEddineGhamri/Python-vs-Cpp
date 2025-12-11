// this is the main script of our program
#include "humanClass.h"

int main(){
    // Direct declaration
    human Salah( "Salah", 30);
    human SalahCopy(Salah);
    SalahCopy.sayName();
    SalahCopy.sayAge();
    // Declaration through dynamic allocation
    human *Eddine = new human("Eddine", 45);
    Eddine ->sayAge();
    Eddine ->sayName();
    // Pointer of a class
    human *ptrHuman;
    ptrHuman = &Salah;
    ptrHuman ->sayName();
    ptrHuman ->sayAge();
    //
    human *ptrHuman2 = Eddine;
    ptrHuman2 ->sayName();
    ptrHuman2 ->sayAge();
    return 0;
}