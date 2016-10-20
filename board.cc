#include "board.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>
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
  clearEPFlags(6);
  kRookWhiteMoved = false;
  qRookWhiteMoved = false;
  kingWhiteMoved = false;
  kRookBlackMoved = false;
  qRookBlackMoved = false;
  kingBlackMoved = false;
}

void Board::clear() {
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
  graphics->clear();
}

void Board::clearEPFlags(int n)
{
  for (int i = 0; i < 8; i++)
  {
    if (n % 2 == 0)
      bEnPassant[i] = false;
    if (n % 3 == 0)
      wEnPassant[i] = false;
  }
}

Board::~Board() {
  delete graphics;
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
  graphics->defaultPieces();
}

bool Board::setPiece(char type, string location) {
  int row = 8 - location[1] + '0';
  int col = location[0] - 'a';
  if (row < 0 || row > 7 || col < 0 || col > 7) return false;
  graphics->notify(col, row, col, row, type);
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

bool Board::move(char player, string start, string end) {
  char deleted = brd[8 - end[1] + '0'][end[0] - 'a'];
  string takenPiece = "default"; // only for en passant
  if (legalMove(start, end)) {
    if ((brd[8 - start[1] + '0'][start[0] - 'a'] == 'P' || brd[8 - start[1] + '0'][start[0] - 'a'] == 'p') && (start[0] - 'a' != end[0] - 'a') && (brd[8 - end[1] + '0'][end[0] - 'a'] == ' ' || brd[8 - end[1] + '0'][end[0] - 'a'] == '_'))
    {
      takenPiece = end;
      takenPiece[1] = start[1];
      setPiece(0, takenPiece);
    }
    if (brd[8 - start[1] + '0'][start[0] - 'a'] == 'K')
    {
      if ((end[0] - start[0]) == 2) //king side
      {
	setPiece(0,"h1");
	setPiece('R',"f1");
      }
      else if ((end[0] - start[0]) == -2) //queen side
      {
	setPiece(0,"a1");
	setPiece('R',"d1");
      }
    }
    if (brd[8 - start[1] + '0'][start[0] - 'a'] == 'k')
    {
      if ((end[0] - start[0]) == 2) //king side
      {
	setPiece(0,"h8");
	setPiece('r',"f8");
      }
      else if ((end[0] - start[0]) == -2) //queen side
      {
	setPiece(0,"a8");
	setPiece('r',"d8");
      }
    }
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
    if ((brd[8 - end[1] + '0'][end[0] - 'a'] == 'p' || brd[8 - end[1] + '0'][end[0] - 'a'] == 'P') && ((end[1] - start[1]) == 2 || (end[1] - start[1]) == -2))
    {
      if (end[1] == '5')
	bEnPassant[end[0] - 'a'] = true;
      else
	wEnPassant[end[0] - 'a'] = true;
    }
    if (player == 'w' && checkWhite(findKing(player))) {
      if (takenPiece != "default")
      {
	deleted = 'p';
	setPiece(deleted, takenPiece);
      }
      undoMove(start, end, deleted);
      return false;
    }
    if (player == 'b' && checkBlack(findKing(player))) {
      if (takenPiece != "default")
      {
	deleted = 'p';
	setPiece(deleted, takenPiece);
      }
      undoMove(start, end, deleted);
      return false;
    }
    if (deleted == 'r')
    {
      if (start == "a8")
	qRookBlackMoved = true;
      else if (start == "h8")
	kRookBlackMoved = true;
    }
    else if (deleted == 'R')
    {
      if (start == "a1")
	qRookWhiteMoved = true;
      else if (start == "h1")
	kRookWhiteMoved = true;
    }
    else if (deleted == 'k')
      kingBlackMoved = true;
    else if (deleted == 'K')
      kingWhiteMoved = true; 
    return true;
  }
  return false;
}

bool Board::testMoves(char player, int sRow, int sCol, int eRow, int eCol) {
  char deleted = brd[eRow][eCol];
  string start = "a1";
  start[0] = sCol + 'a';
  start[1] = (8-sRow) + '0';
  string end = "a1";
  end[0] = eCol + 'a';
  end[1] = (8-eRow) + '0';
  if (rightTeam(player, start) && move(player, start, end)) {
    if (player == 'w' && !checkWhite(findKing('w'))) {
      undoMove(start, end, deleted);
      return true;
    }
    if (player == 'b' && !checkBlack(findKing('b'))) {
      undoMove(start, end, deleted);
      return true;
    }
    undoMove(start, end, deleted);
  }
  return false;
}

bool Board::move(char player, string start, string end, char promotion) {
  if (move(player, start, end)) {
    if ((brd[8 - end[1] + '0'][end[0] - 'a'] == 'P' && end[1] == '8') || (brd[8 - end[1] + '0'][end[0] - 'a'] == 'p' && end[1] == '1')) {
      brd[8 - end[1] + '0'][end[0] - 'a'] = promotion;
      return true;
    }
  }
  return false;
}

void Board::undoMove(string start, string current, char deleted) {
  int cRow = 8 - current[1] + '0';
  int cCol = current[0] - 'a';
  setPiece(brd[cRow][cCol], start);
  if (deleted == ' ' || deleted == '_') setPiece(0, current);
  else setPiece(deleted, current);
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
    if (eRow == sRow && eCol == sCol-1 && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow && eCol == sCol-1 && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow && eCol == sCol+1 && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow && eCol == sCol+1 && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow+1 && eCol == sCol && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow+1 && eCol == sCol && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow-1 && eCol == sCol && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow-1 && eCol == sCol && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow+1 && eCol == sCol+1 && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow+1 && eCol == sCol+1 && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow+1 && eCol == sCol-1 && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow+1 && eCol == sCol-1 && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow-1 && eCol == sCol+1 && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow-1 && eCol == sCol+1 && pieceType == 'K' && !checkWhite(end)) return true;
    if (eRow == sRow-1 && eCol == sCol-1 && pieceType == 'k' && !checkBlack(end)) return true;
    if (eRow == sRow-1 && eCol == sCol-1 && pieceType == 'K' && !checkWhite(end)) return true;

    if ((sRow == eRow) && (eCol == sCol - 2)) //queen side
    {
      if (pieceType =='K')  //white
	return (!qRookWhiteMoved && !kingWhiteMoved && !checkWhite("c1") && !checkWhite("d1") && !checkWhite("e1") && !(brd[7][1] < 'z' && brd[7][1] > 'a') && !(brd[7][2] < 'z' && brd[7][2] > 'a') && !(brd[7][3] < 'z' && brd[7][3] > 'a'));
      if (pieceType =='k') //black
	return (!qRookBlackMoved && !kingBlackMoved && !checkBlack("c8") && !checkBlack("d8") && !checkBlack("e8") && !(brd[0][1] < 'z' && brd[0][1] > 'a') && !(brd[0][2] < 'z' && brd[0][2] > 'a') && !(brd[0][3] < 'z' && brd[0][3] > 'a'));
    }
    if ((sRow == eRow) && (eCol == sCol + 2)) //king side
    {
      if (pieceType =='K')  //white
	return (!kRookWhiteMoved && !kingWhiteMoved && !checkWhite("e1") && !checkWhite("f1") && !checkWhite("g1") && !(brd[7][5] < 'z' && brd[7][5] > 'a') && !(brd[7][6] < 'z' && brd[7][6] > 'a'));
      if (pieceType =='k') //black
	return (!kRookBlackMoved && !kingBlackMoved && !checkBlack("e8") && !checkBlack("f8") && !checkBlack("g8") && !(brd[0][5] < 'z' && brd[0][5] > 'a') && !(brd[0][6] < 'z' && brd[0][6] > 'a'));
    }
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
      if (sRow == 4 && wEnPassant[eCol])
      {
	return (brd[sRow][eCol] == 'P');
      }
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
	if ((brd[sRow-1][eCol] == ' ' || brd[sRow-1][eCol] == '_') && (brd[sRow-2][eCol] == ' ' || brd[sRow-2][eCol] == '_')) return true;
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
      if (sRow == 3 && bEnPassant[eCol])
      {
	return (brd[sRow][eCol] == 'p');
      }
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
    }
    return false;
  }
  return false;
}

bool Board::anyMoves(char player) {
  for (int i = 0 ; i < 8 ; i++) {
    for (int j = 0 ; j < 8 ; j++) {
      if ((player == 'w' && brd[i][j] < 'Z' && brd[i][j] > 'A') || (player == 'b' && brd[i][j] < 'z' && brd[i][j] > 'a')) {
	if (brd[i][j] == 'r' || brd[i][j] == 'R') {
	  if (testMoves(player, i, j, i+1, j)) return true;
	  if (testMoves(player, i, j, i-1, j)) return true;
	  if (testMoves(player, i, j, i, j-1)) return true;
	  if (testMoves(player, i, j, i, j+1)) return true;
	}
	if (brd[i][j] == 'n' || brd[i][j] == 'N') {
	  if (testMoves(player, i, j, i+2, j+1)) return true;
	  if (testMoves(player, i, j, i+2, j-1)) return true;
	  if (testMoves(player, i, j, i-2, j+1)) return true;
	  if (testMoves(player, i, j, i-2, j-2)) return true;
	  if (testMoves(player, i, j, i+1, j+2)) return true;
	  if (testMoves(player, i, j, i+1, j-2)) return true;
	  if (testMoves(player, i, j, i-1, j+2)) return true;
	  if (testMoves(player, i, j, i-1, j-2)) return true;
	}
	if (brd[i][j] == 'b' || brd[i][j] == 'B') {
	  if (testMoves(player, i, j, i+1, j+1)) return true;
	  if (testMoves(player, i, j, i-1, j+1)) return true;
	  if (testMoves(player, i, j, i+1, j-1)) return true;
	  if (testMoves(player, i, j, i-1, j-1)) return true;
	}
	if (brd[i][j] == 'q' || brd[i][j] == 'Q') {
	  if (testMoves(player, i, j, i+1, j)) return true;
	  if (testMoves(player, i, j, i-1, j)) return true;
	  if (testMoves(player, i, j, i, j-1)) return true;
	  if (testMoves(player, i, j, i, j+1)) return true;
	  if (testMoves(player, i, j, i+1, j+1)) return true;
	  if (testMoves(player, i, j, i-1, j+1)) return true;
	  if (testMoves(player, i, j, i+1, j-1)) return true;
	  if (testMoves(player, i, j, i-1, j-1)) return true;

	}
	if (brd[i][j] == 'k' || brd[i][j] == 'K') {
	  if (testMoves(player, i, j, i, j-1)) return true;
	  if (testMoves(player, i, j, i, j+1)) return true;
	  if (testMoves(player, i, j, i-1, j)) return true;
	  if (testMoves(player, i, j, i+1, j)) return true;
	  if (testMoves(player, i, j, i+1, j+1)) return true;
	  if (testMoves(player, i, j, i+1, j-1)) return true;
	  if (testMoves(player, i, j, i-1, j+1)) return true;
	  if (testMoves(player, i, j, i-1, j-1)) return true;
	}
	if (brd[i][j] == 'p' || brd[i][j] == 'P') {
	  if (testMoves(player, i, j, i+1, j)) return true;
	  if (testMoves(player, i, j, i+1, j-1)) return true;
	  if (testMoves(player, i, j, i+1, j+1)) return true;
	}
      }
    }
  }
  return false;
}

string Board::aiMove(char player) {
  vector<string> moves;
  string location = "a1 a1";
  for (int i = 0 ; i < 8 ; i++) {
    for (int j = 0 ; j < 8 ; j++) {
      if ((player == 'w' && brd[i][j] < 'Z' && brd[i][j] > 'A') || (player == 'b' && brd[i][j] < 'z' && brd[i][j] > 'a')) {
	if (brd[i][j] == 'r' || brd[i][j] == 'R') {
	  for (int ii = i+1 ; ii < 8 ; ii++) {
	    if (testMoves(player, i, j, ii, j)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = j+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i-1 ; ii >= 0 ; ii--) {
	    if (testMoves(player, i, j, ii, j)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = j+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int jj = j-1 ; jj >= 0 ; jj--) {
	    if (testMoves(player, i, j, i, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-i)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int jj = j+1 ; jj < 8 ; jj++) {
	    if (testMoves(player, i, j, i, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-i)+'0';
	      moves.push_back(location);
	    }
	  }
	  continue;
	}
	if (brd[i][j] == 'n' || brd[i][j] == 'N') {
	  if (testMoves(player, i, j, i+2, j+1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1+'a';
	    location[4] = (8-i+2)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+2, j-1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-1+'a';
	    location[4] = (8-i+2)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-2, j+1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1+'a';
	    location[4] = (8-i-2)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-2, j-1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-1+'a';
	    location[4] = (8-i-2)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j+2)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+2+'a';
	    location[4] = (8-i+1)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j-2)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-2+'a';
	    location[4] = (8-i+1)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-1, j+2)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+2+'a';
	    location[4] = (8-i-1)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-1, j-2)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-2+'a';
	    location[4] = (8-i-1)+'0';
	    moves.push_back(location);
	  }
	}
	if (brd[i][j] == 'b' || brd[i][j] == 'B') {
	  for (int ii = i+1, jj = j+1 ; ii < 8 && jj < 8 ; ii++, jj++) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i-1, jj = j+1 ; ii >= 0 && jj < 8 ; ii--, jj++) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i+1, jj = j-1 ; ii < 8 && jj >= 0 ; ii++, jj--) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i-1, jj = j-1 ; ii >= 0 && jj >= 0 ; ii--, jj--) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	}
	if (brd[i][j] == 'q' || brd[i][j] == 'Q') {
	  for (int ii = i+1 ; ii < 8 ; ii++) {
	    if (testMoves(player, i, j, ii, j)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = j+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i-1 ; ii >= 0 ; ii--) {
	    if (testMoves(player, i, j, ii, j)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = j+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int jj = j-1 ; jj >= 0 ; jj--) {
	    if (testMoves(player, i, j, i, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-i)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int jj = j+1 ; jj < 8 ; jj++) {
	    if (testMoves(player, i, j, i, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-i)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i+1, jj = j+1 ; ii < 8 && jj < 8 ; ii++, jj++) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i-1, jj = j+1 ; ii >= 0 && jj < 8 ; ii--, jj++) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i+1, jj = j-1 ; ii < 8 && jj >= 0 ; ii++, jj--) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	  for (int ii = i-1, jj = j-1 ; ii >= 0 && jj >= 0 ; ii--, jj--) {
	    if (testMoves(player, i, j, ii, jj)) {
	      location[0] = j+'a';
	      location[1] = (8-i)+'0';
	      location[3] = jj+'a';
	      location[4] = (8-ii)+'0';
	      moves.push_back(location);
	    }
	  }
	}
	if (brd[i][j] == 'k' || brd[i][j] == 'K') {
	  if (testMoves(player, i, j, i, j-1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-1+'a';
	    location[4] = (8-i)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i, j+1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1+'a';
	    location[4] = (8-i)+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-1, j)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+'a';
	    location[4] = (8-(i-1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+'a';
	    location[4] = (8-(i+1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j+1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1+'a';
	    location[4] = (8-(i+1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j-1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-1+'a';
	    location[4] = (8-(i+1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-1, j+1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1+'a';
	    location[4] = (8-(i-1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i-1, j-1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j-1+'a';
	    location[4] = (8-(i-1))+'0';
	    moves.push_back(location);
	  }
	}
	if (brd[i][j] == 'p') {
	  if (testMoves(player, i, j, i+1, j)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+'a';
	    location[4] = (8-(i+1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j-1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1-'a';
	    location[4] = (8-(i+1))+'0';
	    moves.push_back(location);
	  }
	  if (testMoves(player, i, j, i+1, j+1)) {
	    location[0] = j+'a';
	    location[1] = (8-i)+'0';
	    location[3] = j+1+'a';
	    location[4] = (8-(i+1))+'0';
	    moves.push_back(location);
	  }
	}
        if (brd[i][j] == 'P') {
          if (testMoves(player, i, j, i-1, j)) {
            location[0] = j+'a';
            location[1] = (8-i)+'0';
            location[3] = j+'a';
            location[4] = (8-(i-1))+'0';
            moves.push_back(location);
          }
          if (testMoves(player, i, j, i-1, j-1)) {
            location[0] = j+'a';
            location[1] = (8-i)+'0';
            location[3] = j+1-'a';
            location[4] = (8-(i-1))+'0';
            moves.push_back(location);
          }
          if (testMoves(player, i, j, i-1, j+1)) {
            location[0] = j+'a';
            location[1] = (8-i)+'0';
            location[3] = j+1+'a';
            location[4] = (8-(i-1))+'0';
            moves.push_back(location);
          }
        }
      }
    }
  }
  srand(time(0));
  int chosenMove = rand() % moves.size();
  string start = (moves.at(chosenMove)).substr(0, 2);
  string end = (moves.at(chosenMove)).substr(3, 2);
  move(player, start, end);
  return moves.at(chosenMove);
}

string Board::findKing(char player) {
  string location = "a1";
  for (int i = 0 ; i < 8 ; i++) {
    for (int j = 0 ; j < 8 ; j++) {
      if (player == 'w' && brd[i][j] == 'K') {
	location[0] = j + 'a';
	location[1] = (8 - i) + '0';
	return location;
      }
      else if (player == 'b' && brd[i][j] == 'k') {
	location[0] = j + 'a';
	location[1] = (8 - i) + '0';
	return location;
      }
    }
  }
  return "";
}

bool Board::checkBlack (string location) {
  int row = 8 - location[1] + '0';
  int col = location [0] - 'a';

  for (int cCol = col-1 ; cCol >= 0 ; cCol--) {                                // check horizontals and verticals
    if (brd[row][cCol] == 'R' || brd[row][cCol] == 'Q') return true;
    if (brd[row][cCol] < 'z' && brd[row][cCol] > 'a') break;
    if (brd[row][cCol] < 'Z' && brd[row][cCol] > 'A') break;
  }
  for (int cCol = col+1 ; cCol < 8 ; cCol++) {
    if (brd[row][cCol] == 'R' || brd[row][cCol] == 'Q') return true;
    if (brd[row][cCol] < 'z' && brd[row][cCol] > 'a') break;
    if (brd[row][cCol] < 'Z' && brd[row][cCol] > 'A') break;
  }
  for (int cRow = row-1 ; cRow >= 0 ; cRow--) {
    if (brd[cRow][col] == 'R' || brd[cRow][col] == 'Q') return true;
    if (brd[cRow][col] < 'z' && brd[cRow][col] > 'a') break;
    if (brd[cRow][col] < 'Z' && brd[cRow][col] > 'A') break;
  }
  for (int cRow = row+1 ; cRow < 8 ; cRow++) {
    if (brd[cRow][col] == 'R' || brd[cRow][col] == 'Q') return true;
    if (brd[cRow][col] < 'z' && brd[cRow][col] > 'a') break;
    if (brd[cRow][col] < 'Z' && brd[cRow][col] > 'A') break;
  }

  for (int cRow = row+1, cCol = col+1 ; cRow < 8 && cCol < 8 ; cRow++, cCol++) {    // check diagonals
    if (brd[cRow][cCol] == 'B' || brd[cRow][cCol] == 'Q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }
  for (int cRow = row+1, cCol = col-1 ; cRow < 8 && cCol >= 0 ; cRow++, cCol--) {
    if (brd[cRow][cCol] == 'B' || brd[cRow][cCol] == 'Q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }
  for (int cRow = row-1, cCol = col+1 ; cRow >= 0 && cCol < 8 ; cRow--, cCol++) {
    if (brd[cRow][cCol] == 'B' || brd[cRow][cCol] == 'Q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }
  for (int cRow = row-1, cCol = col-1 ; cRow >= 0 && cCol >= 0 ; cRow--, cCol--) {
    if (brd[cRow][cCol] == 'B' || brd[cRow][cCol] == 'Q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }

  if (row+2 < 8 && col+1 < 8 && brd[row+2][col+1] == 'N') return true;             // check knights
  if (row+2 < 8 && col-1 >= 0 && brd[row+2][col-1] == 'N') return true;
  if (row-2 >= 0 && col+1 < 8 && brd[row-2][col+1] == 'N') return true;
  if (row-2 >= 0 && col-1 >=0 && brd[row-2][col-1] == 'N') return true;
  if (row+1 < 8 && col+2 < 8 && brd[row+1][col+2] == 'N') return true;
  if (row+1 < 8 && col-2 >= 0 && brd[row+1][col-2] == 'N') return true;
  if (row-1 >= 0 && col+2 < 8 && brd[row-1][col+2] == 'N') return true;
  if (row-1 >= 0 && col-2 >=0 && brd[row-1][col-2] == 'N') return true;

  if (row+1 < 8 && col-1 < 8 && brd[row+1][col+1] == 'P') return true;             // check pawns
  if (row+1 < 8 && col-1 >= 0 && brd[row+1][col-1] == 'P') return true;

  if (col+1 < 8 && brd[row][col+1] == 'K') return true;                            // check king
  if (col-1 >= 0 && brd[row][col-1] == 'K') return true;
  if (row+1 < 8 && brd[row+1][col] == 'K') return true;
  if (row-1 >= 0 && brd[row-1][col] == 'K') return true;
  if (row+1 < 8 && col+1 < 8 && brd[row+1][col+1] == 'K') return true;
  if (row+1 < 8 && col-1 >= 0 && brd[row+1][col-1] == 'K') return true;
  if (row-1 >= 0 && col+1 < 8 && brd[row-1][col+1] == 'K') return true;
  if (row-1 >= 0 && col-1 >= 0 && brd[row-1][col-1] == 'K') return true;

  return false;
}

bool Board::checkWhite (string location) {
  int row = 8 - location[1] + '0';
  int col = location[0] - 'a';

  for (int cCol = col-1 ; cCol >= 0 ; cCol--) {                                // check horizontals and verticals
    if (brd[row][cCol] == 'r' || brd[row][cCol] == 'q') return true;
    if (brd[row][cCol] < 'z' && brd[row][cCol] > 'a') break;
    if (brd[row][cCol] < 'Z' && brd[row][cCol] > 'A') break;
  }
  for (int cCol = col+1 ; cCol < 8 ; cCol++) {
    if (brd[row][cCol] == 'r' || brd[row][cCol] == 'q') return true;
    if (brd[row][cCol] < 'z' && brd[row][cCol] > 'a') break;
    if (brd[row][cCol] < 'Z' && brd[row][cCol] > 'A') break;
  }
  for (int cRow = row-1 ; cRow >= 0 ; cRow--) {
    if (brd[cRow][col] == 'r' || brd[cRow][col] == 'q') return true;
    if (brd[cRow][col] < 'z' && brd[cRow][col] > 'a') break;
    if (brd[cRow][col] < 'Z' && brd[cRow][col] > 'A') break;
  }
  for (int cRow = row+1 ; cRow < 8 ; cRow++) {
    if (brd[cRow][col] == 'r' || brd[cRow][col] == 'q') return true;
    if (brd[cRow][col] < 'z' && brd[cRow][col] > 'a') break;
    if (brd[cRow][col] < 'Z' && brd[cRow][col] > 'A') break;
  }

  for (int cRow = row+1, cCol = col+1 ; cRow < 8 && cCol < 8 ; cRow++, cCol++) {    // check diagonals
    if (brd[cRow][cCol] == 'b' || brd[cRow][cCol] == 'q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }
  for (int cRow = row+1, cCol = col-1 ; cRow < 8 && cCol >= 0 ; cRow++, cCol--) {
    if (brd[cRow][cCol] == 'b' || brd[cRow][cCol] == 'q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }
  for (int cRow = row-1, cCol = col+1 ; cRow >= 0 && cCol < 8 ; cRow--, cCol++) {
    if (brd[cRow][cCol] == 'b' || brd[cRow][cCol] == 'q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }
  for (int cRow = row-1, cCol = col-1 ; cRow >= 0 && cCol >= 0 ; cRow--, cCol--) {
    if (brd[cRow][cCol] == 'b' || brd[cRow][cCol] == 'q') return true;
    if (brd[cRow][cCol] < 'z' && brd[cRow][cCol] > 'a') break;
    if (brd[cRow][cCol] < 'Z' && brd[cRow][cCol] > 'A') break;
  }

  if (row+2 < 8 && col+1 < 8 && brd[row+2][col+1] == 'n') return true;             // check knights
  if (row+2 < 8 && col-1 >= 0 && brd[row+2][col-1] == 'n') return true;
  if (row-2 >= 0 && col+1 < 8 && brd[row-2][col+1] == 'n') return true;
  if (row-2 >= 0 && col-1 >=0 && brd[row-2][col-1] == 'n') return true;
  if (row+1 < 8 && col+2 < 8 && brd[row+1][col+2] == 'n') return true;
  if (row+1 < 8 && col-2 >= 0 && brd[row+1][col-2] == 'n') return true;
  if (row-1 >= 0 && col+2 < 8 && brd[row-1][col+2] == 'n') return true;
  if (row-1 >= 0 && col-2 >=0 && brd[row-1][col-2] == 'n') return true;

  if (row+1 < 8 && col-1 < 8 && brd[row+1][col+1] == 'p') return true;             // check pawns
  if (row+1 < 8 && col-1 >= 0 && brd[row+1][col-1] == 'p') return true;

  if (col+1 < 8 && brd[row][col+1] == 'k') return true;                            // check king
  if (col-1 >= 0 && brd[row][col-1] == 'k') return true;
  if (row+1 < 8 && brd[row+1][col] == 'k') return true;
  if (row-1 >= 0 && brd[row-1][col] == 'k') return true;
  if (row+1 < 8 && col+1 < 8 && brd[row+1][col+1] == 'k') return true;
  if (row+1 < 8 && col-1 >= 0 && brd[row+1][col-1] == 'k') return true;
  if (row-1 >= 0 && col+1 < 8 && brd[row-1][col+1] == 'k') return true;
  if (row-1 >= 0 && col-1 >= 0 && brd[row-1][col-1] == 'k') return true;

  return false;
}
