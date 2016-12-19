#include<cmath>
#include<string>
#include<iostream>
#include<cstdio>
#include<map>
using namespace std;

map <char,int> priority;

struct numstack
{
    double n;
    numstack *next;
}*numtop=0;

struct opstack
{
    char ch;
    opstack *next;
}*optop=0;

void pushNum(double n)
{
    struct numstack *newnode=new numstack[sizeof(numstack)];
    newnode->n=n;
    newnode->next=0;
    newnode->next=numtop;
    numtop=newnode;
}

void pushOp(char ch)
{
    struct opstack *newnode=new opstack[sizeof(opstack)];
    newnode->ch=ch;
    newnode->next=0;
    newnode->next=optop;
    optop=newnode;
}

double popNum()
{
    struct numstack *node=numtop;
    double n=numtop->n;
    numtop=numtop->next;
    delete node;
    return n;
}

char popOp()
{
    struct opstack *node=optop;
    char ch=optop->ch;
    optop=optop->next;
    delete node;
    return ch;
}


void display(numstack *head1,opstack *head2)
{
    if(head1!=0)
    {
        while(head1!=0)
        {
            cout<<head1->n<<"  ";
            head1=head1->next;
        }
        cout<<endl;
    }
    if(head2!=0)
    {
        while(head2!=0)
        {
            cout<<head2->ch<<"  ";
            head2=head2->next;
        }
        cout<<endl;
    }
}

bool Operator(char ch)
{
    string operators="+-*/(^";
    if(operators.find(ch)!=string::npos)
        return true;
    return false;
}

bool evaluate(char ch)
{
    if(optop==0)
        return false;
    if(priority[ch]==-1)
        return false;
    cout<<"ch : "<<ch<<" optop : "<<optop->ch<<endl;
    if(priority[ch]>priority[optop->ch])
        return false;
    return true;
}

void initialize()
{
    priority['+']=2;
    priority['-']=2;
    priority['*']=3;
    priority['/']=3;
    priority['^']=4;
    priority['(']=-1;
}
int main()
{
    initialize();
    double a,b;
    char expr[1000];
    cout<<"Enter expression to evaluate : \n\n"; //1+2-3*4+8-1
    cin>>expr;
    cout<<endl;
    int len=strlen(expr);
    for(int i=0;i<len;i++)
    {
        cout<<"character : "<<expr[i]<<endl;
        if(expr[i]==')')
        {
            char ch=popOp();
            while(ch!='(')
            {
                a=popNum();
                b=popNum();

                cout<<"a:"<<a<<" b:"<<b<<" op:"<<ch<<endl;
                if(ch=='+')
                    pushNum(a+b);
                else if(ch=='-')
                    pushNum(b-a);
                else if(ch=='*')
                    pushNum(a*b);
                else if(ch=='/')
                    pushNum(b/a);

                ch=popOp();
            }

        }
        else if(Operator(expr[i]))
        {
            if(evaluate(expr[i]))
            {
                a=popNum();
                b=popNum();
                char ch=popOp();
                cout<<"a:"<<a<<" b:"<<b<<" op:"<<ch<<endl;
                if(ch=='+')
                    pushNum(a+b);
                else if(ch=='-')
                    pushNum(b-a);
                else if(ch=='*')
                    pushNum(a*b);
                else if(ch=='/')
                    pushNum(b/a);
                else if(ch=='^')
                    pushNum( pow((double)b,(double)a));
                pushOp(expr[i]);
            }
            else
                pushOp(expr[i]);
        }
        else
        {
            string num;
            for(int j=i;expr[j]>='0' && expr[j]<='9';j++)
                num+=expr[j];
            pushNum(atoi(num.c_str()));
        }
        display(numtop,0);
        display(0,optop);
    }
    while(optop!=0)
    {
        char ch=popOp();
        a=popNum();
        b=popNum();
        cout<<"a:"<<a<<" b:"<<b<<" op:"<<ch<<endl;
        if(ch=='+')
            pushNum(a+b);
        else if(ch=='-')
            pushNum(b-a);
        else if(ch=='*')
            pushNum(a*b);
        else if(ch=='/')
            pushNum(b/a);
        else if(ch=='^')
            pushNum(pow((double)b,(double)a));
        display(numtop,0);
        display(0,optop);
    }
    cout<<"result = "<<popNum()<<endl;
    return 0;
}