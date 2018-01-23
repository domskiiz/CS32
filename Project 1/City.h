//
//  City.h
//  project1
//
//  Created by Alissa Niewiadomski on 1/15/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#ifndef City_h
#define City_h

#include "globals.h"
#include "History.h"

class Player;
class Flatulan;

class City
{
public:
    // Constructor/destructor
    City(int nRows, int nCols);
    ~City();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     flatulanCount() const;
    int     nFlatulansAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    
    // Mutators
    bool  addFlatulan(int r, int c);
    bool  addPlayer(int r, int c);
    void  preachToFlatulansAroundPlayer();
    void  moveFlatulans();
    History& history();
    
private:
    int       m_rows;
    int       m_cols;
    Player*   m_player;
    Flatulan* m_flatulans[MAXFLATULANS];
    History   m_history;
    int       m_nFlatulans;
    
    // Helper functions
    bool isInBounds(int r, int c) const;
};


#endif /* City_h */
