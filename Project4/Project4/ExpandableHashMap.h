// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct KeyValue
    {
        KeyValue(KeyType k,ValueType v)
        {
            m_key=k;
            m_value=v;
        }
        KeyType m_key;
        ValueType m_value;
    };
    
    std::list<KeyValue*> *m_hashTable;
    int m_numBuckets;
    int m_size;
    double m_maxLF;
    
    
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType,ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    m_maxLF=maximumLoadFactor;
    m_size=0;
    m_numBuckets=8;
    m_hashTable=new std::list<KeyValue*> [m_numBuckets];
    for(int i=0;i<8;++i)
    {
        std::list<KeyValue*> tmpList;
        m_hashTable[i]=tmpList;
    }
    //std::vector<std::list<KeyValue*>> m_hashTable(m_numBuckets,std::list<KeyValue*>());
}
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType,ValueType>::~ExpandableHashMap()
{
    delete [] m_hashTable;
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType,ValueType>::reset()
{
}
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType,ValueType>::size() const
{
    return m_size;
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType,ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hasher(const KeyType& k);
    unsigned int h=hasher(key)%m_numBuckets;
    
    ValueType *v=find(key);
    if(v==nullptr)
    {
        KeyValue *kv=new KeyValue(key,value);
        m_hashTable[h].push_back(kv);
        m_size++;
    }
    else
    {
        *v=value;
    }
    
    
    
    
}
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType,ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType& k);
    unsigned int h=hasher(key)%m_numBuckets;
    typename std::list<KeyValue*>::const_iterator it;
    //std::cout <<m_hashTable.size();
    std::cout <<m_hashTable[h].size();
    if(!m_hashTable[h].empty())
    {
        for (it=m_hashTable[h].begin();it!=m_hashTable[h].end();it++)
        {
            if((*it)->m_key==key)
                return &(*it)->m_value;
        }
    }
        
    return nullptr;  // Delete this line and implement this function correctly
}

