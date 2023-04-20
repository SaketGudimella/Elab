#include <iostream>
using namespace std;
class ChangeBase 
{
public:
void sumBase(int n,int k) 
{
int res = 0;
while (n > 0) {
res += (n % k);
n /= k;
}
cout<<res;
}
};
int main()
{int n,k;
ChangeBase Convert;
cin>>n>>k;
Convert.sumBase(n,k);
return 0;
}
