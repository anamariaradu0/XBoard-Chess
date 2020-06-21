/*
* Auxiliary functions pertaining to the MiniMax algorithm, such as listing all possible
* moves for a given color, checking if the king is in danger, computing scores for
* both the engine's and the enemy's possible moves, picking the best next move
* accordingly.
*/
#ifndef ALGO_UTILS_H
#define ALGO_UTILS_H

#include <stdio.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include "Cell.h"
#include "Board_Utils.h"
#include "Eval_Board.h"

#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6
#define BOARD_SIZE 8
#define COLOR_SIZE 16

using namespace std;

Cell** MaxMove(Cell **board, short int depth_limit, short int depth, std::string color,
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE], std::ofstream &out);
Cell** MinMove(Cell **board, short int depth_limit, short int depth, std::string color,
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE], std::ofstream &out);

/*
* Lists all possible moves for one of the players.
* Note that this function also lists moves that can put the king in danger, and it also
* doesn't take into account prioritizing moving or protecting the king in case of danger.
*/
vector<std::string> list_all_moves(std::string pieces[COLOR_SIZE],
    Cell board[BOARD_SIZE][BOARD_SIZE], std::string color, std::ofstream &out) {
    vector <std::string> possible_moves;

    for (int k = 0; k < COLOR_SIZE; ++k) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (pieces[k] != "") {
                    std::string verify = pieces[k] + board[i][j].position;
                    if (is_valid(verify, board, color, out)) {
                        possible_moves.push_back(verify);
                    }
                }
            }
        }
    }
    return possible_moves;
}

/*
* Function that checks if the enemy has any possible moves that can put our engine's
* king in danger. At this stage, this means checking for a /check/ situation.
*/
std::string check_king(std::string my_pieces[COLOR_SIZE], std::string enemy_pieces[COLOR_SIZE],
    Cell board[8][8], std::string color, std::ofstream &out) {
    std::string king_move = NULL;
    vector<std::string> possible_enemy_moves = list_all_moves(enemy_pieces, board, color, out);

    for (int i = 0; i < possible_enemy_moves.size(); ++i) {
        if (possible_enemy_moves[i].substr(2, 2) == my_pieces[14]) {
            vector<std::string> my_possible_moves = list_all_moves(my_pieces, board, color, out);

            for (int j = 0; j < my_possible_moves.size(); ++j) {
                if (my_possible_moves[j].substr(0,2) == my_pieces[14]) {
                    king_move = my_possible_moves[j];
                    break;
                }
            }

            break;
        }
    }

    std::string move = king_move + "\n";
    return move;
}

/*
* Function that picks an enemy's best possible move by assigning scores to
* each cell on the board.
*/
Cell** MinMove(Cell **board, short int depth_limit, short int depth, std::string color,
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE], std::ofstream &out) {
    vector<std::string> moves;

    int compare = -1000;

    Cell ** best_move = new Cell * [BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        best_move[i] = new Cell[BOARD_SIZE];
    }

    Cell ** move = new Cell * [BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        move[i] = new Cell[BOARD_SIZE];
    }

    Cell ** best_real_move = new Cell * [BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        best_real_move[i] = new Cell[BOARD_SIZE];
    }


    if (depth >= depth_limit) {
        return board;
    } else {
        Cell matrix[BOARD_SIZE][BOARD_SIZE];

        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                matrix[i][j].position = board[i][j].position;
                matrix[i][j].pieceType = board[i][j].pieceType;
                matrix[i][j].pieceColor = board[i][j].pieceColor;
                matrix[i][j].isOccupied = board[i][j].isOccupied;
            }
        }

        if (color == "white") {
            moves = list_all_moves(whites, matrix, "black", out);
        } else {
            moves = list_all_moves(blacks, matrix, "white", out);
        }

        for (int i = 0; i < moves.size(); ++i) {
            move_piece(moves[i], matrix, whites, blacks);

            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    board[i][j].position = matrix[i][j].position;
                    board[i][j].pieceType = matrix[i][j].pieceType;
                    board[i][j].pieceColor = matrix[i][j].pieceColor;
                    board[i][j].isOccupied = matrix[i][j].isOccupied;

                }
            }

            if (color == "white") {
                move = MaxMove(board, depth_limit, depth + 1, "black", whites, blacks, out);
            } else {
                move = MaxMove(board, depth_limit, depth + 1, "white", whites, blacks, out);
            }

            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    board[i][j].position = matrix[i][j].position;
                    board[i][j].pieceType = matrix[i][j].pieceType;
                    board[i][j].pieceColor = matrix[i][j].pieceColor;
                    board[i][j].isOccupied = matrix[i][j].isOccupied;

                }
            }

            std::string newMove = moves[i].substr(2, 2) + moves[i].substr(0, 2);
            move_piece_back(newMove, matrix, whites, blacks);

            if (best_move == NULL ||
                eval_board(move, color) <= eval_board(best_move, color) ||
                eval_board(move, color) > compare) {
                compare = eval_board(move, color);
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        best_move[i][j].position = move[i][j].position;
                        best_move[i][j].pieceType = move[i][j].pieceType;
                        best_move[i][j].pieceColor = move[i][j].pieceColor;
                        best_move[i][j].isOccupied = move[i][j].isOccupied;

                    }
                }

                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        best_real_move[i][j].position = board[i][j].position;
                        best_real_move[i][j].pieceType = board[i][j].pieceType;
                        best_real_move[i][j].pieceColor = board[i][j].pieceColor;
                        best_real_move[i][j].isOccupied = board[i][j].isOccupied;
                    }

                }
            }
        }
        return best_real_move;
    }
}

/*
* Function that picks engine's best possible move by assigning scores to
* each cell on the board and taking into account enemy's best possible moves.
*/
Cell** MaxMove(Cell **board, short int depth_limit, short int depth, std::string color,
    std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE], std::ofstream &out) {
    vector<std::string> moves;
    int compare = -1000;

    Cell** best_move = new Cell*[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        best_move[i] = new Cell[BOARD_SIZE];
    }

    Cell** move = new Cell*[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        move[i] = new Cell[BOARD_SIZE];
    }

    Cell** best_real_move = new Cell*[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        best_real_move[i] = new Cell[BOARD_SIZE];
    }

    if (depth >= depth_limit) {
        return board;

    } else {
        Cell matrix[BOARD_SIZE][BOARD_SIZE];

        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                matrix[i][j].position = board[i][j].position;
                matrix[i][j].pieceType = board[i][j].pieceType;
                matrix[i][j].pieceColor = board[i][j].pieceColor;
                matrix[i][j].isOccupied = board[i][j].isOccupied;
            }
        }

        if (color == "white") {
            moves = list_all_moves(whites, matrix, "white", out);
        } else {
            moves = list_all_moves(blacks, matrix, "black", out);
        }
        
        for (int i = 0; i < moves.size(); ++i) {
            move_piece(moves[i], matrix, whites, blacks);

            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    board[i][j].position = matrix[i][j].position;
                    board[i][j].pieceType = matrix[i][j].pieceType;
                    board[i][j].pieceColor = matrix[i][j].pieceColor;
                    board[i][j].isOccupied = matrix[i][j].isOccupied;

                }
            }

            if (color == "white") {
                move = MinMove(board, depth_limit, depth + 1, "black", whites, blacks, out);
            } else {
                move = MinMove(board, depth_limit, depth + 1, "white", whites, blacks, out);
            }
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    board[i][j].position = matrix[i][j].position;
                    board[i][j].pieceType = matrix[i][j].pieceType;
                    board[i][j].pieceColor = matrix[i][j].pieceColor;
                    board[i][j].isOccupied = matrix[i][j].isOccupied;

                }
            }

            std::string newMove = moves[i].substr(2, 2) + moves[i].substr(0, 2);
            move_piece_back(newMove, matrix, whites, blacks);

            if (best_move == NULL ||
                eval_board(move, color) >= eval_board(best_move, color) ||
                eval_board(move, color) > compare) {
                compare = eval_board(move, color);
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        best_move[i][j].position = move[i][j].position;
                        best_move[i][j].pieceType = move[i][j].pieceType;
                        best_move[i][j].pieceColor = move[i][j].pieceColor;
                        best_move[i][j].isOccupied = move[i][j].isOccupied;
                    }
                }

                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        best_real_move[i][j].position = board[i][j].position;
                        best_real_move[i][j].pieceType = board[i][j].pieceType;
                        best_real_move[i][j].pieceColor = board[i][j].pieceColor;
                        best_real_move[i][j].isOccupied = board[i][j].isOccupied;
                    }
                }
            }
        }

        return best_real_move;
    }
}

/*
* MiniMax implemented on possible boards, starting from the current one.
* It assigns scores to moves by assigning a particular score to each
* board cell, based on position, piece typ and color.
*/
std::string MinMax(Cell board[BOARD_SIZE][BOARD_SIZE], short int depth_limit,
    std::string color, std::string whites[COLOR_SIZE], std::string blacks[COLOR_SIZE], std::ofstream &out) {
    
    std::string move = "";

    Cell **newBoard = new Cell*[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        newBoard[i] = new Cell[BOARD_SIZE];
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            newBoard[i][j].position = board[i][j].position;
            newBoard[i][j].pieceType = board[i][j].pieceType;
            newBoard[i][j].pieceColor = board[i][j].pieceColor;
            newBoard[i][j].isOccupied = board[i][j].isOccupied;
        }
    }

    newBoard = MaxMove(newBoard, depth_limit, 0, color, whites, blacks, out);

    // Determining the best move by comparing the current board to the best board
    std::string start;
    std::string end;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].isOccupied && (!newBoard[i][j].isOccupied)
                && board[i][j].pieceColor == color) {
                start = board[i][j].position;
            }
            if (newBoard[i][j].isOccupied && (!board[i][j].isOccupied)
                && newBoard[i][j].pieceColor == color) {
                end = board[i][j].position;
            }
        }
    }

    move = start + end + "\n";
    return move;
}

#endif
