//
//  Map.cpp
//  project2
//
//  Created by Alissa Niewiadomski on 1/26/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#include "Map.h"
#include<iostream>

// constructor
Map::Map() {
    head = nullptr;
    m_numElements = 0;
}

// copy constructor
Map::Map(const Map& other) {
    m_numElements = other.m_numElements;
    head = nullptr;
    
    for (int i = 0; i < m_numElements; i++) {
        KeyType key;
        ValueType value;
        other.get(i, key, value);   // get each other node
        insert(key, value);         // insert function dyanmically allocates new nodes
    }
}

// assignment operator
Map& Map::operator=(const Map& rhs) {
    if (&rhs == this)
        return (*this);
    // simply swap the current map with the rhs without changing rhs
    Map temp = rhs;
    swap(temp);
    return *this;
}

// destructor
Map::~Map() {
    Node* p = head;
    while (p != nullptr) {
        Node* deleteNode = p;
        p = p->next;
        delete deleteNode;
    }
}

// check if there are no nodes
bool Map::empty() const {
    return (head == nullptr);
}

// return the number of nodes
int Map::size() const {
//    Node* p = head;
//    int size = 0;
//    while (p != nullptr) {
//        size++;
//        p = p->next;
//    }
//    return size;
    return m_numElements;
}

// add a node if there isn't a node with the key already given
bool Map::insert(const KeyType& key, const ValueType& value) {
    // check if map contains node with given key
    if (contains(key)) {
        return false;
    }
    
    // allocate memory for a new node
    Node* newNode = new Node;
    newNode->value = value;
    newNode->key = key;
    
    // make new node point to first node
    // prev point to nullptr
    newNode->next = head;
    newNode->prev = nullptr;
    
    // make head point to new node
    head = newNode;
    m_numElements++;
    return true;
}

// update node if key exists in map
bool Map::update(const KeyType& key, const ValueType& value) {
    // check if map contains node with given key
    Node* p = head;
    while (p != nullptr) {
        if (p->key == key) {
            p->value = value;
            return true;
        }
        p = p->next;
    }
    // otherwise, return false
    return false;
}

// insert a new node or update an existing one
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (update(key, value)) {
        ;
    } else {
        insert(key, value);
    }
    return true;
}

// delete a node with the given key
bool Map::erase(const KeyType& key) {
    // check if map contains node with given key
    Node* p = head;
    while (p != nullptr) {
        if (p->key == key) {
            // if deleting first node, set head to nullptr
            if (p->prev == nullptr) {
                head = p->next;
            }
            // if deleting last node, set prev node->next to nullptr
            else if (p->next == nullptr) {
                Node* previousNode = p->prev;
                previousNode->next = nullptr;
            }
            // if deleting from middle:
            //      1. set previous node->next to deleted node->next
            //      2. set deleted node->prev to be deleted node->prev
            else {
                Node* previousNode = p->prev;
                Node* nextNode = p->next;
                previousNode->next = nextNode;
                nextNode->prev = previousNode;
            }
            delete p;
            m_numElements--;
            return true;
        }
        p = p->next;
    }
    return false;
}

// check if the map contains a node with given key
bool Map::contains(const KeyType& key) const {
    Node* p = head;
    while (p != nullptr) {
        if (p->key == key) {
            return true;
        }
        p = p->next;
    }
    return false;
}

// set value to the value assigned to key if it exists in map
// otherwise, don't change key or value
bool Map::get(const KeyType& key, ValueType& value) const {
    if (contains(key)) {
        Node* p = head;
        while (p->key != key) {
            p = p->next;
        }
        value = p->value;
        return true;
    } else {
        return false;
    }
}

// get the i'th element of the map if i is in bounds of the size
// set key and value to the key and value of that element
bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i >= 0 && i < size()) {
        Node* p = head;
        for (int j = 0; j < i; j++) {
            p = p->next;
        }
        key = p->key;
        value = p->value;
        return true;
    } else {
        return false;
    }
}

// exchange contents of two maps
void Map::swap(Map& other) {
    Node* tempHead = head;
    int tempSize = size();
    
    head = other.head;
    m_numElements = other.m_numElements;
    
    other.head = tempHead;
    other.m_numElements = tempSize;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    Map map1temp = m1;
    result.swap(map1temp);      // start result with all the nodes of m1

    bool returnTrue = true;
    
    for (int i = 0; i < m2.size(); i++) {
        // get key and value of each element of m2
        KeyType key2;
        ValueType value2;
        m2.get(i, key2, value2);
        
        // if m1 (result) contains a key in m2
        if (result.contains(key2)) {
            ValueType value1;
            result.get(key2, value1);       // compare values of corresponding nodes
            if (value1 != value2) {         // if not equal
                returnTrue = false;         // set return value to false
                result.erase(key2);         // get rid of that element from result
            }
        } else {
            result.insert(key2, value2);    // else, insert that key and value
        }
    }
    return (returnTrue);
}

void subtract(const Map& m1, const Map& m2, Map& result) {
    Map map1temp = m1;
    result.swap(map1temp);                  // start result with contents of m1
    
    for (int i = 0; i < m2.size(); i++) {   // go through each element of m2
        KeyType key2;
        ValueType value2;
        m2.get(i, key2, value2);
        
        if (result.contains(key2)) {        // if m1 (result) contains key of m2, erase it from result
            result.erase(key2);
        }
    }
}

// test function for debugging, prints out contents of linked list
void Map::dump() const {
    Node* p = head;
    int counter = 0;
    while (p != nullptr) {
        std::cerr << "Node " << counter + 1;
        std::cerr << " ------ Key: " << p->key;
        std::cerr << " Value: " << p->value << std::endl;
        p = p->next;
        counter++;
    }
}

