/*
 * still leaks memory
 */
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "board.h"

/* statistics for debugging. */
int attempts = 0;
int allocs = 0;
int frees = 0;

/** create a new board */
board_t * board_new(void)
{
    ++allocs;
    return calloc(1, sizeof(board_t));
}

/** free a previously allocated board (or ignore if null) */
void board_free(board_t *p)
{
    frees++;
    if(p) free(p);
}

/** clear all cells in a board to zero */
board_t * board_clear(board_t *b)
{
    return board_set(b, 0);
}

/** compare two boards, returning 1 if equal, 0 if not equal */
int board_isequal(const board_t *dest, const board_t *source)
{
    return (int) !memcmp((void *)dest, (void *)source, sizeof(*dest));
}

/** copy one board into another. both must already be allocated. */
board_t *board_copy(board_t *dest, const board_t *source)
{
    return (board_t *)memcpy((void *)dest, (void *)source, sizeof(*dest));
}

/** allocate a new board and copy source into it. */
board_t *board_dup(const board_t *source)
{
    board_t *target = board_new();

    if (target == NULL) {
        return target;
    }else{
        return board_copy(target, source);
    }
}

/** print a board to stdout. */
void board_print(const board_t *source)
{
    if(source) {
        printf("    ");
        for(int col =0; col < 9; col++) {
            printf("%02d ", col+1);
            if (col == 2 || col == 5) {
                printf(" ");
            }
        }
        printf("\n    ----------------------------\n");
        for(int row =0; row < 9; row++) {
            if (row == 3 || row == 6) {
                printf("\n");
            }
            printf("%02d: ", row + 1);
            for(int col = 0; col < 9; col++) {
                if (col == 3 || col == 6) {
                    printf(" ");
                }
                printf("%2d ", source->cell[row][col]);
            }
            printf("\n");
        }
    } else {
        printf("NO SOLUTION\n");
    }
}
/** Determine if a board as duplicate values in a given column.  Does not count zeros */
int board_has_dup_in_col(const board_t *b, int col)
{
    char already_found[10] =  {0,};
    for(int row = 0; row < 9; row++) {
        int  val = b->cell[row][col];
        if (val == 0) continue;
        if (already_found[val]) return 1;
        already_found[val] = 1;
    }
    return 0;
}
/** Determine if a board as duplicate values in any column.  Does not count zeros */
int board_has_dup_in_any_col(const board_t *b)
{
    for(int col = 0; col < 9; col++) {
        if (board_has_dup_in_col(b, col)) return 1;
    }
    return 0;
}
/** Determine if a board as duplicate values in a given row.  Does not count zeros */
int board_has_dup_in_row(const board_t *b, int row)
{
    char already_found[10] =  {0,};
    for(int col = 0; col < 9; col++) {
        int  val = b->cell[row][col];
        if (val == 0) continue;
        if (already_found[val]) return 1;
        already_found[val] = 1;
    }
    return 0;
}
/** Determine if a board as duplicate values in any row.  Does not count zeros */
int board_has_dup_in_any_row(const board_t *b)
{
    for(int row = 0; row < 9; row++) {
        if (board_has_dup_in_row(b, row)) return 1;
    }
    return 0;
}
/** Determine if a board has zeros in any cell */
int board_has_empties(const board_t *target)
{
  for(int row = 0; row < 9; row ++) {
    for(int col = 0; col < 9; col ++) {
        if (target->cell[row][col] == 0) return 1;
    }
  }  
    return 0;
}

/** a box is a set of 9 cells in which no duplicates are allowed. this checks that. */
int board_has_dup_in_box(const board_t *b, int row, int col)
{
    char already_found[10] =  {0,};
   
    for(int r = row; r < row + 3; r++) {
        for(int c = col; c < col + 3; c++) {
            int val = b->cell[r][c];
            if(val == 0) continue;
            if(already_found[val]) return 1;
            already_found[val] = 1;
        }
    }  
    return 0;
}
/** checks all boxes */
int board_has_dup_in_any_box(const board_t *b)
{
    for(int row = 0; row < 9; row += 3) {
        for(int col = 0; col < 9; col += 3) {
            if (board_has_dup_in_box(b, row, col)) return 1;
        }
    }
    return 0;
}

/** check if a board is fully solved. */
int board_issolved(const board_t *target)
{
  return target && !board_has_empties(target) && !board_has_dup_in_any_row(target) && !board_has_dup_in_any_col(target) && !board_has_dup_in_any_box(target);
}

/** load a board from a file */
board_t *board_load(const char *filepath)
{
    char line [1000];
    board_t *b = board_new();

    FILE *inf = fopen(filepath, "r");
    if (inf == NULL) {
        printf("ERROR: unable to load %s\n", filepath);
        return NULL;
    }

    for (int row = 0; row < 9; row++) {
        line[0] = '\0';

        do {
            if (fgets(line, sizeof(line)-2, inf) == NULL) {
                return NULL;
            }
            // printf("row %d:data %s\n", row, line);
        } while (line[0] == '#');

        char *p = line;
        for (int col = 0; col < 9; col++) {
            int offset = 0;
            int val;
            sscanf(p, "%d%n", &val, &offset);
            p += offset;
            b->cell[row][col] = val;
        }
    }
    fclose(inf);
    return b;
}

/** set every cell in a board */
board_t *board_set(board_t *b, char val)
{
    return memset(b->cell, val, sizeof(b->cell));
}

/**
 * starting at curr_row, curr_col, find the next zero or return 0 if none are left.
 * sets the last two parameters so that you can pass them back in later.
 *
 * returns 1 if able to find unassigned value, 0 if none left 
 */

int get_next_avail_cell(const board_t *b, int curr_row, int curr_col, int *newrow, int *newcol)
{
    for(int r = curr_row; r < 9; r++) {
        for (int c = curr_col; c < 9; c++) {
            if (b->cell[r][c] == 0) {
                *newrow = r;
                *newcol = c;
                return 1;
            }
        }
    }
    return 0;
}

/** produces a list of possible available values */
void mark_used_values(const board_t *b, int row, int col, char *already_found, int arraysize)
{
    memset(already_found, 0, arraysize);
    for(int r = 0; r < 9; r++) {
        int val = b->cell[r][col];
        already_found[val]++;
    }
    for(int c = 0; c < 9; c++) {
        int val = b->cell[row][c];
        already_found[val]++;
    }
    board_mark_used_values(b, row, col, already_found);
}

/**
 * searching the already_found array for an available, unused value.
 *
 * returns 0 if there is no value available.
 */

int first_available(char already_found[])
{
    for(int i = 1; i< 10; i++) {
        if(already_found[i] == 0) return already_found[i];
    }
    return 0;
}

/**
 * marks all the used values in a given row-col
 *
 *@bug  looks wrong to me. seems to be looking at a box but doesn't finish the job. is this right?
 */
void board_mark_used_values(const board_t *b, int row, int col, char already_found[])
{
    for(int r = 0; r < 3; r++) 
        for (int c = 0; c < 3; c++) {
            int val = b->cell[ (row/3)*3 + r][(col/3)*3 + c];
            already_found[val]++;
        }
}

/**
 * @returns null if unable to solve, else returns a board
 */

void board_print_attempts()
{
    printf("attempts: %d\n", attempts);
    printf("allocs: %d\n", allocs);
    printf("frees: %d\n", frees);
}

#define FOUND_CANDIDATE(val) (already_found[val]==0)

/** solve a board, return a new solved board or null if cannot. */
board_t *board_solve(board_t *b)
{
    int row = 0, col = 0;
    int ret = 0;
    ret = get_next_avail_cell(b, row, col, &row, &col);
    if(ret) {
        char already_found[10]; // indexed by value 1..9, 0 means available.

        mark_used_values(b, row, col, already_found, sizeof(already_found));
        for (int val = 1; val<10; val++) { // zero is never a candidate
            if(FOUND_CANDIDATE(val)) {
                ++attempts;
                board_t *candidate = board_dup(b);
                candidate->cell[row][col] = val;
                if (board_issolved(candidate)) return candidate;
                else {
                    board_t *solution =  board_solve(candidate);
                    board_free(candidate);
                    if (solution) return solution;
                    else {
                        continue;
                    }
                }
            }
        }
        return NULL; 
    }else{
        if (board_issolved(b)) return b;
        else return NULL;
    }
}

