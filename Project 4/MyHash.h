// MyHash.h

#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

#include<iostream>
using namespace std;

const int DEFAULT_MAX_BUCKETS = 100;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;


private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* next;
    };
    double  m_maxLoadFactor;
    Node**  buckets;
    int     m_numBuckets;
    int     m_numItems;
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    if (maxLoadFactor <= 0)
        m_maxLoadFactor = 0.5;
    else if (maxLoadFactor > 2.0)
        m_maxLoadFactor = 2.0;
    else
        m_maxLoadFactor = maxLoadFactor;
    
    m_numBuckets = DEFAULT_MAX_BUCKETS;
    
    buckets = new Node*[m_numBuckets];
    for (int i = 0; i < DEFAULT_MAX_BUCKETS; i++)
        buckets[i] = nullptr;
    
    m_numItems = 0;
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    for (int i = 0; i < m_numBuckets; i++) {
        Node* p = buckets[i];
        while (p != nullptr) {
            Node* deleteMe = p;
            p = p->next;
            delete deleteMe;
        }
    }
    delete[] buckets;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    for (int i = 0; i < m_numBuckets; i++) {
        Node* p = buckets[i];
        while (p != nullptr) {
            Node* deleteMe = p;
            p = p->next;
            delete deleteMe;
        }
    }
    delete[] buckets;
    
    m_numBuckets = DEFAULT_MAX_BUCKETS;
    buckets = new Node*[m_numBuckets];
    for (int i = 0; i < m_numBuckets; i++)
        buckets[i] = nullptr;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hash(const KeyType& k);
    // Check if key is already in hash table, and update if it is
    ValueType* valInHash = find(key);
    if (valInHash != nullptr) {
        *valInHash = value;
        return;
    }
    
    // Check if inserting new node would cause loadfactor to exceed maximum
    double updatedLF = (getNumItems() + 1) / (m_numBuckets * 1);
    if (updatedLF >= m_maxLoadFactor || getLoadFactor() == m_maxLoadFactor) {
        // Make copies of old data
        int prevSize = m_numBuckets;
        Node **prevTable = buckets;
        
        // New table
        m_numBuckets *= 2;
        buckets = new Node*[m_numBuckets];
        for (int i = 0; i < m_numBuckets; i++)
            buckets[i] = nullptr;
        
        // Copy over data into new hash table
        for (int h = 0; h < prevSize; h++) {
            if (prevTable[h] != nullptr) {
                Node* deleteMe;
                Node* entry = prevTable[h];
                while (entry != nullptr) {
                    // Get hash + bucket
                    unsigned int h = hash(entry->key);
                    int bucket = h % m_numBuckets;
                    
                    // Insert new node into bucket
                    Node* newNode = new Node;
                    newNode->value = entry->value;
                    newNode->key = entry->key;
                    newNode->next = buckets[bucket];
                    buckets[bucket] = newNode;
                    
                    // Delete old entry
                    deleteMe = entry;
                    entry = entry->next;
                    delete deleteMe;
                }
            }
        }
        delete[] prevTable;
    }
    
    // Get hash + bucket for item to associate
    unsigned int h = hash(key);
    int bucket = h % m_numBuckets;

    // Insert new node into bucket
    Node* newNode = new Node;
    newNode->value = value;
    newNode->key = key;
    newNode->next = buckets[bucket];
    buckets[bucket] = newNode;
    m_numItems++;
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    // Get hash + bucket
    unsigned int hash(const KeyType& k);
    unsigned int h = hash(key);
    int bucket = h % m_numBuckets;
    
    // Iterate through that bucket to find value
    Node* p = buckets[bucket];
    while (p != nullptr) {
        if (p->key == key) {
            ValueType* value = &(p->value);
            return value;
        }
        p = p->next;
    }
    return nullptr;
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_numItems;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return (getNumItems() / (m_numBuckets * 1.0));
}

//template<typename KeyType, typename ValueType>
//void MyHash<KeyType, ValueType>::printBucketContents() const
//{
//    int numNodes = 0;
//    for (int i = 0; i < m_numBuckets; i++) {
//        Node* p = buckets[i];
//        cout << "in bucket " << i << endl;
//        while (p != nullptr) {
//                cout << p->key << " " << p->value[i] << endl;
//            p = p->next;
//            numNodes++;
//        }
//    }
//}
#endif // MYHASH_INCLUDED
