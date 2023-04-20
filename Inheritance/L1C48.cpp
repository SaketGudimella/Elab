#include <iostream>
using namespace std;
class teacher{
public: 
int num;
void setdata(int n)
{
if(n==1)
num=10;
else
num=7;
}
void setdata2(int n)
{
if(n==2)
num=3;
else
num=8;
}
void tentable(){
for(int i=1;i<=10;i++)
cout<<num<<"*"<<i<<"="<<num*i<<endl;
}
};
class ten:public teacher{
};
class three:public teacher{
};
class eight:public teacher{
};
class seven:public teacher{
};
int main()
{
int n;
cin>>n;
teacher t;
if(n==1 || n==4)
t.setdata(n);
if(n==2 || n==3)
t.setdata2(n);
t.tentable();
return 0;
}
