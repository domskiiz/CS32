//
//  mazequeue.cpp
//  homework2
//
//  Created by Alissa Niewiadomski on 2/5/18.
//  Copyright © 2018 Alissa Niewiadomski. All rights reserved.
//

#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


// void printMaze(string maze[], int rows, int cols);

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    queue<Coord> coordQueue;
    Coord start(sr, sc);
    Coord end(er, ec);
    
    coordQueue.push(start);
    maze[sr][sc] = 'e';
    // printMaze(maze, nRows, nCols);
    
    while(!coordQueue.empty()) {
        Coord curr = coordQueue.front();
        // cout << "POP! " << curr.r() << " " << curr.c() << endl;
        coordQueue.pop();
        
        if (curr.r() == end.r() && curr.c() == end.c()) {
            // printMaze(maze, nRows, nCols);
            return true;
        } else {
            // check EAST of curr
            if (curr.c() < nCols && maze[curr.r()][curr.c() + 1] == '.') {
                Coord newCoord(curr.r(), curr.c() + 1);
                coordQueue.push(newCoord);
                maze[curr.r()][curr.c() + 1] = 'e';
                // printMaze(maze, nRows, nCols);
                
            }
            // check SOUTH of curr
            if (curr.r() < nRows && maze[curr.r() + 1][curr.c()] == '.') {
                Coord newCoord(curr.r() + 1, curr.c());
                coordQueue.push(newCoord);
                maze[curr.r() + 1][curr.c()] = 'e';
                // printMaze(maze, nRows, nCols);
                
            }
            // check WEST of curr
            if (curr.c() > 0 && maze[curr.r()][curr.c() - 1] == '.') {
                Coord newCoord(curr.r(), curr.c() - 1);
                coordQueue.push(newCoord);
                maze[curr.r()][curr.c() - 1] = 'e';
                // printMaze(maze, nRows, nCols);
                
            }
            // check NORTH of curr
            if (curr.r() > 0 && maze[curr.r() - 1][curr.c()] == '.') {
                Coord newCoord(curr.r() - 1, curr.c());
                coordQueue.push(newCoord);
                maze[curr.r() - 1][curr.c()] = 'e';
                // printMaze(maze, nRows, nCols);
                
            }
        }
    }
    return false;
}

//void printMaze(string maze[], int rows, int cols) {
//    for (int i = 0; i < cols; i++) {
//        for (int j = 0; j < rows; j++) {
//            cout << maze[i][j];
//        }
//        cout << endl;
//    }
//    cout << endl;
//}

//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X...X..X.X",
//        "X..XX....X",
//        "X.X.XXXX.X",
//        "XXX......X",
//        "X...X.XX.X",
//        "X.X.X..X.X",
//        "X.XXXX.X.X",
//        "X..X...X.X",
//        "XXXXXXXXXX"
//    };
//
//    if (pathExists(maze, 10,10, 4,3, 1,8))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}






