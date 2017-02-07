#include "score.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 15

/**

For å kompilere testen, bruk: 
  gcc score_test.c score.o -o score_test -Wall -Wextra -O2

Kjøres med: ./score_test

 **/

int main(int argc, char *argv[], char *env[]) {
	size_t failed = 0;
	void *board = board_init(5, 22);
	if (!board) {
		failed++;
		fprintf(stdout, "[ FAILED] Could not do board_init!\n");
		return -1;
	}
	uint32_t score;
	char name_buf[NAME_SIZE];
	if (!board_get_pos(board, 1, &score, name_buf, sizeof(name_buf))) {
		failed++;
		fprintf(stdout, "[WARNING] No entries in board and "\
			"board_get_pos did not return error!\n");
	}
	if (1 != board_add_score(board, 240, "paalh")) {
		failed++;
		fprintf(stdout, "[WARNING] First entry did not get "\
			"first place!\n");
	}
	if (2 != board_add_score(board, 128, "asgeirom")) {
		failed++;
		fprintf(stdout, "[WARNING] Failed second place test!\n");
	}
	if (!board_get_pos(board, 3, &score, name_buf, sizeof(name_buf))) {
		failed++;
		fprintf(stdout, "[WARNING] Asking for a position not "\
			"yet filled did not give error!\n");
	}
	if (board_get_pos(board, 2, &score, name_buf, sizeof(name_buf))) {
		failed++;
		fprintf(stdout, "[WARNING] Asking for pos 2 gave "\
			"error when it should not have!\n");
	} else {
		if (score != 128 || strncmp("asgeirom", name_buf, 7)) {
			failed++;
			fprintf(stdout, "[WARNING] Invalid name/score "\
				"returned %"PRIu32":'%s'!\n", score, name_buf);
		}
	}
	if (2 != board_add_score(board, 196, "janabr")) {
		failed++;
		fprintf(stdout, "[WARNING] Failed the second second "\
			"place test (janabr)!\n");
	}
	if (1 != board_add_score(board, 511, "paalh")) {
		failed++;
		fprintf(stdout, "[WARNING] Failed the second first "\
			"place test (paalh)!\n");
	}
	if (5 != board_add_score(board, 8, "asgeirom")) {
		failed++;
		fprintf(stdout, "[WARNING] Failed the last place test!\n");
	}
	if (0 != board_add_score(board, 1, "not on list!")) {
		failed++;
		fprintf(stdout, "[WARNING] Player did not get '0' (not "\
			"added to highscore list!\n");
	}	
	char *test_name[] = {"paalh", "paalh", "janabr",\
			 "asgeirom", "asgeirom"};
	size_t cnt;
	
	for (cnt = 1; cnt <= (sizeof(test_name) / sizeof(*test_name)); cnt++) {
		if(board_get_pos(board, cnt, &score, name_buf, sizeof(name_buf))) {
			fprintf(stdout, "[WARNING] Score %zu error:\n", cnt);
			continue;
		}
		fprintf(stdout, "%03zu: %s (%04"PRIu32")\n", cnt,\
			name_buf, score);
		if (strncmp(name_buf, test_name[cnt - 1], sizeof(name_buf))) {
			failed++;
			fprintf(stdout, "[WARNING] Name missmatch above!\n");
		}
	}
	if (!board_get_pos(board, cnt, &score, name_buf, sizeof(name_buf))) {
		failed++;
		fprintf(stdout, "[WARING] Trying to get out of bound score "\
			"did not give error!\n");
	}
	if ((board = board_destroy(board))) {
		failed++;
		fprintf(stdout, "[ FAILED] Could not free/"\
			"destroy score board\n");
	}
	if (-1 != board_add_score(NULL, 1, "asgeirom")) {
		failed++;
		fprintf(stdout, "[ FAILED] Did not give error when "\
			"giving add_score NULL pointer!\n");
	}
	if (-1 != board_get_pos(NULL, 1, &score, name_buf, sizeof(name_buf))) {
		failed++;
		fprintf(stdout, "[ FAILED] Did not give error when "\
			"giving get_pos NULL pointer!\n");
	}

	if (!failed) {
		fprintf(stdout, "[     OK] No tests failed!\n");
		return 0;
	} else {
		fprintf(stdout, "[ FAILED] %zu tests failed!\n", failed);
		return -1;
	}
}
