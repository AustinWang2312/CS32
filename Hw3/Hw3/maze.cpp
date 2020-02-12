//
//  maze.cpp
//  Hw3
//
//  Created by Austin Wang on 2/11/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if(sr==er&&sc==ec)
        return true;
    maze[sr][sc]='@';
    if(maze[sr][sc+1]=='.')
        if(pathExists(maze,nRows,nCols,sr,sc+1,er,ec))
            return true;
    if(maze[sr+1][sc]=='.')
        if(pathExists(maze,nRows,nCols,sr+1,sc,er,ec))
            return true;
    if(maze[sr][sc-1]=='.')
        if(pathExists(maze,nRows,nCols,sr,sc-1,er,ec))
            return true;
    if(maze[sr-1][sc]=='.')
        if(pathExists(maze,nRows,nCols,sr-1,sc,er,ec))
            return true;
    return false;
}
/*
using namespace std;
int main()
{
    std::string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X.XXX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 8,2, 1,3))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
**/
