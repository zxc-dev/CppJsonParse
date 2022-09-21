#include<iostream>
#include<string>
#include"json.h"
using namespace std;



int main(){
    FILE *f;long len;char *data;
    f=fopen("test.txt","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+2);fread(data,1,len,f);fclose(f);
    data[len+1]=0;
    Json* json= new Json(data);
    json->Print();
    return 0;
}