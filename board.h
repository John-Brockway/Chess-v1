#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "graphicdisplay.h"

class Board {
  GraphicDisplay *graphics;
  char brd[8][8];
  public:
    Board();
    void defSetup();
    bool setPiece(char type, std::string location);
    bool rightTeam(char player, std::string place);
    bool checkPawnsSetup();
    bool checkKingsSetup();
    bool checkBlack(std::string location);
    bool checkWhite(std::string location);
    std::string findKing(char player);
    bool legalMove(std::string start, std::string end);
    void print();
    bool move(std::string start, std::string end);
    bool move(std::string start, std::string end, char promotion);
//    aiMove(int aiLevel); (Too tied to how legalMoves is implemented to work on yet)
//    std::string gameEnd(); (Need to know about check scenarios to implement)
};

#endif
