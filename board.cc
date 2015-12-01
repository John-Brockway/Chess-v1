#include "board.h"
#include <string>
#include <iostream>
using namespace std;

Board::Board() {
  int k = 0;
  for (int i = 0 ; i < 8 ; i++) {
    for (int j = 0 ; j < 8 ; j++) {
      if (k == 0) {
        brd[i][j] = ' ';
        if (j != 7) k++;
      }
      else {
        brd[i][j] = '_';
        if (j != 7) k--;
      }
    }
  }
}

void Board::print() {
  for (int i = 0 ; i < 8 ; i++) {
    int row = 8 - i;
    cout << row << " ";
    for (int j = 0 ; j < 8 ; j++) {
      cout << brd[i][j];
    }
    cout << endl;
  }
  cout << "  abcdefgh" << endl;
}

bool Board::move(string start, string end) {
  start += ' ';
  start += end;
  if (legalMove(start)) {
    brd[8 - end[1]][end[0] - 'a'] = brd[8 - start[1]][start[0] - 'a'];
    int rowMod = start[1] % 2;
    int colMod = (start[0] - 'a') % 2;
    if (rowMod == 0 && colMod == 0) {
      brd[8 - start[1]][start[0] - 'a'] = ' ';
    }
    else if (rowMod == 1 && colMod == 1) {
      brd[8 - start[1]][start[0] - 'a'] = ' ';
    }
    else {
      brd[8 - start[1]][start[0] - 'a'] = '_';
    }
    return true;
  }
  return false;
}

bool Board::move(string start, string end, char promotion) {
  if (move(start, end)) {
    brd[8 - end[1]][end[0] - 'a'] = promotion;
    return true;
  }
  return false;
}
