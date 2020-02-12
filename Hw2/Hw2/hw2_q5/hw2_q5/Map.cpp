//
//  Map.cpp
//  hw2_q5
//
//  Created by Austin Wang on 2/3/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include "Map.h"

Map::Map()
 : m_size(0)
{
      // create dummy node
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

Map::~Map()
{
      // Delete the m_size non-dummy nodes plus the dummy node

    for (Node* p = m_head->m_prev ; m_size >= 0; m_size--)
    {
        Node* toBeDeleted = p;
        p = p->m_prev;
        delete toBeDeleted;
    }
}

Map::Map(const Map& other)
 : m_size(other.m_size)
{
      // Create dummy node; don't initialize its pointers

    m_head = new Node;

      // Initialize prev to last node created

    Node* prev = m_head;

      // Copy each non-dummy node from the other list; each iteration will set
      // the m_next of the previous node copied

    for (Node* p = other.m_head->m_next ; p != other.m_head; p = p->m_next)
    {
          // Create a copy of the node p points to
        Node* pnew = new Node;
        pnew->m_key = p->m_key;
        pnew->m_value = p->m_value;

          // Connect the new node to the previous one
        pnew->m_prev = prev;
        prev->m_next = pnew;

          // Reset prev to last node created
        prev = pnew;
    }

      // Connect last node created to m_head
    m_head->m_prev = prev;
    prev->m_next = m_head;
}

Map& Map::operator=(const Map& rhs)
{
    if (this != &rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Map::erase(const KeyType& key)
{
    Node* p = find(key);

    if (p == m_head)  // not found
        return false;

      // unlink the node from the list and destroy it

    p->m_prev->m_next = p->m_next;
    p->m_next->m_prev = p->m_prev;
    delete p;

    m_size--;
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* p = find(key);
    if (p == m_head)  // not found
        return false;
    value = p->m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size)
        return false;

      // Return the key and value at position i.  This is one way of ensuring
      // the required behavior of get:  If the Map doesn't change in the
      // interim,
      // * calling get with each i in 0 <= i < size() gets each of the
      //   Map pairs, and
      // * calling get with the same value of i each time gets the same pair.

      // If i is closer to the head of the list, go forward to reach that
      // position; otherwise, start from tail and go backward.

    Node* p;
    if (i < m_size / 2)  // closer to head
    {
        p = m_head->m_next;
        for (int k = 0; k != i; k++)
            p = p->m_next;
    }
    else  // closer to tail
    {
        p = m_head->m_prev;
        for (int k = m_size-1; k != i; k--)
            p = p->m_prev;
    }

    key = p->m_key;
    value = p->m_value;
    return true;
}

void Map::swap(Map& other)
{
      // swap head pointers
    Node* tempHead = m_head;
    m_head = other.m_head;
    other.m_head = tempHead;

      // swap sizes
    int t = m_size;
    m_size = other.m_size;
    other.m_size = t;
}

Map::Node* Map::find(const KeyType& key) const
{
      // Do a linear search through the list

    Node* p;
    for (p = m_head->m_next; p != m_head && p->m_key != key; p = p->m_next)
        ;
    return p;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
                           bool mayInsert, bool mayUpdate)
{
    Node* p = find(key);

    if (p != m_head)  // found
    {
        if (mayUpdate)
            p->m_value = value;
        return mayUpdate;
    }
    if (!mayInsert)  // not found, and not allowed to insert
        return false;

       // Create a new node
    p = new Node;
    p->m_key = key;
    p->m_value = value;

      // Insert new item at tail of list (arbitrary choice of position)
      //     Connect it to tail
    p->m_prev = m_head->m_prev;
    p->m_prev->m_next = p;

      //     Connect it to dummy node
    p->m_next = m_head;
    m_head->m_prev = p;

    m_size++;
    return true;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
      // For better performance, the bigger map should be the basis for
      // the result, and we should iterate over the elements of the
      // smaller one, adjusting the result as required.

    const Map* bigger;
    const Map* smaller;
    if (m1.size() >= m2.size())
    {
        bigger = &m1;
        smaller = &m2;
    }
    else
    {
        bigger = &m2;
        smaller = &m1;
    }

      // Guard against the case that result is an alias for m1 or m2
      // (i.e., that result is a reference to the same map that m1 or m2
      // refers to) by building the answer in a local variable res.  When
      // done, swap res with result; the old value of result (now in res) will
      // be destroyed when res is destroyed.

    bool status = true;
    Map res(*bigger);               // res starts as a copy of the bigger map
    for (int n = 0; n < smaller->size(); n++)  // for each pair in smaller
    {
        KeyType k;
        ValueType vsmall;
        smaller->get(n, k, vsmall);
        ValueType vbig;
        if (!res.get(k, vbig))      // key in smaller doesn't appear in bigger
            res.insert(k, vsmall);  //     so add it to res
        else if (vbig != vsmall)    // same key, different value
        {                           //     so pair shouldn't be in res
            res.erase(k);
            status = false;
        }
    }
    result.swap(res);
    return status;
}

void reassign(const Map& m, Map& result)
{
      // Guard against the case that result is an alias for m (i.e., that
      // result is a reference to the same map that m refers to) by building
      // the answer in a local variable res.  When done, swap res with result;
      // the old value of result (now in res) will be destroyed when res is
      // destroyed.

    Map res;

    if (!m.empty())
    {
        KeyType prevKey;
        ValueType value0;

          // Get pair 0, which must succeed since m is not empty

        m.get(0, prevKey, value0);

          // For each pair i after pair 0, insert into res a pair with
          // pair i-1's key and pair i's value.  (This loop executes 0 times
          // if m has only one pair.)

        for (int i = 1; i < m.size(); i++)
        {
            KeyType k;
            ValueType v;
            m.get(i, k, v);
            res.insert(prevKey, v);
            prevKey = k;
        }

          // Insert a final pair with last pair's key and pair 0's value.

        res.insert(prevKey, value0);
    }

    result.swap(res);
}
/**
//  eval.cpp
//  hw2_q5
//
//  Created by Austin Wang on 2/3/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

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
int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if(infix=="")
        return 1;
    cout<<infix<<endl;
    postfix="";
    infix=removeSpaces(infix);
    stack<char> operatorStack;
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
        
        bool precedence=false;
        switch(ch)
        {
            
            case '(':
                if(i==0 ||(infix[i-1]!='('&&!checkOperand(infix[i-1]) ))
                {
                    operatorStack.push(ch);
                }
                else
                {
                    return 1;
                }
            
                
                break;
            case ')':
                //remember to check for ( earlier in string
                while (operatorStack.top()!= '(')
                {
                    postfix+=operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop();
                break;
            case '+':
            case '-':
                precedence=true;
            case '*':
            case '/':
                //check if ch<= precedence of stacktop
                
                while (!operatorStack.empty() && (( operatorStack.top()!='('&&operatorStack.top()!='-'&&operatorStack.top()!='+')||precedence))
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
                    {
                        return 2;
                    }
                    operandStack.push(operand1/operand2);
                    break;
            }
        }
    }
    result=operandStack.top();
    return 0;
    

    
    
    
    return 1;
}
         // Evaluates an integer arithmetic expression
         //   If infix is a syntactically valid infix integer expression whose
         //   only operands are single lower case letters (whether or not they
         //   appear in the values map), then postfix is set to the postfix
         //   form of the expression; otherwise postfix may or may not be
         //   changed, result is unchanged, and the function returns 1.  If
         //   infix is syntactically valid but contains at least one lower
         //   case letter operand that does not appear in the values map, then
         //   result is unchanged and the function returns 2.  If infix is
         //   syntactically valid and all its lower case operand letters
         //   appear in the values map, then if evaluating the expression
         //   (using for each letter in the expression the value in the map
         //   that corresponds to it) attempts to divide by zero, then result
         //   is unchanged and the function returns 3; otherwise, result is
         //   set to the value of the expression and the function returns 0.


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
    
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
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
    cout << "Passed all tests" << endl;
}
**/
