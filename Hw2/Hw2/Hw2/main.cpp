//
//  main.cpp
//  Hw2
//
//  Created by Austin Wang on 2/1/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;
bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

int main()
{
    string maze[10] = {
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
    
        if (pathExists(maze, 10,10, 4,3, 1,8))
            cout << "Solvable!" << endl;
        else
            cout << "Out of luck!" << endl;
    return 0;
}
