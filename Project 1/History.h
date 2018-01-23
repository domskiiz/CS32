//
//  History.h
//  project1
//
//  Created by Alissa Niewiadomski on 1/15/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#ifndef History_h
#define History_h

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;

private:
    int m_rows;
    int m_cols;
    char FlatulansNotConverted[MAXROWS][MAXCOLS];
};

#endif /* History_h */
