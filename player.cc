#include "player.h"
#include "board.h"
#include <string>

Player::Player(int i, Board *b) {
  if (i != 0) {
    aiLevel = i;
  }
  board = b;
}

void Player::movePiece(std::string start, std::string end) {
  board->move(start, end);
}

void Player::movePiece(std::string start, std::string end, char prom) {
  board->move(start, end, prom);
}

void Player::aiMovePiece() {
 // board->aiMove(aiLevel);
}

void Player::setCheck(bool c) {
  check = c;
}

bool Player::checkCheck() {
  return check;
}

int Player::getAILevel() {
  return aiLevel;
}
