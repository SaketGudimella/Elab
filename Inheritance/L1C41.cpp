#include <iostream>
using namespace std;
class Bank{
public:
int n;
void get(){
cin>>n;
}
void display(){
cout<<"500: "<<n/500<<endl;
n=n%500;
cout<<"200: "<<n/200<<endl;
n=n%200;
cout<<"100: "<<n/100<<endl;
n=n%100;
cout<<"50: "<<n/50<<endl;
n=n%50;
cout<<"10: "<<n/10<<endl;
n=n%10;
cout<<"5: "<<n/5<<endl;
n=n%5;
cout<<"1: "<<n<<endl;
}
};
class CashCounting:public Bank{
};
int main()
{
CashCounting obj;
obj.get();
obj.display();
return 0;
}
