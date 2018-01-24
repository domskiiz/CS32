//
//  testCarMap.cpp
//  homework1
//
//  Created by Alissa Niewiadomski on 1/23/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//


#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    CarMap m;
    m.addCar("123456789");
    m.addCar("BBGURL");
    assert(m.gas("123456789") == 0 && m.gas("BBGURL") == 0);
    
    m.addGas("123456789", 19.3);
    assert(!m.addGas("123456789", -3) && !m.addGas("23", 3.2) && m.gas("123456789") == 19.3);
    
    m.addGas("BBGURL", 29.3);
    m.useGas("BBGURL", 24.1);
    assert(m.gas("BBGURL") == 29.3 - 24.1 && !m.useGas("123456789", 100));
    
    assert(m.fleetSize() == 2);
    m.addCar("PVNRT");
    assert(m.fleetSize() == 3);
    
    m.print();
    
    cout << "Passed all tests" << endl;
}
