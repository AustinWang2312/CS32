//
//  Map.h
//  Project2
//
//  Created by Austin Wang on 1/26/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//
#include <string>
#include <iostream>

#ifndef Map_h
#define Map_h
using KeyType = std::string;
using ValueType = double;

class Map
{
  public:
    Map()
    {
    }
    Map(const Map &src)
    {
        m_list.m_size=0;
        Node *p_src=src.m_list.dummy_ptr;
        while (p_src->next!=src.m_list.dummy_ptr)
        {
            p_src=p_src->next;
            m_list.addToRear(p_src->m_key, p_src->m_value);
        }
        
    }
    Map &operator= (const Map &src)
    {
        if (&src==this)
            return *this;
        int size=m_list.size();
        for(int i=0;i<size;i++)
        {
            KeyType k;
            ValueType v;
            get(0, k, v);
            erase(k);
        }
        Node *p_src=src.m_list.dummy_ptr;
        while (p_src->next!=src.m_list.dummy_ptr)
        {
            p_src=p_src->next;
            m_list.addToRear(p_src->m_key, p_src->m_value);
        }
        
        return *this;
    }
    ~Map()
    {
        Node *p=m_list.dummy_ptr->next;
        while (p!=m_list.dummy_ptr)
        {
            Node *tmp=p->next;
            delete p;
            p=tmp;
        }
    }
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump() const;
    
    
private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node *next;
        Node *prev;
    };
    struct LinkedList
    {
    public:
        LinkedList()
        {
            dummy_ptr->next=dummy_ptr;
            dummy_ptr->prev=dummy_ptr;
            m_size=0;
        }
        
        bool addToRear(KeyType key, ValueType value)
        {
            if (isKey(key))
            {
                return false;
            }
            Node *p=dummy_ptr->prev;
            Node *n= new Node;
            n->m_key=key;
            n->m_value=value;
            
            p->next=n;
            dummy_ptr->prev=n;
            n->prev=p;
            n->next=dummy_ptr;
            
            m_size++;
            return true;
        }
        void printItems() const
        {
            Node *p=dummy_ptr->next;
            while(p!=dummy_ptr)
            {
                std::cerr<< p->m_key <<std::endl << p->m_value <<std::endl;
                p=p->next;
            }
        }
        int size() const
        {
            return m_size;
        }
        bool updateItem(KeyType key, ValueType value)
        {
            Node *p=dummy_ptr->next;
            while(p!=dummy_ptr)
            {
                if(p->m_key==key)
                {
                    p->m_value=value;
                    return true;
                }
                p=p->next;
            }
            return false;
                
        }
        bool deleteItem(KeyType key)
        {
            if (!isKey(key))
            {
                return false;
            }
            Node *p=dummy_ptr;
            while(p->next!=dummy_ptr)
            {
                if(p->next->m_key==key)
                {
                    Node *kill=p->next;
                    p->next=kill->next;
                    p->next->prev=p;
                    delete kill;
                    m_size--;
                    return true;
                }
                p=p->next;
            }
            return false;
                
            
        }
        bool findItem(const KeyType key, ValueType& value) const
        {
            Node *p=dummy_ptr->next;
            while(p!=dummy_ptr)
            {
                if(p->m_key==key)
                {
                    value=p->m_value;
                    return true;
                }
                p=p->next;
            }
            return false;
        }
        bool findItem(int i,KeyType& key, ValueType& value) const
        {
            if (i < 0  ||  i >= m_size)
                return false;
            Node *p=dummy_ptr;
            for (int j=0;j<=i;j++)
            {
                p=p->next;
            }
            key=p->m_key;
            value=p->m_value;
            return true;
        }
        
        bool isKey(KeyType key) const
        {
            Node *p=dummy_ptr->next;
            while(p!=dummy_ptr)
            {
                if(p->m_key==key)
                    return true;
                p=p->next;
            }
            return false;
        }
        
        int checkKeyIndex(KeyType key) const
        {
            int nCount =0;
            Node *p=dummy_ptr->next;
            while(p!=dummy_ptr)
            {
                nCount++;
                if(p->m_key==key)
                    return nCount;
                p=p->next;
            }
            return nCount;
        }
        Node dummy;
        Node *dummy_ptr=&dummy;
        int m_size;
        
    };
    
    LinkedList m_list;
};
void reassign(const Map& m, Map& result);
bool combine(const Map& m1, const Map& m2, Map& result);


#endif /* Map_h */

