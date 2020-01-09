//
//  main.cpp
//  Assignment1
//
//  Created by Austin Wang on 1/8/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

// vampires.cpp




#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>

#include "Game.h"
using namespace std;
int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}


