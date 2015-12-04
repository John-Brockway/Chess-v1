#include "board.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
  cout << "Welcome to PP9K! By Scott Broeders and John Brockway, Fall 2015" << endl;
  string input;
  bool gameOn = true;
  while (gameOn) {                     // cycling through games
    char player = 'b';
    Board b;
    if (argc == 1) {
      while (1) {
        cout << endl << "To start a new game, enter ''game x y'' where x and y are either " << endl;
        cout << "''human'' or ''computer[1-4]''. To enter setup, type ''setup''" << endl;
        cout << "To stop playing for right now, type ''quit''" << endl;
        getline(cin, input);
        istringstream str(input);
        input = "";
        str >> input;
        if (input == "game") {
          b.defSetup();
          b.print();
          player = 'w';
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
        else if (input == "quit") {
          gameOn = false;
          break;
        }
        else {
          cout << "Enter valid input" << endl;
        }
      }
      argc;
    }
    else {
      string line;
      ifstream save(argv[1]);
      for (int i = 0 ; i < 8 ; i++) {
        getline(save, line);
        for (int j = 0 ; j < 8 ; j++) {
          if (line[j] != '_') {
            string placement = "a1";
            placement[0] = j+'a';
            placement[1] = (8 - i) + '0';
            b.setPiece(line[j], placement);
          }
        }
      }
      getline(save, line);
      player = line[0] - 'A' +'a';
      argc = 0;
      b.print();
    }
    while (gameOn) {                      // cycling through turns
      cout << player << "'s Move: ";
      input = "";
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
        bool rightTeam = b.rightTeam(player, start);
        if (rightTeam) {
          if (ss >> promotion) validTurn = b.move(start, end, promotion);
          else validTurn = b.move(start, end);
          if (validTurn) b.print();
          else {
            cout << "That was not a valid move!" << endl;
            continue;
          }
        }
        else {
         cout << "Pick your own piece!" << endl;
         continue;
        }
      }
      else {
      	cout << "That was not a valid command." << endl;
	continue;
      }

      if (b.findKing('w') == "") {
        cout << "Checkmate! Black wins!" << endl;
        break;
      }
      if (b.findKing('b') == "") {
        cout << "Checkmate! White wins!" << endl;
      }

      if (b.checkWhite(b.findKing('w'))) {
        cout << "White is in check!" << endl;
      }
      if (b.checkBlack(b.findKing('b'))) {
        cout << "Black is in check!" << endl;
      }
      if (player == 'w') player = 'b';
      else if (player == 'b') player = 'w';
    }
  }
}

