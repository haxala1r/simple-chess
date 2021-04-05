#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pieces.h"






int main(void) {

	if (load_board("board.dat")) {
		printf("Save data could not be found. ");
		return 1;
	}

	while (1) {
		/* Print the prompt and the board. */
		print_board();

		printf("Turn number %d \n", get_turn_num());
		if (get_turn() == WHITE) {
			printf("White's turn.\n");
		} else {
			printf("Black's turn. \n");
		}

		/* Take input from the player. */
		char *l = read_line(0);
		if (!strcmp(l, "exit")) {
			break;
		}

		char *first_coord, *sec_coord;
		if (split_str(l, ' ', &first_coord, &sec_coord)) {
			printf("Invalid move.");
			free(l);
			continue;
		}

		/* Parse the coordinates. */
		int srow, scolumn, drow, dcolumn;
		if (parse_size(first_coord, 'x', &srow, &scolumn)) {
			printf("Invalid move.");
			free(l);
			free(first_coord);
			free(sec_coord);
			continue;
		}
		if (parse_size(sec_coord, 'x', &drow, &dcolumn)) {
			printf("Invalid move.");
			free(l);
			free(first_coord);
			free(sec_coord);
			continue;
		}

		/* Make the move. */
		if (make_move(srow, scolumn, drow, dcolumn, get_turn())) {
			printf("Invalid move.");
			free(first_coord);
			free(sec_coord);
			free(l);
			continue;
		}

		free(first_coord);
		free(sec_coord);
		free(l);

		/* Next turn. */
		next_turn();
	}

	printf("\nThe game ends. \n");
	return 0;
}
