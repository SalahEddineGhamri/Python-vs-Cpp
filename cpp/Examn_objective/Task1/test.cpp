#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
using namespace std;

vector<int> Window = { 7, 8, 1, 99, 10 , 12, 0, 4, 5};

int main(int args, char ** argv) {
    // Order elements (only half of them)
    int mid = (Window.size()%2 == 0) ? Window.size()/2 : (Window.size()/2)+1;
    cout << mid << endl;
    for (int j = 0; j < mid ; ++j)
    {
        int min = j;
        for (int k = j + 1; k < Window.size(); ++k){
            if (Window[k] < Window[min]){
                min = k;
            }
        }
        const int temp = Window[j];
        Window[j] = Window[min];
        Window[min] = temp;
    }
    for (auto &i:Window)
    cout << i << " ";
    cout << endl;
}
