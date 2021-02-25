#include "board.h"
#include <stdio.h>
/*
 * solve a 9 by 9 sudoku board
 *
 * @author: Ron Perrella
 */

int main(int argc, char ** argv)
{
    if (argc == 2) {
        board_t *bp = board_load(argv[1]);
        board_print(bp);
        board_t *solution = board_solve(bp);
        printf("Solution is \n");
        board_print(solution);
        board_free(bp);
        board_free(solution);
        // board_print_attempts();
    }else{
        printf("usage: sud sodokufile.txt\n");
    }
}

