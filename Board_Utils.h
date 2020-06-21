/*
* Auxiliary functions pertaining to internal changes, special moves,
* validating moves, some printing functions.
*/
#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "Cell.h"
#include "Eval_Board.h"

#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6
#define MOVE_SIZE 4
#define BOARD_SIZE 8
#define COLOR_SIZE 16
#define PROMOTION_SIZE 5

/*
 * Moves a piece on the internal board, updtes the current and previous
 * positions, updates position arrays.
 */
void move_piece(string positions, Cell board[BOARD_SIZE][BOARD_SIZE],
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE]) {
    // Transforming the indexes from standard board positions to the
    // inner board indexes (0-7 x 0-7)
    int oldy = positions[0] - 'a';
    int oldx = positions[1] - '0' - 1;
    int newy = positions[2] - 'a';
    int newx = positions[3] - '0' - 1;

    std::string init_pos{positions[0], positions[1]};
    std::string new_pos{positions[2], positions[3]};

    // Update the position of the piece n the player's "hand"
    // (the vector containing the position of every piece of the player's color)
    bool isWhite = false;
    for (int i = 0; i < COLOR_SIZE; ++i) {
        if (whites[i] == init_pos) {
            whites[i] = new_pos;
            isWhite = true;
            break;
        }
    }

    if (isWhite == false) {
        for (int i = 0; i < COLOR_SIZE; ++i) {
            if (blacks[i] == init_pos) {
                blacks[i] = new_pos;
                break;
            }
        }
    }

    // Update current and previous positions for the board
    board[oldx][oldy].prev_isOccupied = board[oldx][oldy].isOccupied;
    board[oldx][oldy].prev_pieceColor = board[oldx][oldy].pieceColor;
    board[oldx][oldy].prev_pieceType = board[oldx][oldy].pieceType;

    board[newx][newy].prev_isOccupied = board[newx][newy].isOccupied;
    board[newx][newy].prev_pieceColor = board[newx][newy].pieceColor;
    board[newx][newy].prev_pieceType = board[newx][newy].pieceType;

    board[newx][newy].pieceType = board[oldx][oldy].pieceType;
    board[newx][newy].pieceColor = board[oldx][oldy].pieceColor;
    board[newx][newy].isOccupied = true;

    board[oldx][oldy].pieceType = 0;
    board[oldx][oldy].pieceColor = "";
    board[oldx][oldy].isOccupied = false;
}

// Checks if a given move is a possible castling
bool is_castling(std::string s) {
    return s == "e1g1" || s == "e1c1" || s == "e8c8" || s == "e8g8";
}

// Checks if a given move is a pawn promotion
bool is_promotion(std::string s) {
    if (s.size() != PROMOTION_SIZE) {
        return false;
    }

    return isalpha(s[0]) && isalpha(s[2]) && isalpha(s[4]) && isdigit(s[1]) && isdigit(s[3]);
}

// Updates the internal representations after a pawn promotion
void pawn_promotion(std::string s, Cell board[BOARD_SIZE][BOARD_SIZE],
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE]) {
    if (is_promotion(s)) {
        std::string move = s.substr(0, 4);
        int oldy = move[0] - 'a';
        int oldx = move[1] - '0' - 1;
        int newy = move[2] - 'a';
        int newx = move[3] - '0' - 1;

        // Updates the internal board with the new piece type
        move_piece(move, board, whites, blacks);
        board[newx][newy].prev_pieceType = board[newx][newy].pieceType;

        char c = s[MOVE_SIZE];
        if (c == 'p') {
            board[newx][newy].pieceType = PAWN;
        }
        if (c == 'r') {
            board[newx][newy].pieceType = ROOK;
        }
        if (c == 'k') {
            board[newx][newy].pieceType = KNIGHT;
        }
        if (c == 'b') {
            board[newx][newy].pieceType = BISHOP;
        }
    }
}

// Updates the internal representations after a castling
void castling(std::string s, Cell board[BOARD_SIZE][BOARD_SIZE],
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE]) {
    if (s == "e1g1") {
        if (whites[15] == "e1") {
            whites[15] = "g1";
            whites[9] = "f1";

            board[0][4].isOccupied = false;
            board[0][4].pieceColor = "";
            board[0][4].pieceType = 0;

            board[0][6].isOccupied = true;
            board[0][6].pieceColor = "white";
            board[0][6].pieceType = 6;

            board[0][7].isOccupied = false;
            board[0][7].pieceColor = "";
            board[0][7].pieceType = 0;

            board[0][5].isOccupied = true;
            board[0][5].pieceColor = "white";
            board[0][5].pieceType = 2;
        }
    }

    if (s == "e1c1") {
        if (whites[15] == "e1") {
            whites[15] = "c1";
            whites[8] = "d1";

            board[0][4].isOccupied = false;
            board[0][4].pieceColor = "";
            board[0][4].pieceType = 0;

            board[0][2].isOccupied = true;
            board[0][2].pieceColor = "white";
            board[0][2].pieceType = 6;

            board[0][0].isOccupied = false;
            board[0][0].pieceColor = "";
            board[0][0].pieceType = 0;

            board[0][3].isOccupied = true;
            board[0][3].pieceColor = "white";
            board[0][3].pieceType = 2;
        }
    }

    if (s == "e8g8") {
        if (blacks[15] == "e8") {
            blacks[15] = "g8";
            blacks[9] = "f8";

            board[7][4].isOccupied = false;
            board[7][4].pieceColor = "";
            board[7][4].pieceType = 0;

            board[7][6].isOccupied = true;
            board[7][6].pieceColor = "white";
            board[7][6].pieceType = 6;

            board[7][7].isOccupied = false;
            board[7][7].pieceColor = "";
            board[7][7].pieceType = 0;

            board[7][5].isOccupied = true;
            board[7][5].pieceColor = "white";
            board[7][5].pieceType = 2;
        }
    }

    if (s == "e8c8") {
        if (blacks[15] == "e8") {
            blacks[15] = "c8";
            blacks[8] = "d8";

            board[7][4].isOccupied = false;
            board[7][4].pieceColor = "";
            board[7][4].pieceType = 0;

            board[7][2].isOccupied = true;
            board[7][2].pieceColor = "white";
            board[7][2].pieceType = 6;

            board[7][0].isOccupied = false;
            board[7][0].pieceColor = "";
            board[7][0].pieceType = 0;

            board[7][3].isOccupied = true;
            board[7][3].pieceColor = "white";
            board[7][3].pieceType = 2;
        }
    }
}

/* 
* Auxiliary function for is_valid that checks if a move can be
* performed diagonally.
*/
bool diagonal_check(int oldx, int oldy, int newx, int newy,
    Cell board[BOARD_SIZE][BOARD_SIZE]) {
    if (abs(newx - oldx) != abs(newy - oldy)) {
        return false;
    }

    if (oldx < newx) {
        if (oldy < newy) {
            int i = oldx + 1;
            int j = oldy + 1;
            for (int k = 0; k < (newx - oldx); ++k) {
                if (board[i][j].isOccupied == true ||
                    board[i][j].pieceColor == board[newx][newy].pieceColor) {
                    return false;
                } else {
                    ++i;
                    ++j;
                }
            }
        }

        if (oldy > newy) {
            int i = oldx + 1;
            int j = oldy - 1;
            for (int k = 0; k < (newx - oldx); ++k) {
                if (board[i][j].isOccupied == true ||
                    board[i][j].pieceColor == board[newx][newy].pieceColor) {
                    return false;
                } else {
                    ++i;
                    --j;
                }
            }
        }
    }

    if (oldx > newx) {
        if (oldy < newy) {
            int i = oldx - 1;
            int j = oldy + 1;
            for (int k = 0; k < (oldx - newx); ++k) {
                if (board[i][j].isOccupied == true ||
                    board[i][j].pieceColor == board[newx][newy].pieceColor) {
                    return false;
                } else {
                    --i;
                    ++j;
                }
            }
        }

        if (oldy > newy) {
            int i = oldx - 1;
            int j = oldy - 1;
            for (int k = 0; k < (oldx - newx); ++k) {
                if (board[i][j].isOccupied == true ||
                    board[i][j].pieceColor == board[newx][newy].pieceColor) {
                    return false;
                } else {
                    --i;
                    --j;
                }
            }
        }
    }

    return true;
}

/* 
* Auxiliary function for is_valid that checks if a move can be
* performed on either linear direction.
*/
bool linear_check(int oldx, int oldy, int newx, int newy,
    Cell board[BOARD_SIZE][BOARD_SIZE]) {
    if (oldx != newx && oldy != newy) {
        return false;
    }

    if (oldx == newx) {
        if (oldy < newy) {
            int j = oldy + 1;
            for (int k = 0; k < (newy - oldy); ++k) {
                if (board[newx][j].isOccupied == true ||
                    board[oldx][oldy].pieceColor ==  board[oldx][j].pieceColor) {
                    return false;
                } else {
                    ++j;
                }
            }
        }

        if (oldy > newy) {
            int j = oldy - 1;
            for (int k = 0; k < (oldy - newy); ++k) {
                if (board[newx][j].isOccupied == true ||
                    board[oldx][oldy].pieceColor ==  board[oldx][j].pieceColor) {
                    return false;
                } else {
                    --j;
                }
            }
        }
    }

    if (oldy == newy) {
        if (oldx < newx) {
            int i = oldx + 1;
            for (int k = 0; k < (newx - oldx); ++k) {
                if (board[i][newy].isOccupied == true ||
                    board[oldx][oldy].pieceColor ==  board[i][newy].pieceColor) {
                    return false;
                } else {
                    ++i;
                }
            }
        }

        if (oldx > newx) {
            int i = oldx - 1;
            for (int k = 0; k < (oldx - newx); ++k) {
                if (board[i][newy].isOccupied == true ||
                    board[oldx][oldy].pieceColor ==  board[i][newy].pieceColor) {
                    return false;
                } else {
                    --i;
                }
            }
        }
    }
    return true;
}

// Check if a move is valid, regardless of piece type
bool is_valid(std::string positions, Cell board[BOARD_SIZE][BOARD_SIZE],
    std::string color, std::ofstream &out) {
    int oldy = positions[0] - 'a';
    int oldx = positions[1] - '0' - 1;
    int newy = positions[2] - 'a';
    int newx = positions[3] - '0' - 1;

    // If the piece tries to move on an already ocupied space on the board
    if(board[oldx][oldy].isOccupied == false)
        return false;

    if (board[newx][newy].isOccupied == true && board[newx][newy].pieceColor == color)
        return false;



    if (newx > 7 || newy > 7 || newx < 0 || newy < 0)
        return false;


    if (board[oldx][oldy].pieceType == KING){
        if(max(abs(newx - oldx), abs(newy - oldy)) != 1)
            return false;
    }

    if (board[oldx][oldy].pieceType == KNIGHT) {
        if(abs((oldx - newx) * (oldy - newy)) != 2)
            return false;

        if(abs((oldx - newx) * (oldy - newy)) == 2)
            if(board[newx][newy].isOccupied == true && board[newx][newy].pieceColor == color)
                return false;
    }

    if (board[oldx][oldy].pieceType == QUEEN) {
        return diagonal_check(oldx, oldy, newx, newy, board) ||
               linear_check(oldx, oldy, newx, newy, board);
    }

    if (board[oldx][oldy].pieceType == PAWN) {
        if (oldy == newy) {
            if ((newx > oldx && color != "white") || board[newx][newy].isOccupied == true){
                return false;
            }
            if ((newx < oldx && color != "black") || board[newx][newy].isOccupied == true){
                return false;
            }
        }
    
        if (color == "white") {
            if (newx - oldx == 1) {
                if (newy != oldy) {
                    if (abs(newy - oldy) != 1) {
                        return false;
                    }
                    if (board[newx][newy].isOccupied == false) {
                        return false;
                    }
                }
            } else if (newx - oldx != 1){
                return false;
            }
        }

        if (color == "black") {
            if (oldx - newx == 1) {
                if (newy != oldy) {
                    if (abs(newy - oldy) != 1) {
                        return false;
                    }
                    if (board[newx][newy].isOccupied == false) {
                        return false;
                    }
                }
            } else if (oldx - newx != 1){
                return false;
            }
        }

    }

    if (board[oldx][oldy].pieceType == ROOK) {
        return linear_check(oldx, oldy, newx, newy, board);
    }

    if (board[oldx][oldy].pieceType == BISHOP) {

        return diagonal_check(oldx, oldy, newx, newy, board);
    }

    return true;
}

// Checks if the format of a move is correct
bool is_move(std::string s) {
    if (s.size() != 4) {
        return false;
    }

    return isalpha(s[0]) && isalpha(s[2]) &&
           isdigit(s[1]) && isdigit(s[3]);
}

// Checks for an overtake
void check_overtake(std::string positions, Cell board[BOARD_SIZE][BOARD_SIZE],
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE], std::ofstream &out) {
    int oldy = positions[0] - 'a';
    int oldx = positions[1] - '0' - 1;
    int newy = positions[2] - 'a';
    int newx = positions[3] - '0' - 1;

    std::string init_pos{positions[0], positions[1]};
    std::string new_pos{positions[2], positions[3]};

    if (board[newx][newy].isOccupied) {
        for (int i = 0; i < 16; ++i) {
            if (whites[i] == new_pos) {
                whites[i] = "";
            } else {
                if (blacks[i] == new_pos) {
                    blacks[i] = "";
                }
            }
        }
    }
}

// Used in MiniMax to undo a move
void move_piece_back(string positions, Cell board[BOARD_SIZE][BOARD_SIZE],
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE]) {
    // Transforming the indexes from standard board positions to the inner board indexes (0-7 x 0-7)
    int oldy = positions[0] - 'a';
    int oldx = positions[1] - '0' - 1;
    int newy = positions[2] - 'a';
    int newx = positions[3] - '0' - 1;

    std::string init_pos{positions[0], positions[1]};
    std::string new_pos{positions[2], positions[3]};

    // Update the position of the piece n the player's "hand" (the vector containing the position
    // of every piece of the player's color)
    bool isWhite = false;
    for (int i = 0; i < COLOR_SIZE; ++i) {
        if (whites[i] == init_pos) {
            whites[i] = new_pos;
            isWhite = true;
            break;
        }
    }

    if (isWhite == false) {
        for (int i = 0; i < COLOR_SIZE; ++i) {
            if (blacks[i] == init_pos) {
                blacks[i] = new_pos;
                break;
            }
        }
    }

    board[oldx][oldy].prev_isOccupied = board[oldx][oldy].isOccupied;
    board[oldx][oldy].prev_pieceColor = board[oldx][oldy].pieceColor;
    board[oldx][oldy].prev_pieceType = board[oldx][oldy].pieceType;

    board[newx][newy].pieceType = board[newx][newy].prev_pieceType;
    board[newx][newy].pieceColor = board[newx][newy].prev_pieceColor;
    board[newx][newy].isOccupied = board[newx][newy].prev_isOccupied;

    board[oldx][oldy].pieceType = 0;
    board[oldx][oldy].pieceColor = "";
    board[oldx][oldy].isOccupied = false;
}

// AUX for debugging
void print_board(Cell board[8][8], std::ofstream &out) {
    for (int i = 7; i >= 0; --i) {
        out << char('A' + i) << "           ";
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].pieceType == 0) {
                out << board[i][j].pieceType << "             ";
            } else {
                out << board[i][j].pieceType << " " << board[i][j].pieceColor << "       ";
            }
        }
        out << "\n";
        out << "\n";
    }
    out << "           ";
    for (int j = 0; j < 8; ++j) {
        out << j << "             ";
    }
    out << endl;
}

// AUX for debugging
void print_hand(std::string pieces[16], std::ofstream &out) {

    for (int i = 0; i < 16; ++i) {
        out << pieces[i] << " ";
    }
    out << "\n";
}

#endif