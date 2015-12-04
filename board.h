#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "graphicdisplay.h"

class Board {
  GraphicDisplay *graphics;
  char brd[8][8];
  bool wEnPassant[8];
  bool bEnPassant[8];
  bool kRookWhiteMoved;
  bool qRookWhiteMoved;
  bool kingWhiteMoved;
  bool kRookBlackMoved;
  bool qRookBlackMoved;
  bool kingBlackMoved;
  bool testMoves(char player, int sRow, int sCol, int eRow, int eCol);
  public:
    Board();
    ~Board();
    void clear();
    void defSetup();
    bool setPiece(char type, std::string location);
    bool rightTeam(char player, std::string place);
    bool checkPawnsSetup();
    bool checkKingsSetup();
    bool checkBlack(std::string location);
    bool checkWhite(std::string location);
    std::string findKing(char player);
    bool legalMove(std::string start, std::string end);
    bool anyMoves(char player);
    void print();
    bool move(char player, std::string start, std::string end);
    bool move(char player, std::string start, std::string end, char promotion);
    void undoMove(std::string start, std::string current, char deleted);
    std::string aiMove(char player);
    void clearEPFlags(int n); // 2 for black, 3 for white, 6 for both
};

#endif
