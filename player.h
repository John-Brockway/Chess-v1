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
    void movePiece(std::string start, std::string end);
    void movePiece(std::string start, std::string end, std::string prom);
    void aiMovePiece();
    void setCheck(bool c);
    bool checkCheck();
    int getAILevel();
};

#endif