//
//  Player.h
//  Assignment1
//
//  Created by Austin Wang on 1/8/20.
//  Copyright © 2020 Austin Wang. All rights reserved.
//

#ifndef Player_h
#define Player_h
#include <string>

class Arena;

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string dropPoisonVial();
    std::string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);



#endif /* Player_h */
