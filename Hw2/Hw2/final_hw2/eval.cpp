#include <stdio.h>
#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;
bool checkOperator(char ch)
{
    if (ch=='*'|| ch=='/' || ch=='+' || ch=='-')
        return true;
    return false;
}

bool checkOperand(char ch)
{
    if(isalpha(ch)&&islower(ch))
        return true;
    return false;
}

string removeSpaces(string infix)
{
    string new_infix="";
    for (int i=0;i<infix.length();i++)
    {
        if (infix[i]!=' ')
            new_infix+=infix[i];
    }
    return new_infix;
}

bool checkParenthesis(string postfix)
{
    for (int i=0;i<postfix.length();i++)
    {
        if (postfix[i]=='(' || postfix[i]==')')
            return true;
    }
    return false;
}
int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if(infix=="")
        return 1;
    postfix="";
    infix=removeSpaces(infix);
    stack<ValueType> operatorStack;
    for (int i=0; i<infix.length();i++)
    {
        char ch=infix[i];
        if(i!=0 && checkOperator(ch)&&checkOperator(infix[i-1]))
            return 1;
        if(i==0 && checkOperator(ch))
           return 1;
        if(i!=0 && checkOperand(ch)&&checkOperand(infix[i-1]))
            return 1;
        if(i==infix.length()-1 && checkOperator(ch))
            return 1;
        switch(ch)
        {
            case '(':
                if(i==0  || (infix[i-1]!=')' &&!checkOperand(infix[i-1]) ))
                    operatorStack.push(ch);
                else
                    return 1;
                break;
            case ')':
                if(operatorStack.empty())
                    return 1;
                if(i!=0&&checkOperator(infix[i-1]))
                    return 1;
                while (!operatorStack.empty() && operatorStack.top()!= '(')
                {
                    postfix+=operatorStack.top();
                    operatorStack.pop();
                }
                if(operatorStack.empty())
                    return 1;
                operatorStack.pop();
                break;
            case '+':
            case '-':
                if(i!=0 &&infix[i-1]=='(')
                    return 1;
                while (!operatorStack.empty() && operatorStack.top()!='(')
                {
                    postfix+=operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(ch);
                break;
            case '*':
            case '/':
                if(i!=0 &&infix[i-1]=='(')
                    return 1;
                while (!operatorStack.empty() && operatorStack.top()!='(' && operatorStack.top()!='-' && operatorStack.top()!='+')
                {
                    postfix+=operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(ch);
                break;
            default:
                if(checkOperand(ch))
                {
                    if(i!=0&&infix[i-1]==')')
                        return 1;
                    postfix+=ch;
                }
                else
                {
                    return 1;
                }
                break;
        }
    }
    while (!operatorStack.empty())
    {
        postfix+=operatorStack.top();
        operatorStack.pop();
    }
    if (postfix=="")
        return 1;
    if(checkParenthesis(postfix))
        return 1;
    
    ///evaluation
    stack<char> operandStack;
    for (int i=0; i<postfix.length();i++)
    {
        char ch=postfix[i];
        if(checkOperand(ch))
        {
            if(values.contains(ch))
            {
                ValueType v;
                values.get(ch, v);
                operandStack.push(v);
            }
            else
                return 2;
        }
        else
        {
            int operand2=operandStack.top();
            operandStack.pop();
            int operand1=operandStack.top();
            operandStack.pop();
            switch(ch)
            {
                case '+':
                    operandStack.push(operand1+operand2);
                    break;
                case '-':
                    operandStack.push(operand1-operand2);
                    break;
                case '*':
                    operandStack.push(operand1*operand2);
                    break;
                case '/':
                    if(operand2==0)
                        return 3;
                    operandStack.push(operand1/operand2);
                    break;
            }
        }
    }
    result=operandStack.top();
    return 0;
}
       
/*
int main()
{

    Map a;
    a.insert('a', 1);
    a.insert('b', 2);
    string test="a+a*b";
    string finalTest="";
    int c;
    evaluate(test, a, finalTest, c);
    cout<<finalTest;
    cout<<c;
    
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y',   't', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1,      0};
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
                            pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
      // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
                            pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                            pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                            pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    assert(evaluate(")(", m, pf, answer) == 1  &&
    pf == ""  &&  answer == 3);
    //int tester =evaluate("(-a)", m, pf, answer);
    //cout <<"test"<<tester<<endl;
    assert(evaluate("(-a)", m, pf, answer) == 1  && pf == ""  &&  answer == 3);
    assert(evaluate("-a)", m, pf, answer) == 1  && pf == ""  &&  answer == 3);
    cout << evaluate("a", m, pf, answer)<<endl;
    assert(evaluate("a", m, pf, answer) == 0  && pf == "a"  &&  answer == 3);
    assert(evaluate("a+ (o-u) /(y-a ", m, pf, answer) == 1 &&  answer == 3);
    assert(evaluate("ab", m, pf, answer) == 1   &&  answer == 3);
    assert(evaluate("a+ (o-u) /y*b-a ", m, pf, answer) == 2 &&  answer == 3);
    assert(evaluate("a+ (o-u) /y*b)-a ", m, pf, answer) == 1 &&  answer == 3);
    assert(evaluate("a+ ((o-u) /y*b-a ", m, pf, answer) == 1 &&  answer == 3);
    cout <<evaluate("a/t*t", m, pf, answer)<<endl;
    assert(evaluate("a/t*t", m, pf, answer) == 3  && pf == "at/t*"  &&  answer == 3);
    assert(evaluate("#/t*t", m, pf, answer) == 1   &&  answer == 3);


    assert(evaluate("b", m, pf, answer) == 2 &&  answer == 3);
    cout << "Passed all tests" << endl;
}
**/
