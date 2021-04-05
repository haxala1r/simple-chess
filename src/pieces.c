#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pieces.h"

uint16_t get_full_piece(char piece, char color) {
	uint16_t p = 0;
	uint16_t c = 0;
	switch (piece) {
	case 'P':
		p = PAWN;
		break;
	case 'B':
		p = BISHOP;
		break;
	case 'N':
		p = KNIGHT;
		break;
	case 'R':
		p = ROOK;
		break;
	case 'Q':
		p = QUEEN;
		break;
	case 'K':
		p = KING;
		break;
	default:
		p = EMPTY_SQUARE;
		break;
	}

	switch (color) {
	case 'W':
		c = WHITE;
		break;
	case 'B':
		c = BLACK;
		break;
	default:
		c = NO_COLOR;
		break;
	};
	p = p | (c << 8);

	return p;
};

int diff(int a, int b) {
	if (a > b) {
		return a - b;
	}
	return b - a;
}

int min(int a, int b) {
	return ((a < b) ? a : b);
}


int is_move_valid(int srow, int scolumn, int drow, int dcolumn, int ct) {
	if ((srow == drow) && (scolumn == dcolumn)) {
		return 1;
	}

	uint16_t full_piece = get_piece_at(srow, scolumn);
	uint16_t piece = full_piece & 0xFF;
	uint16_t color = full_piece >> 8;

	if (ct != color) {
		return 1;
	}
	if ((get_piece_at(drow, dcolumn) >> 8) == color) {
		return 1;
	}

	switch (piece) {
	case PAWN:
		if (scolumn == dcolumn) {
			/* Regular, one-space or two-space move forward. */
			if ((color == WHITE) && (((drow - srow) == 1) || ((drow - srow) == 2))) {
				return 0;
			}
			if ((color == BLACK) && (((srow - drow) == 1) || ((srow - drow) == 2))) {
				return 0;
			}
		}
		/* Pawn capture. */
		if (diff(scolumn, dcolumn) == 1) {
			if (get_piece_at(drow, dcolumn) == 0) {
				break;
			}
			if ((color == WHITE) && ((drow - srow) == 1)) {
				return 0;
			}
			if ((color == BLACK) && ((srow - drow) == 1)) {
				return 0;
			}
		}
		break;
	case BISHOP:
		if (diff(srow, drow) == diff(scolumn, dcolumn)) {
			for (int i = 1; i < diff(drow, srow); i++) {
				if (get_piece_at(min(srow, drow) + i, min(scolumn, dcolumn) + i) != 0) {
					return 1;
				}
			}
			return 0;
		}
		break;
	case KNIGHT:
		if ((diff(srow, drow) == 2) && (diff(scolumn, dcolumn) == 1)) {
			return 0;
		}
		if ((diff(srow, drow) == 1) && (diff(scolumn, dcolumn) == 2)) {
			return 0;
		}
		break;
	case ROOK:
		if (srow == drow) {
			for (int i = 1; i < diff(scolumn, srow); i++) {
				if (get_piece_at(srow, min(scolumn, dcolumn) + i) != 0) {
					return 1;
				}
			}
			return 0;
		}
		if (scolumn == dcolumn) {
			for (int i = 1; i < diff(scolumn, srow); i++) {
				if (get_piece_at(min(srow, drow) + i, scolumn) != 0) {
					return 1;
				}
			}
			return 0;
		}
		break;
	case KING:
		if ((diff(srow, drow) <= 1) && (diff(scolumn, dcolumn) <= 1)) {
			return 0;
		}
		break;
	case QUEEN:

	default:
		break;
	};


	return 1;
};



int make_move(int srow, int scolumn, int drow, int dcolumn, int color) {
	if (is_move_valid(srow, scolumn, drow, dcolumn, color)) {
		return 1;
	}
	set_piece_at(drow, dcolumn, get_piece_at(srow, scolumn));
	set_piece_at(srow, scolumn, 0);
	return 0;
};

