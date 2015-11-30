#ifndef PIECE_H
#define PIECE_H

class Piece {
  char type;
  public:
    Piece(char t);
    char getType();
    void promote(char t);
};

#endif
