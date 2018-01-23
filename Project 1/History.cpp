//
//  History.cpp
//  project1
//
//  Created by Alissa Niewiadomski on 1/15/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#include <iostream>
using namespace std;

#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            FlatulansNotConverted[i][j] = '.';
        }
    }
}

bool History::record(int r, int c) {
    if (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols) {
        if (FlatulansNotConverted[r-1][c-1] >= 'A' && FlatulansNotConverted[r-1][c-1] <= 'Z') {
            FlatulansNotConverted[r-1][c-1]++;
        } else {
            FlatulansNotConverted[r-1][c-1] = 'A';
        }
        return true;
    } else {
        return false;
    }
}

void History::display() const {
    clearScreen();
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++) {
            cout << FlatulansNotConverted[r][c];
        }
        cout << endl;
    }
    cout << endl;
}
