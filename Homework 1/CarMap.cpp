//
//  CarMap.cpp
//  homework1
//
//  Created by Alissa Niewiadomski on 1/23/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#include "CarMap.h"
#include <iostream>

CarMap::CarMap()
: m_carmap() {

}

bool CarMap::addCar(std::string license) {
    if (m_carmap.insert(license, 0)) {
        return true;
    } else {
        return false;
    }
}

double CarMap::gas(std::string license) const {
    ValueType gallons;
    if (m_carmap.get(license, gallons)) {
        return gallons;
    } else {
        return -1;
    }
}

bool CarMap::addGas(std::string license, double gallons) {
    if (m_carmap.contains(license) && gallons > 0) {
        ValueType currentGals;
        m_carmap.get(license, currentGals);
        double newGals = currentGals + gallons;
        m_carmap.update(license, newGals);
        return true;
    } else {
        return false;
    }
}

bool CarMap::useGas(std::string license, double gallons) {
    if (m_carmap.contains(license) && gas(license) >= gallons) {
        m_carmap.update(license, gas(license) - gallons);
        return true;
    } else {
        return false;
    }
}

int CarMap::fleetSize() const {
    return m_carmap.size();
}

void CarMap::print() const {
    for (int i = 0; i < m_carmap.size(); i++) {
        KeyType license;
        ValueType gallons;
        m_carmap.get(i, license, gallons);
        std::cout << license << " " << gallons << std::endl;
    }
}


