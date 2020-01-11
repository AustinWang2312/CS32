//
//  History.h
//  Assignment1
//
//  Created by Austin Wang on 1/10/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
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
    int m_columns;
    int m_grid[MAXROWS][MAXCOLS];
};

#endif /* History_h */
