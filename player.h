#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Board;

class Player {
  char colour;
  bool check;
  int aiLevel;
  Board *board;
  public:
    Player(int i, Board *b, char c);
    void movePiece(std::string start, std::string end);
    void movePiece(std::string start, std::string end, char prom);
    void aiMovePiece();
    void setCheck(bool c);
    bool checkCheck();
    int getAILevel();
};

#endif
