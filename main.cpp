#include<iostream>
#include<string>
using namespace std;


int pow2gt (int x)	{	--x;	x|=x>>1;	x|=x>>2;	x|=x>>4;	x|=x>>8;	x|=x>>16;	return x+1;	}
int main(){
    char * p=nullptr;
    unsigned int x=10;
    char a[]="0123\n\"";
    string s="123abc";
    cout<<s<<endl;
    return 0;
}