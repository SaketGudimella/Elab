#include <iostream>
using namespace std;

int main() {
    int number, sum = 0, temp;
    cin>>number;
    temp = number;
    while (temp > 0) {
        int digit = temp % 10;
        sum += digit * digit * digit;
        temp /= 10;
    }
    if (sum == number) {
        cout << "Part of Memorable Coin";
    } else {
        cout << "Not a Part of Memorable Coin";
    }
    return 0;
}
