#include <iostream>
using namespace std;

int main() {
    int bro1, bro2, bro3;
    cin>>bro1>>bro2>>bro3;

    int tallest;
    if (bro1 > bro2 && bro1 > bro3) {
        tallest = bro1;
    } else if (bro2 > bro1 && bro2 > bro3) {
        tallest = bro2;
    } else {
        tallest = bro3;
    }

    cout<<tallest<< endl;
    return 0;
}
