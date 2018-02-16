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


// IMPLEMENTATION //
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    Coord start(sr, sc);
    Coord end(er, ec);
    
    if (start.r() == end.r() && start.c() == end.c()) {
        return true;
    }
    
    maze[sr][sc] = 'e';
    
    // check EAST of start
    if (start.c() < nCols && maze[start.r()][start.c() + 1] == '.') {
        if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec)) {
            return true;
        }
    }
    // check SOUTH of start
    if (start.r() < nRows && maze[start.r() + 1][start.c()] == '.') {
        if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) {
            return true;
        }
    }
    // check WEST of start
    if (start.c() > 0 && maze[start.r()][start.c() - 1] == '.') {
        if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec)) {
            return true;
        }
    }
    // check NORTH of start
    if (start.r() > 0 && maze[start.r() - 1][start.c()] == '.') {
        if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec)) {
            return true;
        }
    }
    return false;
}


//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X...X..X.X",
//        "X..XX....X",
//        "X.X.XXXX.X",
//        "XXX......X",
//        "X...XXXX.X",
//        "X.X.X..X.X",
//        "X.XXXX.X.X",
//        "X..X...X.X",
//        "XXXXXXXXXX"
//    };
//
//    if (pathExists(maze, 10,10, 6,5, 8,1))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}





