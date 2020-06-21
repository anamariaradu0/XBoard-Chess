#include <stdio.h>
#include <string>
#include <iostream>
#include "Cell.h"
#include "Board_Utils.h"
#include "Algo_Utils.h"
#include "Eval_Board.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#define BUFFER 100
#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6
using namespace std;

int main() {
    std::ofstream out;
    out.open("dummy.txt");
    std::string color = "black";

    // Placing pieces on the board
   Cell board[8][8] = {
           {Cell("a1", 2, "white"), Cell("b1", 3, "white"), Cell("c1", 4, "white"), Cell("d1", 5, "white"),
           Cell("e1", 6, "white"), Cell("f1", 4, "white"), Cell("g1", 3, "white"), Cell("h1", 2, "white")},
           {Cell("a2", 1, "white"), Cell("b2", 1, "white"), Cell("c2", 1, "white"), Cell("d2", 1, "white"),
           Cell("e2", 1, "white"), Cell("f2", 1, "white"), Cell("g2", 1, "white"), Cell("h2", 1, "white")},
           {Cell("a3"), Cell("b3"), Cell("c3"), Cell("d3"), Cell("e3"), Cell("f3"), Cell("g3"), Cell("h3")},
           {Cell("a4"), Cell("b4"), Cell("c4"), Cell("d4"), Cell("e4"), Cell("f4"), Cell("g4"), Cell("h4")},
           {Cell("a5"), Cell("b5"), Cell("c5"), Cell("d5"), Cell("e5"), Cell("f5"), Cell("g5"), Cell("h5")},
           {Cell("a6"), Cell("b6"), Cell("c6"), Cell("d6"), Cell("e6"), Cell("f6"), Cell("g6"), Cell("h6")},
           {Cell("a7", 1, "black"), Cell("b7", 1, "black"), Cell("c7", 1, "black"), Cell("d7", 1, "black"),
           Cell("e7", 1, "black"), Cell("f7", 1, "black"), Cell("g7", 1, "black"), Cell("h7", 1, "black")},
           {Cell("a8", 2, "black"), Cell("b8", 3, "black"), Cell("c8", 4, "black"), Cell("d8", 5, "black"),
           Cell("e8", 6, "black"), Cell("f8", 4, "black"), Cell("g8", 3, "black"), Cell("h8", 2, "black")}
   };

    // Placing pieces in players' arrays
    std::string whites[16] = {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
                              "a1", "h1", "b1", "g1", "c1", "f1", "d1", "e1"};

    std::string blacks[16] = {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
                              "a8", "h8", "b8", "g8", "c8", "f8", "d8", "e8"};

    char input[BUFFER];

    signal(SIGINT, SIG_IGN);

    fgets(input, BUFFER, stdin);

    fflush(stdout);

    std::string s;

    std::string verify;

    while (s != "quit") {
        std::cin >> s;
        out << s << "\n";

        if (s == "protover") {
            std::cout << "feature san=0\n";
        }

        // Check for a castling; if found, modify internal board accordingly and move
        if (is_castling(s)) {
            castling(s, board, whites, blacks);
            std::string command = MinMax(board, 2, color, whites, blacks, out);
            out << "COMMAND IS " << command << "\n";
            verify = command.substr(0, 4);
            out << "VERIFY IS " << verify << "\n";
            if (is_valid(verify, board, color, out)) {
                check_overtake(verify, board, whites, blacks, out);
                out << command;
                command = "move " + verify + "\n";
                std::cout << command;
                move_piece(verify, board, whites, blacks);
                continue;
            } else {
                vector <std::string> moves;

                if (color == "white") {
                    moves = list_all_moves(whites, board, "white", out);
                } else {
                    moves = list_all_moves(blacks, board, "black", out);
                }

                std::string move = "move " + moves[0] + "\n";
                std::cout << move;
            }
        }
        
        // Check for a pawn promotion; if found, modify internal board accordingly and move
        if (is_promotion(s)) {
            pawn_promotion(s, board, whites, blacks);
            std::string command = MinMax(board, 2, color, whites, blacks, out);
            out << "COMMAND IS " << command << "\n";
            verify = command.substr(0, 4);
            out << "VERIFY IS " << verify << "\n";
            if (is_valid(verify, board, color, out)) {
                check_overtake(verify, board, whites, blacks, out);
                out << command;
                command = "move " + verify + "\n";
                std::cout << command;
                move_piece(verify, board, whites, blacks);
                continue;
            } else {
                vector <std::string> moves;

                if (color == "white") {
                    moves = list_all_moves(whites, board, "white", out);
                } else {
                    moves = list_all_moves(blacks, board, "black", out);
                }

                std::string move = "move " + moves[0] + "\n";
                std::cout << move;
            }
        }
        
        // Check for colors; if engine is white, move
        if (s == "white") {
            color = "white";
            std::string command = MinMax(board, 2, color, whites, blacks, out);
            out << "COMMAND IS " << command << "\n";
            verify = command.substr(0, 4);
            out << "VERIFY IS " << verify << "\n";
            if (is_valid(verify, board, color, out)) {
                check_overtake(verify, board, whites, blacks, out);
                out << command;
                command = "move " + verify + "\n";
                std::cout << command;
                move_piece(verify, board, whites, blacks);
                continue;
            } else {
                vector <std::string> moves;

                if (color == "white") {
                    moves = list_all_moves(whites, board, "white", out);
                } else {
                    moves = list_all_moves(blacks, board, "black", out);
                }

                std::string move = "move " + moves[0] + "\n";
                std::cout << move;
            }
        }

        if (s == "black") {
            color = "black";
        }

        if (is_move(s)) {
            check_overtake(s, board, whites, blacks, out);
            move_piece(s, board, whites, blacks);

            std::string command = MinMax(board, 2, color, whites, blacks, out);
            out << "COMMAND IS " << command << "\n";
            verify = command.substr(0, 4);
            out << "VERIFY IS " << verify << "\n";
            if (is_valid(verify, board, color, out)) {
                check_overtake(verify, board, whites, blacks, out);
                out << command;
                command = "move " + verify + "\n";
                std::cout << command;
                move_piece(verify, board, whites, blacks);
                continue;
            } else {
                vector <std::string> moves;

                if (color == "white") {
                    moves = list_all_moves(whites, board, "white", out);
                } else {
                    moves = list_all_moves(blacks, board, "black", out);
                }

                std::string move = "move " + moves[0] + "\n";
                std::cout << move;
            }
        }

        // Treating analysis mode
        if (s == "resign") {
            std::cout << "resign" << "\n";
        }

        if (s == "force") {
            while (s != "go") {
                std::cin >> s;
                if (is_move(s)) {
                    check_overtake(s, board, whites, blacks, out);
                    move_piece(s, board, whites, blacks);
                } else if (s == "white") {
                    color = "white";
                } else if (s == "black") {
                    color = "black";
                }
            }

            if (s == "go") {
                std::string command = MinMax(board, 2, color, whites, blacks, out);
                out << "COMMAND IS " << command << "\n";
                verify = command.substr(0, 4);
                out << "VERIFY IS " << verify << "\n";
                if (is_valid(verify, board, color, out)) {
                    check_overtake(verify, board, whites, blacks, out);
                    out << command;
                    command = "move " + verify + "\n";
                    std::cout << command;
                    move_piece(verify, board, whites, blacks);
                    continue;
                } else {
                    std::cout << "resign" << "\n";
                }
            }

        }
    }

    print_hand(whites, out);

    print_hand(blacks, out);

    out.close();
    return 0;
}