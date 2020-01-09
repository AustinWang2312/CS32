//
//  Game.h
//  Assignment1
//
//  Created by Austin Wang on 1/8/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#ifndef Game_h
#define Game_h
#include <string>
class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game_h */
