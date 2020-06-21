/* 
* A cell is a single position on the board, characterised by
* position, whether it is occupied or not, piece type and color.
* Also keeping previous states to allow undoing.
*/
#ifndef CELL_H
#define CELL_H

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class Cell {
	public:
	string position;
	bool isOccupied;
	int pieceType;
	string pieceColor;

	string prev_position;
	bool prev_isOccupied;
	int prev_pieceType;
	string prev_pieceColor;

	Cell(){}

	// When given no piece type, the cell is considered empty
	Cell(string position) {
		this->position = position;
		this->prev_position = position;

		this->pieceType = 0;
		this->prev_pieceType = 0;

		this->isOccupied = false;
		this->prev_isOccupied = false;
	}


	Cell(string position, int pieceType, string pieceColor) {
		this->position = position;
		this->prev_position = position;

		this->isOccupied = true;
		this->prev_isOccupied = true;

		this->pieceType = pieceType;
		this->prev_pieceType = pieceType;

		this->pieceColor = pieceColor;
		this->prev_pieceColor = pieceColor;
	}

};

#endif