#include <iostream>
using namespace std;
int main()
{
  int N,c;    
  cin>>N;    
  for(int i=1;i<=N;i++) {
      if(i%2==0)        
      c=2;       
      else       
      c=1; 
      for(int j=1;j<=i;j++){      
          cout<<c<<" ";        
          c+=2;
          
      }
      
      cout<<endl; 
  }  
	return 0;
}
