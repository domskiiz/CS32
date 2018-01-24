//
//  Map.cpp
//  homework1
//
//  Created by Alissa Niewiadomski on 1/22/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#include "Map.h"
#include <iostream>

Map::Map() {
    m_numElements = 0;
}

bool Map::empty() const {
    return m_numElements == 0;
}

int Map::size() const {
    return m_numElements;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    // check if key already exists
    for (int i = 0; i < size(); i++) {
        if (m_map[i].m_key == key) {
            return false;
        }
    }
    // check if map is full
    if (size() == DEFAULT_MAX_ITEMS)
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
    if (size() == DEFAULT_MAX_ITEMS)
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
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
        KeyType tempKey;
        ValueType tempValue;
        
        other.get(i, tempKey, tempValue);
        
        other.m_map[i].m_key = m_map[i].m_key;
        other.m_map[i].m_value = m_map[i].m_value;
        
        m_map[i].m_key = tempKey;
        m_map[i].m_value = tempValue;
    }
    int tempNumElements = other.m_numElements;
    other.m_numElements = size();
    m_numElements =  tempNumElements;
}

