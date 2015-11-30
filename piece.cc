#include "piece.h"

Piece::Piece(char t) {
  type = t;
}

char Piece::getType() {
  return type;
}

void Piece::promote(char t) {
  if (type == 'p' || type == 'P') {
    type == t;
  }
}

