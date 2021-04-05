#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pieces.h"


int split_str(char *str, char sep, char **ret1, char **ret2) {
	int first_len = 0, sec_len = 0, sec_start = 0;

	while (str[first_len] != sep) {
		if (str[first_len] == '\0') {
			/* The string ended before the seperator was found. Error.*/
			return 1;
		}
		first_len++;
	}
	sec_start = first_len + 1; /* Skip the seperator. */

	while (str[sec_start + sec_len]) {
		sec_len++;
	}

	/* Copy the strings. */
	*ret1 = malloc(first_len + 1);
	memset(*ret1, 0, first_len + 1);
	*ret2 = malloc(sec_len + 1);
	memset(*ret2, 0, sec_len + 1);

	strncpy(*ret1, str, first_len);
	strncpy(*ret2, str + first_len + 1, sec_len);
	return 0;
};


int parse_size(char *str, char sep, int *ret1, int *ret2) {
	char *s1, *s2;
	if (split_str(str, sep, &s1, &s2)) {
		return 1;
	}

	*ret1 = atoi(s1);
	*ret2 = atoi(s2);

	free(s1);
	free(s2);
	return 0;
};


char *read_line(int fd) {
	if (fd < 0) {
		return NULL;
	}

	char *buf = NULL;
	char c = ' ';
	int len = 0;

	while (1) {
		if (read(fd, &c, 1) != 1) {
			/* Error occured, or EOF. */
			if (buf != NULL) {
				free(buf);
			}
			return NULL;
		}

		buf = realloc(buf, (len + 1) * sizeof(char));

		if ((c == '\0') || (c == '\n')) {
			/* End of the line in either case. */
			buf[len] = '\0';
			break;
		}
		buf[len] = c;
		len++;
	}

	return buf;
};

