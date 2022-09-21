#ifndef C94849BF_3285_494D_835D_033AB6BD9D6A
#define C94849BF_3285_494D_835D_033AB6BD9D6A

#include"json.h"
#include <stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;
enum JsonType {
    JsonFalse,
    JsonTrue,
    JsonNULL,
    JsonNumber,
    JsonString,
    JsonArray,
    JsonObject
};

class Json
{
private:
    /* data */
    struct ArrayElem
    {
        /* data */
        ArrayElem *next;
        Json *elem;
    };
    struct ObjectElem
    {
        /* data */
        ObjectElem* next;
        char *name;
        Json *value;
    };

    JsonType type;
    int length;
    int capacity;
    int depth;
    double num;
    char *str;
    ArrayElem *arrayHead;
    ObjectElem *objHead;
   
    
    char* ParseNumber(char *p);
    char* ParseString(char *p);
    char* ParseArray(char *p);
    char* ParseObj(char* p);
    char* Skip(char *p);

    void ClearNumber();
    void ClearString();
    void ClearArray();
    void ClearObj();
    void AddChar(char ch);

    char* ArrayAddElem(char *p);
    char* ObjAddElem(char *p);
    //考虑实现错误处理函数
    void PrintDepth();
    void PrintNumber();
    void PrintString();
    void PrintArray();
    void PrintObject();
public:
    Json(/* args */);
    Json(char *p);
    ~Json();
    char* Parse(char *p,int curDepth);
    void Clear();
    void Print();
    int GetLength();
};


Json::Json(/* args */)
{
    Clear();
}

Json::Json(char *p)
{
    Clear();
    Parse(p,0);
}


Json::~Json()
{
    Clear();
}

char* Json::Skip(char *p)//考虑实现跳过注释
{
    while(p&&*p&&*p<33){
        p++;
    }
    if(*p=='/'&&*(p+1)=='/'){
        while(p&&*p&&*p!='\n'){
            p++;
        }
        if(*p=='\n'){
            p++;
            p=Skip(p);
        }
    }
    if(*p=='/'&&*(p+1)=='*'){
        while(p&&*p&&!(*p=='*'&&*(p+1)=='/')){
            p++;
        }
        if(*p=='*'&&*(p+1)=='/'){
            p+=2;
            p=Skip(p);
        }
    }
    return p;
}

char* Json::Parse(char *p,int curDepth)
{
    Clear();
    depth=curDepth+1;
    if(!p) return nullptr;
    p=Skip(p);
    
    switch (*p)
    {
    case 'n':
        if(*(p+1)=='u'&&*(p+2)=='l'&&*(p+3)=='l'){
            type=JsonNULL;
        }
        return p+4;

    case 't':
        if(*(p+1)=='r'&&*(p+2)=='u'&&*(p+3)=='e'){
            type=JsonTrue;
        }
        return p+4;
        
    case 'f':
        if(*(p+1)=='a'&&*(p+2)=='l'&&*(p+3)=='s'&&*(p+4)=='e'){
            type=JsonFalse;
        }
        return p+5;
       
    case '\"':
        return ParseString(p);
       
    case '[':
        return ParseArray(p);
       
    case '{':
        return ParseObj(p);
       
    default:
        if(*p=='-'||(*p>='0'&&*p<='9')){
            return ParseNumber(p);
        }
        break;
    }
    return nullptr;
}

char* Json::ParseNumber(char *p)
{
    int sign;
    double res=0;
    type=JsonNumber;
    if(*p=='-'){
        sign=-1;
        p++;
    }
    else{
        sign=1;
    }
    while(*p>='0'&&*p<='9'){
        res=res*10+*p-'0';
        p++;
    }
    if(*p=='.'){
        p++;
        double flag=10;
        while(*p>='0'&&*p<='9'){
            res+=(*p-'0')/flag;
            flag*=10;
            p++;
        }
    }//可以考虑实现指数识别 如1e7
    num=res*sign;
    return p;
}

char* Json::ParseString(char *p)
{
    type=JsonString;
    p++;
    while(*p){
        if(*p=='\"'){//有改进空间，可以尝试识别转义的引号
            p++;
            break;
        }
        AddChar(*p);
        p++;
    }
    return p;
}

char* Json::ParseArray(char *p)
{
    type=JsonArray;
    p++;
    p=Skip(p);
    while(p&&*p&&*p!=']'){
        switch (*p)
        {
        case ',':
            p++;
            break;
        default:
            p=ArrayAddElem(p);
            break;
        }
        p=Skip(p);
    }
    if(*p==']'){
        p++;
        return p;
    }
    else{
        return nullptr;
    }
}

char *Json::ParseObj(char *p)
{
    type=JsonObject;
    p++;
    p=Skip(p);
    while(p&&*p&&*p!='}'){
        switch (*p)
        {
        case ',':
            p++;
            break;
        case '\"':
            p=ObjAddElem(p);
            break;
        default:
            return nullptr;
            break;
        }
        p=Skip(p);
    }
    if(*p=='}'){
        p++;
        return p;
    }
    else {
        return nullptr;
    }
}

void Json::Clear()
{
    switch (type)
    {
    case JsonString:
        ClearString();
        break;
    case JsonArray:
        ClearArray();
        break;
    case JsonObject:
        ClearObj();
        break;
    default:
        break;
    }
    type=JsonNULL;
    length=0;
    capacity=0;
    depth=0;
    num=0;
    str=nullptr;
    arrayHead=nullptr;
    objHead=nullptr;
}

void Json::AddChar(char ch){
    if(!str){
        str=(char*)malloc(2);
        if(!str) return;
        capacity=1;
        memset(str,0,capacity+1);
    }
    if(length>=capacity){
        char* tmp=str;
        capacity*=2;
        str=(char*)malloc(capacity+1);
        if(!str) return;
        memset(str,0,capacity+1);
        strcpy(str,tmp);
        free(tmp);
    }
    str[length++]=ch;
}

char* Json::ArrayAddElem(char *p){
    
    ArrayElem* q=arrayHead;
    
    if(!arrayHead){
        arrayHead=(ArrayElem*)malloc(sizeof(ArrayElem));
        if(!arrayHead) return nullptr;
        memset(arrayHead,0,sizeof(ArrayElem));
        q=arrayHead;
    }
    else{
       
        while(q->next){
            q=q->next;
        }
        q->next=(ArrayElem*)malloc(sizeof(ArrayElem));
        q=q->next;
        if(!q) return nullptr;
        memset(q,0,sizeof(ArrayElem));
       
    }

    q->elem=new Json();
    p=q->elem->Parse(p,depth);
    
    if(p)
    length++;
    return p;
}

char* Json::ObjAddElem(char *p){
    
    p++;
    char *tmp=p;
    int len=0;
    while(p&&*p&&*p!='\"'){//考虑能识别转义的引号
        p++;
        len++;
    }
    if(*p!='\"') return nullptr;
    p=tmp;

    ObjectElem* q=nullptr;
    if(!objHead){
        objHead=(ObjectElem*)malloc(sizeof(ObjectElem));
        if(!objHead) return nullptr;
        memset(objHead,0,sizeof(ObjectElem));
        q=objHead;
    }
    else{
        q=objHead;
        while(q->next){
            q=q->next;
        }
        q->next=(ObjectElem*)malloc(sizeof(ObjectElem));
        q=q->next;
        if(!q->next) return nullptr;
        memset(q,0,sizeof(ObjectElem));
    }

    q->name=(char *)malloc(len+1);
    if(!q->name) return nullptr;
    memset(q->name,0,len+1);

    for(int i=0;i<len;++i){
        *(q->name+i)=*p;
        p++;
    }
    p++;
    p=Skip(p);
    if(*p!=':'){
        return nullptr;
    }
    p++;
    q->value=new Json();
    p=q->value->Parse(p,depth);

    if(p)
    length++;
    return p;
}

int Json::GetLength()
{
    return length;
}

void Json::Print(){
    PrintDepth();
    switch (type)
    {
    case JsonNULL:
        cout<<"null";
        break;
    case  JsonFalse:
        cout<<"false";
        break;
    case  JsonTrue:
        cout<<"true";
        break;   
    case JsonNumber:
        PrintNumber();
        break;
    case JsonString:
        PrintString();
        break;
    case JsonArray:
        PrintArray();
        break;
    case JsonObject:
        PrintObject();
        break;
    default:
        break;
    }
}

void Json::PrintDepth()
{
    for(int i=0;i<depth;++i){
        cout<<'\t';
    }
}

void Json::PrintNumber()
{
    cout<<num;
}

void Json::PrintString()
{
    char *p=str;
    cout<<'\"';
    for(int i=0;i<length;++i){
        cout<<*p;
        p++;
    }
    cout<<'\"';
}

void Json::PrintArray()
{
    cout<<'['<<endl;
    ArrayElem *p=arrayHead;
    for(int i=0;i<length;++i){
        PrintDepth();
        cout<<'\t';
        p->elem->Print();
        cout<<','<<endl;
        p=p->next;
    }
    PrintDepth();
    cout<<']'<<endl;
}

void Json::PrintObject()
{
    cout<<'{'<<endl;
    ObjectElem *p=objHead;
    for(int i=0;i<length;++i){
        PrintDepth();
        cout<<'\t';
        char *n=p->name;
        cout<<'\"';
        while(n&&*n){
            cout<<*n;
            n++;
        }
        cout<<"\" : ";    
        p->value->Print();
        cout<<','<<endl;
        p=p->next;
    }
    PrintDepth();
    cout<<'}'<<endl;
}

void Json::ClearString()
{
    free(str);
}

void Json::ClearArray()
{
    ArrayElem *p=arrayHead;
    while(p){
        ArrayElem* q=p->next;
        delete p->elem;
        free(p);
        p=q;
    }
}
void Json::ClearObj()
{
    ObjectElem *p=objHead;
    while(p){
        ObjectElem *q=p->next;
        free(p->name);
        delete p->value;
        free(p);
        p=q;
    }
}
#endif /* C94849BF_3285_494D_835D_033AB6BD9D6A */
