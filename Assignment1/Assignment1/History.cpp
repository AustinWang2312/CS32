//
//  History.cpp
//  Assignment1
//
//  Created by Austin Wang on 1/10/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <stdio.h>
#include "History.h"
#include "globals.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_columns = nCols;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_columns; c++)
            m_grid[r-1][c-1]=EMPTY;
}
bool History::record(int r, int c)
{
    if (r <= 0  ||  c <= 0  ||  r > MAXROWS  ||  c > MAXCOLS)
        return false;
    if(m_grid[r-1][c-1]!=26)
        m_grid[r-1][c-1]+=1;
    return true;
}
void History::display() const
{
    clearScreen();
    char display[MAXROWS][MAXCOLS];
    int r, c;
    
    for (r = 1; r <= m_rows; r++)
        for (c = 1; c <= m_columns; c++)
            switch (m_grid[r-1][c-1])
            {
              case 0:
                    display[r-1][c-1]= '.';
                    break;
              case 26:
                    display[r-1][c-1]= 'Z';
                    break;
                default:
                    display[r-1][c-1]= '@'+ m_grid[r-1][c-1];
                    break;
            }
    for (r = 1; r <= m_rows; r++)
    {
        for (c = 1; c <= m_columns; c++)
            cout << display[r-1][c-1];
        cout << endl;
    }
    cout <<endl;
}
