#include "player.h"
#include "board.h"
#include <string>

Player::Player(int i, Board *b, char c) {
  if (i != 0) {
    aiLevel = i;
  }
  board = b;
  colour = c;
}

void Player::movePiece(std::string start, std::string end) {
  board->move(colour, start, end);
}

void Player::movePiece(std::string start, std::string end, char prom) {
  board->move(colour, start, end, prom);
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
