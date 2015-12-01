#ifndef BOARD_H
#define BOARD_H

#include <string>

class Board {
//  GraphicDisplay *graphics;
  char brd[8][8];
  public:
    Board();
//    void setPiece(int i, int j); (I'm not sure what the integers represent)
    bool legalMove(char);
    void print();
    bool move(std::string start, std::string end);
    bool move(std::string start, std::string end, char promotion);
//    aiMove(int aiLevel); (Too tied to how legalMoves is implemented to work on yet)
//    std::string gameEnd(); (Need to know about check scenarios to implement)
};

#endif
