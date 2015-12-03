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
  graphics = new GraphicDisplay();
}

void Board::defSetup() {
  brd[0][0] = 'r';
  brd[0][7] = 'r';
  brd[0][1] = 'n';
  brd[0][6] = 'n';
  brd[0][2] = 'b';
  brd[0][5] = 'b';
  brd[0][3] = 'q';
  brd[0][4] = 'k';
  for (int i = 0 ; i < 8 ; i++) {
    brd[1][i] = 'p';
  }
  brd[7][0] = 'R';
  brd[7][7] = 'R';
  brd[7][1] = 'N';
  brd[7][6] = 'N';
  brd[7][2] = 'B';
  brd[7][5] = 'B';
  brd[7][3] = 'Q';
  brd[7][4] = 'K';
  for (int i = 0 ; i < 8 ; i++) {
    brd[6][i] = 'P';
  }
}

bool Board::setPiece(char type, string location) {
  int row = 8 - location[1] + '0';
  int col = location[0] - 'a';
  if (row < 0 || row > 7 || col < 0 || col > 7) return false;
  if (type == 0) {
    int rowMod = row % 2;
    int colMod = col % 2;
    if (rowMod == 0 && colMod == 0) {
      brd[row][col] = ' ';
    }
    else if (rowMod == 1 && colMod == 1) {
      brd[row][col] = ' ';
    }
    else {
      brd[row][col] = '_';
    }
    return true;
  }
  else if (type == 'r' || type == 'n' || type == 'b' || type == 'q' || type == 'k' || type == 'p' ||
            type == 'R' || type == 'N' || type == 'B' || type == 'Q' || type == 'K' || type == 'P') {
    brd[row][col] = type;
    return true;
  }
  return false;
}

bool Board::checkPawnsSetup() {
  for (int i = 0 ; i < 8 ; i++) {
    if (brd[0][i] == 'P') return false;
  }
  for (int i = 0 ; i < 8 ; i++) {
    if (brd[7][i] == 'p') return false;
  }
  return true;
}

bool Board::checkKingsSetup() {
  int whiteK = 0;
  int blackK = 0;
  for (int i = 0 ; i < 8 ; i++) {
    for (int j = 0 ; j < 8 ; j++) {
      if (brd[i][j] == 'k') blackK++;
      else if (brd[i][j] == 'K') whiteK++;
    }
  }
  if (whiteK == 1 && blackK == 1) return true;
  else return false;
}

void Board::print() {
  cout << endl;
  for (int i = 0 ; i < 8 ; i++) {
    int row = 8 - i;
    cout << row << " ";
    for (int j = 0 ; j < 8 ; j++) {
      cout << brd[i][j];
    }
    cout << endl;
  }
  cout << "  abcdefgh" << endl << endl;
}

bool Board::rightTeam(char player, string place) {
  int row = 8 - place[1] + '0';
  int col = place[0] - 'a';
  if (player == 'w' && brd[row][col] < 'Z' && brd[row][col] > 'A') return true;
  if (player == 'b' && brd[row][col] < 'z' && brd[row][col] > 'a') return true;
  return false;
}

bool Board::move(string start, string end) {
  if (legalMove(start, end)) {
    brd[8 - end[1] + '0'][end[0] - 'a'] = brd[8 - start[1] + '0'][start[0] - 'a'];
    int rowMod = start[1] % 2;
    int colMod = (start[0] - 'a') % 2;
    if (rowMod == 0 && colMod == 0) {
      brd[8 - start[1] + '0'][start[0] - 'a'] = ' ';
    }
    else if (rowMod == 1 && colMod == 1) {
      brd[8 - start[1] + '0'][start[0] - 'a'] = ' ';
    }
    else {
      brd[8 - start[1] + '0'][start[0] - 'a'] = '_';
    }
    graphics->notify(start[0] - 'a', 7 - (start[1] - '1'), end[0] - 'a', 7 - (end[1] - '1'), brd[8 - end[1] + '0'][end[0] - 'a']);
    return true;
  }
  return false;
}

bool Board::move(string start, string end, char promotion) {
  if (move(start, end)) {
    if ((brd[8 - end[1] + '0'][end[0] - 'a'] == 'P' && end[1] == '8') || (brd[8 - end[1] + '0'][end[0] - 'a'] == 'p' && end[1] == '1')) {
      brd[8 - end[1] + '0'][end[0] - 'a'] = promotion;
      return true;
    }
  }
  return false;
}

bool Board::legalMove(string start, string end) {
  int sRow = 8 - start[1] + '0';
  int sCol = start[0] - 'a';
  int eRow = 8 - end[1] + '0';
  int eCol = end[0] - 'a';
  if (sRow == eRow && sCol == eCol) return false;
  if (eRow < 0 || eRow > 7 || eCol < 0 || eCol > 7) return false;
  char pieceType = brd[sRow][sCol];
  if (pieceType == 'r' || pieceType == 'R') {
    if (sRow != eRow && sCol != eCol) return false;
    if (sCol == eCol && eRow > sRow) {
      for (int cRow = sRow+1 ; cRow < 8 ; cRow++) {
        if (cRow == eRow) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'r') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'R') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[cRow][eCol] != ' ' && brd[cRow][eCol] != '_') return false;
      }
      return false;
    }
    else if (sCol == eCol && eRow < sRow) {
      for (int cRow = sRow-1 ; cRow >= 0 ; cRow--) {
        if (cRow == eRow) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'r') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'R') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[cRow][eCol] != ' ' && brd[cRow][eCol] != '_') return false;
      }
      return false;
    }
    else if (sRow == eRow && eCol > sCol) {
      for (int cCol = sCol+1 ; cCol < 7 ; cCol++ ) {
        if (cCol == eCol) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'r') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'R') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[eRow][cCol] != ' ' && brd[eRow][cCol] != '_') return false;
      }
      return false;
    }
    else if (sRow == eRow && eCol < sCol) {
      for (int cCol = sCol-1 ; cCol >= 0 ; cCol--) {
        if (cCol == eCol) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'r') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'R') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[eRow][cCol] != ' ' && brd[eRow][cCol] != '_') return false;
      }
      return false;
    }
  }
  else if (pieceType == 'n' || pieceType == 'N') {
    if (pieceType == 'n' && brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return false;
    if (pieceType == 'N' && brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return false;
    if (eRow == sRow+2 && eCol == sCol+1) return true;
    if (eRow == sRow+2 && eCol == sCol-1) return true;
    if (eRow == sRow-2 && eCol == sCol+1) return true;
    if (eRow == sRow-2 && eCol == sCol-1) return true;
    if (eRow == sRow+1 && eCol == sCol+2) return true;
    if (eRow == sRow+1 && eCol == sCol-2) return true;
    if (eRow == sRow-1 && eCol == sCol+2) return true;
    if (eRow == sRow-1 && eCol == sCol-2) return true;
    return false;
  }
  else if (pieceType == 'b' || pieceType == 'B') {
    if (sRow == eRow || sCol == eCol) return false;
    for (int cRow = sRow+1, cCol = sCol+1 ; cRow < 8 && cCol < 8 ; cRow++, cCol++) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'b') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'B') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;;
    }
    for (int cRow = sRow+1, cCol = sCol-1 ; cRow < 8 && cCol >= 0 ; cRow++, cCol--) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'b') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'B') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;
    }
    for (int cRow = sRow-1, cCol = sCol+1 ; cRow >= 0 && cCol < 8 ; cRow--, cCol++) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'b') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'B') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;
    }
    for (int cRow = sRow-1, cCol = sCol-1 ; cRow >= 0 && cCol >= 0 ; cRow--, cCol--) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'b') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'B') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;
    }
    return false;
  }
  else if (pieceType == 'q' || pieceType == 'Q') {
    if (sCol == eCol && eRow > sRow) {
      for (int cRow = sRow+1 ; cRow < 8 ; cRow++) {
        if (cRow == eRow) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'q') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'Q') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[cRow][eCol] != ' ' && brd[cRow][eCol] != '_') return false;
      }
      return false;
    }
    else if (sCol == eCol && eRow < sRow) {
      for (int cRow = sRow-1 ; cRow >= 0 ; cRow--) {
        if (cRow == eRow) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'q') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'Q') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[cRow][eCol] != ' ' && brd[cRow][eCol] != '_') return false;
      }
      return false;
    }
    else if (sRow == eRow && eCol > sCol) {
      for (int cCol = sCol+1 ; cCol < 7 ; cCol++ ) {
        if (cCol == eCol) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'q') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'Q') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[eRow][cCol] != ' ' && brd[eRow][cCol] != '_') return false;
      }
      return false;
    }
    else if (sRow == eRow && eCol < sCol) {
      for (int cCol = sCol-1 ; cCol >= 0 ; cCol--) {
        if (cCol == eCol) {
          if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
          if (pieceType == 'q') {
            if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
          }
          else if (pieceType == 'Q') {
            if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
          }
          return false;
        }
        else if (brd[eRow][cCol] != ' ' && brd[eRow][cCol] != '_') return false;
      }
      return false;
    }
    for (int cRow = sRow+1, cCol = sCol+1 ; cRow < 8 && cCol < 8 ; cRow++, cCol++) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'q') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'Q') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;;
    }
    for (int cRow = sRow+1, cCol = sCol-1 ; cRow < 8 && cCol >= 0 ; cRow++, cCol--) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'q') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'Q') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;
    }
    for (int cRow = sRow-1, cCol = sCol+1 ; cRow >= 0 && cCol < 8 ; cRow--, cCol++) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'q') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'Q') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;
    }
    for (int cRow = sRow-1, cCol = sCol-1 ; cRow >= 0 && cCol >= 0 ; cRow--, cCol--) {
      if (cRow == eRow && cCol == eCol) {
        if (pieceType == 'q') {
          if (!(brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a')) return true;
          else return false;
        }
        else if (pieceType == 'Q') {
          if (!(brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A')) return true;
          else return false;
        }
      }
      else if (brd[cRow][cCol] != ' ' && brd[cRow][cCol] != '_') break;
    }
    return false;
  }
  else if (pieceType == 'k' || pieceType == 'K') {
    if (pieceType == 'k' && brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return false;
    if (pieceType == 'K' && brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return false;
    if (eRow == sRow && eCol == sCol-1) return true;
    if (eRow == sRow && eCol == sCol+1) return true;
    if (eRow == sRow+1 && eCol == sCol) return true;
    if (eRow == sRow-1 && eCol == sCol) return true;
    if (eRow == sRow+1 && eCol == sCol+1) return true;
    if (eRow == sRow+1 && eCol == sCol-1) return true;
    if (eRow == sRow-1 && eCol == sCol+1) return true;
    if (eRow == sRow-1 && eCol == sCol-1) return true;
                 //implement castling
    return false;
  }
  else if (pieceType == 'p') {
    if (sRow == 1) {
      if (eRow == sRow+1 && eCol == sCol) {
        if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
        else return false;
      }
      if (eRow == sRow+2 && eCol == sCol) {
        if ((brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') && (brd[eRow-1][eCol] == ' ' || brd[eRow-1][eCol] == '_')) return true;
        else return false;
      }
      if (eRow == sRow+1 && eCol == sCol-1) {
        if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
        else return false;
      }
      if (eRow == sRow+1 && eCol == sCol+1) {
        if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
        else return false;
      }
    }
    else {
      if (eRow != sRow+1) return false;
      if (eCol == sCol) {
        if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
        else return false;
      }
      if (eCol == sCol-1) {
        if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
        else return false;
      }
      if (eCol == sCol+1) {
        if (brd[eRow][eCol] < 'Z' && brd[eRow][eCol] > 'A') return true;
        else return false;
      }
    }
    return false;
  }
  else if (pieceType == 'P') {
    if (sRow == 6) {
      if (eRow == sRow-1 && eCol == sCol) {
        if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
        else return false;
      }
      if (eRow == sRow-2 && eCol == sCol) {
        if ((brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') && (brd[eRow-1][eCol] == ' ' || brd[eRow-1][eCol] == '_')) return true;
        else return false;
      }
      if (eRow == sRow-1 && eCol == sCol-1) {
        if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
        else return false;
      }
      if (eRow == sRow-1 && eCol == sCol+1) {
        if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
        else return false;
      }
    }
    else {
      if (eRow != sRow-1) return false;
      if (eCol == sCol) {
        if (brd[eRow][eCol] == ' ' || brd[eRow][eCol] == '_') return true;
        else return false;
      }
      if (eCol == sCol-1) {
        if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
        else return false;
      }
      if (eCol == sCol+1) {
        if (brd[eRow][eCol] < 'z' && brd[eRow][eCol] > 'a') return true;
        else return false;
      }
    } //En passent is tricky, not done yet
    return false;
  }
  return false;
}
