//
//  mazequeue.cpp
//  Hw2
//
//  Created by Austin Wang on 2/3/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//
#include <stdio.h>
#include <string>
#include <iostream>
#include <queue>
#include <stdio.h>
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

bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    std::queue<Coord> coordQueue;
    Coord startCoord(sr,sc);
    coordQueue.push(startCoord);
    maze[sr][sc]='@';
    while (!coordQueue.empty())
    {
        Coord temp = coordQueue.front();
        //std::cout<<"("<<temp.r()<<","<<temp.c()<<")"<<std::endl;
        coordQueue.pop();
        if(temp.r()==er && temp.c()==ec)
        {
            return true;
        }
        if(maze[temp.r()][temp.c()+1]=='.')
        {
            Coord eastPath(temp.r(), temp.c()+1);
            coordQueue.push(eastPath);
            maze[temp.r()][temp.c()+1]='@';
        }
        if(maze[temp.r()+1][temp.c()]=='.')
        {
            Coord southPath(temp.r()+1, temp.c());
            coordQueue.push(southPath);
            maze[temp.r()+1][temp.c()]='@';
        }
        if(maze[temp.r()][temp.c()-1]=='.')
        {
            Coord westPath(temp.r(), temp.c()-1);
            coordQueue.push(westPath);
            maze[temp.r()][temp.c()-1]='@';
        }
        if(maze[temp.r()-1][temp.c()]=='.')
        {
            Coord northPath(temp.r()-1, temp.c());
            coordQueue.push(northPath);
            maze[temp.r()-1][temp.c()]='@';
        }
    }
    return false;
}
