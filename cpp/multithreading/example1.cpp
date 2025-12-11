#include <iostream>
#include <thread>
#include <mutex>

using namespace std;


int myAmount = 0;
mutex m;

void addMoney(){
  m.lock();
  // critical section 
  ++myAmount;
  m.unlock();
}

int main(){
  thread t1(addMoney);
  thread t2(addMoney);

  t1.join();
  t2.join();

  if (myAmount == 2){
  cout << "2 means that t1 and t2 both updated sequencially :  " << myAmount << endl;
  } else {
  cout << "1 means that t1 and t2 are in race condition to change myAmount which is unwanted behavior : "<< myAmount << endl;
  }
  return 0;
}
