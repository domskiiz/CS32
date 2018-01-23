//
//  Game.h
//  project1
//
//  Created by Alissa Niewiadomski on 1/15/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#ifndef Game_h
#define Game_h

class City;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nFlatulans);
    ~Game();
    
    // Mutators
    void play();
    
private:
    City* m_city;
};

#endif /* Game_h */
