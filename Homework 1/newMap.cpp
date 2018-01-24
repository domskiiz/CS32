//
//  newMap.cpp
//  homework1
//
//  Created by Alissa Niewiadomski on 1/23/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#include "newMap.h"
#include <iostream>

Map::Map() {
    m_numElements = 0;
    m_maxNumElements = DEFAULT_MAX_ITEMS;
    m_map = new MapElement[DEFAULT_MAX_ITEMS];
}

Map::Map(int maxItems) {
    if (maxItems < 0) {
        exit(1);
    }
    m_numElements = 0;
    m_maxNumElements = maxItems;
    m_map = new MapElement[m_maxNumElements];
}

Map::~Map() {
    delete [] m_map;
}

Map::Map(const Map& other) {
    m_numElements = other.m_numElements;
    m_maxNumElements = other.m_maxNumElements;
    m_map = new MapElement[m_maxNumElements];
    // copy each MapElement one-by-one
    for (int i = 0; i < m_maxNumElements; i++) {
        m_map[i] = other.m_map[i];
    }
}

Map& Map::operator=(const Map& rhs) {
    if (this != &rhs) {         // check for self-assignment
        delete [] m_map;        // solves memory leak
        m_numElements = rhs.m_numElements;
        m_maxNumElements = rhs.m_maxNumElements;
        m_map = new MapElement[m_maxNumElements];
        // copy each MapElement one-by-one
        for (int i = 0; i < m_maxNumElements; i++) {
            m_map[i] = rhs.m_map[i];
        }
    }
    return *this;
}


bool Map::empty() const {
    return m_numElements == 0;
}

int Map::size() const {
    return m_numElements;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    // check if key already exists
    if (contains(key)) {
        return false;
    }
    // check if map is full
    if (size() == m_maxNumElements)
        return false;
    
    // else, add key-value pair to map
    MapElement newElement;
    newElement.m_key = key;
    newElement.m_value = value;
    
    m_map[size()] = newElement;
    m_numElements++;
    
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    // check if key exists in map
    for (int i = 0; i < size(); i++) {
        if (m_map[i].m_key == key) {
            m_map[i].m_value = value;
            return true;
        }
    }
    // else, key doesn't exist
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    // check if key exists in map. if so, update
    for (int i = 0; i < size(); i++) {
        if (m_map[i].m_key == key) {
            m_map[i].m_value = value;
            return true;
        }
    }
    
    // check if map is full, don't add if it is
    if (size() == m_maxNumElements)
        return false;
    
    // if key doesn't exist, then add it to the map
    MapElement newElement;
    newElement.m_key = key;
    newElement.m_value = value;
    
    m_map[size()] = newElement;
    m_numElements++;
    
    return true;
}

bool Map::erase(const KeyType& key) {
    for (int i = 0; i < size(); i++) {
        if (m_map[i].m_key == key) {
            // iterate through from m_map[i] -> end and
            // replace each element with the one after
            for (int j = i; j < size() - i; j++) {
                m_map[j] = m_map[j + 1];
            }
            m_numElements--;
            return true;
        }
    }
    // if key doesn't exist
    return false;
}

bool Map::contains(const KeyType& key) const {
    for (int i = 0; i < size(); i++) {
        if (m_map[i].m_key == key) {
            return true;
        }
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    for (int i = 0; i < size(); i++) {
        if (m_map[i].m_key == key) {
            value = m_map[i].m_value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i >= 0 && i < size()) {
        key = m_map[i].m_key;
        value = m_map[i].m_value;
        return true;
    } else {
        return false;
    }
}

void Map::swap(Map& other) {
    Map tempmap = *this;
    *this = other;
    other = tempmap;
    
    int tempNumElements = m_numElements;
    m_numElements = other.m_numElements;
    other.m_numElements = tempNumElements;
    
    int tempMaxElements = m_maxNumElements;
    m_maxNumElements = other.m_maxNumElements;
    other.m_maxNumElements = tempMaxElements;
}


