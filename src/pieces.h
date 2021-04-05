
#include <stdint.h>


/* The board is represented as an array of shorts (2-byte).
 * The low byte represents the piece, the high byte represents the color.
 * Here are the values used:
 */
#define EMPTY_SQUARE  0
#define PAWN          1
#define BISHOP        2
#define KNIGHT        3
#define ROOK          4
#define QUEEN         5
#define KING          6

#define NO_COLOR      0
#define BLACK         1
#define WHITE         2

/* Some String utilities. */
int split_str(char *str, char sep, char **ret1, char **ret2);
int parse_size(char *str, char sep, int *ret1, int *ret2);
char *read_line(int fd);


/* Things to do with pieces. */
uint16_t get_piece_at(int row, int column);
int set_piece_at(int row, int column, uint16_t val);

int is_move_valid(int srow, int scolumn, int drow, int dcolumn, int ct);
char get_piece_type(uint16_t piece);
char get_piece_color(uint16_t piece);
uint16_t get_full_piece(char piece, char color);
uint16_t get_full_piece(char piece, char color);
int make_move(int srow, int scolumn, int drow, int dcolumn, int color);
int get_turn();
int get_turn_num();

/* Things to do with the board. */
int load_board(char *file_name);
void print_board();
void next_turn();

