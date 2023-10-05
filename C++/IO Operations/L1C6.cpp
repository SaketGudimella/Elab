#include <iostream>
using namespace std;

int main() {
    
   int nooffamilymembers,i;
   cin>>nooffamilymembers;
   
 for(i=nooffamilymembers;i>=1;i--) {        
     for(int j=1;j<=i; ++j) {          
         cout<<i<<" ";        
         }        
         cout<<endl; 
 }
    
    return 0;
}
