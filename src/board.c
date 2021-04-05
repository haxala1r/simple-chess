#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pieces.h"

uint16_t *board;

int board_width;
int board_height;

/* The turn number we're at. */
int turn_num;

/* Determines who's turn it is. */
int turn;


int get_turn() {
	return turn;
}

int get_turn_num() {
	return turn_num;
}

void next_turn() {
	turn_num++;
	if (turn == WHITE) {
		turn = BLACK;
	} else {
		turn = WHITE;
	}
}

uint16_t get_piece_at(int row, int column) {
	/* TODO: Add a check here to ensure the coordinates are valid. */
	return board[row * board_width + column];
};

int set_piece_at(int row, int column, uint16_t val) {
	/* TODO: Add a check here to ensure the coordinates are valid. */
	board[row * board_width + column] = val;
	return 0;
};


char get_piece_color(uint16_t piece) {
	switch ((piece >> 8) & 0xFF) {
	case WHITE:
		return 'W';
	case BLACK:
		return 'B';
	default:
		return ' ';
	}
}

char get_piece_type(uint16_t piece) {
	switch (piece & 0xFF) {
	case PAWN:
		return 'P';
	case BISHOP:
		return 'B';
	case KNIGHT:
		return 'N';
	case ROOK:
		return 'R';
	case QUEEN:
		return 'Q';
	case KING:
		return 'K';
	default:
		return ' ';
	}
}


int load_board(char *file_name) {
	/* This function loads the state of the board from a file.
	 * The file uses a custom format, consult format.txt for info.
	 */

	int fd = open(file_name, O_RDONLY);
	if (fd < 0) {
		return -1;
	}
	uint16_t piece = 0;
	char *line;

	/* Starting color. */
	line = read_line(fd);
	if (line[0] == 'W') {
		turn = WHITE;
	} else if (line[0] == 'B') {
		turn = BLACK;
	}

	/* Board size. */
	line = read_line(fd);
	parse_size(line, 'x', &board_height, &board_width);
	board = malloc(board_height * board_width * sizeof(uint16_t));
	free(line);

	/* Now load the pieces. */
	while ((line = read_line(fd)) != NULL) {
		if (6 > strlen(line)) {
			free(line);
			continue;
		}
		piece = get_full_piece(line[0], line[1]);

		/* Get the position of the piece. */
		char *pos = line + 3;
		int row, column;
		parse_size(pos, '-', &row, &column);
		if ((row >= board_height) || (column >= board_width)) {
			free(line);
			close(fd);
			return -1;
		}

		/* Put the piece. */
		board[column + row * board_width] = piece;
		free(line);
	}


	close(fd);
	return 0;
}

void print_board() {
	for (int i = 0; i < board_height * 2; i++) {
		printf("\n");
	}
	printf("   ");
	for (int i = 0; i < board_width; i++) {
		printf("%1d    ", i);
	}
	printf("\n");
	for (int i = 0; i < board_height; i++) {
		printf("%1d  ", i);
		for (int j = 0; j < board_width; j++) {
			uint16_t piece = board[i * board_width + j];
			printf("[%c%c] ", get_piece_type(piece), get_piece_color(piece));
		}
		printf("\n\n");
	}
};




