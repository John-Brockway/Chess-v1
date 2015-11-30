#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Board;

class Player {
  bool check;
  int aiLevel;
  Board *board;
  public:
    Player(int i, Board *b);
    void movePiece(string start, string end);
    void movePiece(string start, string end, string prom);
    void aiMovePiece();
    void setCheck(bool c);
    bool checkCheck();
    int getAILevel();
};

#endif
