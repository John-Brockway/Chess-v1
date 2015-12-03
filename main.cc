#include "board.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main() {
  cout << "Welcome to PP9K! By Scott Broeders and John Brockway, Fall 2015" << endl;
  string input;
  while (1) {                     // cycling through games
    char player = 'b';
    Board b;
    while (1) {
      cout << endl << "To start a new game, enter ''game x y'' where x and y are either " << endl;
      cout << "''human'' or ''computer[1-4]''. To enter setup, type ''setup''" << endl;
      cin >> input;
      if (input == "game") {
        b.defSetup();
        b.print();
        player = 'b';
        // human vs computer stuff
        break;
      }
      else if (input == "setup") {
        player = 'w';
        b.print();
        while (1) {
          char addType;
          getline (cin, input);
          istringstream iss(input);
          iss >> input;
          if (input == "+") {
            iss >> addType;
            iss >> input;
            bool validAdd = b.setPiece(addType, input);
            if (validAdd) b.print();
            else {
              cout << "Not a valid add" << endl;
            }
          }
          else if (input == "-") {
            iss >> input;
            bool validRem = b.setPiece(0, input);
            if (validRem) b.print();
            else {
              cout << "Not a valid remove" << endl;
            }
          }
          else if (input == "=") {
            iss >> input;
            if (input == "black") player = 'b';
            else player = 'w';
          }
          else if (input == "done") {
            bool pawns = b.checkPawnsSetup();
            bool kings = b.checkKingsSetup();
            if (!pawns) {
              cout << "Pawns on last row are illegal" << endl;
            }
            else if (!kings) {
              cout << "Need exactly one of each king" << endl;
            }
            else break;
          }
        }
        break;
      }
      else {
        cout << "Enter valid input" << endl;
      }
    }
    while (1) {                      // cycling through turns
      getline(cin, input);
      if (input == "resign") {
        if (player == 'w') {
          cout << "Black wins!" << endl;
        }
        else {
          cout << "White wins!" << endl;
        }
        break;
      }
      string input2;
      istringstream ss(input);
      ss >> input2;
      if (input2 == "move") {
        // if current player is computer {} else:
        string start;
        string end;
        char promotion;
        bool validTurn = false;
        ss >> start;
        ss >> end;
        if (ss >> promotion) validTurn = b.move(start, end, promotion);
        else validTurn = b.move(start, end);
        if (validTurn) b.print();
        else {
          cout << "That was not a valid move!" << endl;
          continue;
        }
      }
      if (player == 'w') player = 'b';
      else if (player == 'b') player = 'w';
    }
  }
}
