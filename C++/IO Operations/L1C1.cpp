#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    float num1,num2; 
    double resnum1,resnum2;
    
    cin >> num1;
    cin >> num2;
    
     resnum1 = (double) num1;
     resnum2 = (double)  num2;
    
    cout << fixed << std::setprecision(6)  << resnum1 << endl;
    cout << fixed << std::setprecision(6) <<  resnum2 << endl;

	return 0;
}
