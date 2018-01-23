//
//  globals.h
//  project1
//
//  Created by Alissa Niewiadomski on 1/15/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#ifndef globals_h
#define globals_h

int randInt(int min, int max);
void clearScreen();

const int MAXROWS = 20;               // max number of rows in a city
const int MAXCOLS = 30;               // max number of columns in a city
const int MAXFLATULANS = 100;         // max number of Flatulans allowed
const int INITIAL_PLAYER_HEALTH = 10;

const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;

#endif /* globals_h */
