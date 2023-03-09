#include <iostream>
using namespace std;

int main() {
    int husage,wfage,coupleavgage;
    cin>>husage>>wfage;
    coupleavgage = (husage + wfage) / 2;
    cout<< "I am " << husage << endl;
    cout<< "You are " << wfage << endl;
    cout<< "We are around " << coupleavgage << endl;
    return 0;
}
