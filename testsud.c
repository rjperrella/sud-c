#include <stdio.h>
#include "board.h"
#include "assert.h"

int passed = 0;

/* each individual test below */

void test_all_empty(void) 
{
    board_t *b = board_new();
    board_t empty;
    board_clear(&empty); 

    assert(board_isequal(b, &empty));
    ++passed;
}
void test_duplicate_board(void) 
{
    board_t *perfect = board_load("perfect.txt");
    board_t *dup = board_dup(perfect);
    
    assert(board_isequal(perfect, dup));
    ++passed;
}

void test_empty_board_is_not_solved(void) 
{
    board_t *b = board_new();
    
    assert(!board_issolved(b));
    ++passed;
}

void test_duplicate_in_row_is_not_solved(void) 
{
    board_t *b = board_new();

    b->cell[0][0] = 1;
    b->cell[0][1] = 1;
    
    assert(!board_issolved(b));
    ++passed;
}

void test_zeros_not_duplicate_in_row(void) 
{
    board_t *b = board_new();

    assert(!board_has_dup_in_row(b, 0));
    ++passed;
}

void test_find_duplicate_in_row(void) 
{
    board_t *b = board_new();

    b->cell[0][0] = 1;
    b->cell[0][1] = 1;
    
    assert(board_has_dup_in_row(b, 0));
    ++passed;
}

void test_find_duplicate_in_col(void) 
{
    board_t *b = board_new();

    b->cell[0][0] = 1;
    b->cell[1][0] = 1;
    
    assert(board_has_dup_in_col(b, 0));
    ++passed;
}

void test_find_duplicate_in_2nd_row(void) 
{
    board_t *b = board_new();

    b->cell[1][0] = 1;
    b->cell[1][1] = 1;
    
    assert(board_has_dup_in_row(b, 1));
    ++passed;
}

void test_find_duplicate_in_any_row(void) 
{
    board_t *b = board_new();

    b->cell[2][0] = 1;
    b->cell[2][8] = 1;
    
    assert(board_has_dup_in_any_row(b));
    ++passed;
}

void test_find_duplicate_in_any_col(void) 
{
    board_t *b = board_new();

    b->cell[7][1] = 9;
    b->cell[8][1] = 9;
    
    assert(board_has_dup_in_any_col(b));
    ++passed;
}

void test_find_duplicate_in_box(void) 
{
    board_t *b = board_new();

    b->cell[0][0] = 9;
    b->cell[0][1] = 9;
    
    assert(board_has_dup_in_box(b, 0, 0));
    ++passed;
}

void test_find_duplicate_in_box_diagonal(void) 
{
    board_t *b = board_new();

    b->cell[0][0] = 9;
    b->cell[1][1] = 9;
    
    assert(board_has_dup_in_box(b, 0, 0));
    ++passed;
}

void test_find_duplicate_in_box_corners(void) 
{
    board_t *b = board_new();

    b->cell[6][6] = 9;
    b->cell[8][8] = 9;
    
    assert(board_has_dup_in_box(b, 6, 6));
    ++passed;
}

void test_find_duplicate_in_any_box(void) 
{
    board_t *b = board_new();

    b->cell[6][6] = 9;
    b->cell[8][8] = 9;
    
    assert(board_has_dup_in_any_box(b));
    ++passed;
}

void test_load_a_simple_puzzle(void) 
{
    board_t *initial = board_load("allones.txt");
    
    board_t *allones = board_new();
    board_set(allones, 1);
    assert(board_isequal(initial, allones));
    board_free(initial);
    board_free(allones);
    ++passed;
}

void test_load_a_puzzle(void) 
{
    board_t *initial = board_load("testpattern.txt");
    
    board_t *b = board_new();
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            b->cell[row][col] = col + 1; 
        }

    }
    assert(board_isequal(initial, b));
    board_free(initial);
    board_free(b);

    ++passed;
}

void test_get_next_avail_cell_first() 
{
    int row = 0, col = 0;
    board_t *b = board_new();
    b->cell[row][col++] = 0;
    b->cell[row][col++] = 1;
    b->cell[row][col++] = 2;
    row = col = 0;

    int ret = get_next_avail_cell(b, row, col, &row, &col);
    assert(row == 0 && col == 0);
    board_free(b);
}
void test_get_next_avail_cell_middle() 
{
    int row = 0, col = 0;
    board_t *b = board_new();
    b->cell[row][col++] = 1;
    b->cell[row][col++] = 2;
    b->cell[row][col++] = 0;
    b->cell[row][col++] = 3;
    row = col = 0;
    int ret = get_next_avail_cell(b, row, col, &row, &col);
    assert(row == 0 && col == 2);
    board_free(b);
}
void test_get_next_avail_cell_endline() 
{
    int row = 4, col = 8;
    board_t *b = board_new();
    board_set(b, 1);
    b->cell[row][col] = 0;
    row = col = 0;
    int ret = get_next_avail_cell(b, row, col, &row, &col);
    assert(row == 4 && col == 8);
    board_free(b);
}

void test_get_next_avail_cell_end() 
{
    board_t *b = board_new();
    board_set(b, 1);
    int row = 8, col = 8;
    b->cell[row][col] = 0;
    row = col = 0;
    int ret = get_next_avail_cell(b, row, col, &row, &col);
    assert(row == 8 && col == 8);
    board_free(b);
}
    
void test_check_a_perfect(void)
{
    board_t *perfect = board_load("perfect.txt");
    
    assert(perfect);
    assert(!board_has_empties(perfect));
    assert(!board_has_dup_in_any_row(perfect)); 
    assert(!board_has_dup_in_any_col(perfect));
    assert(!board_has_dup_in_any_box(perfect));

    assert(board_issolved(perfect));
    board_free(perfect);
    ++passed;
}

void test_solve_a_single_piece_puzzle(void) 
{
    board_t *initial = board_load("onepiecemissing.txt");
    
    board_t *solution = board_solve(initial);

    assert(board_issolved(solution));
    board_free(solution);
    board_free(initial);
    ++passed;
}
void test_solve_a_two_piece_puzzle(void) 
{
    board_t *initial = board_load("twopiecemissing.txt");
    
    board_t *solution = board_solve(initial);

    assert(board_issolved(solution));

    board_free(solution);
    board_free(initial);
    ++passed;
}
void test_solve_a_three_piece_puzzle(void) 
{
    board_t *initial = board_load("threepiecemissing.txt");
    
    board_t *solution = board_solve(initial);

    assert(board_issolved(solution));

    board_free(solution);
    board_free(initial);
    ++passed;
}
void test_solve_a_two_piece_separate_puzzle(void) 
{
    board_t *initial = board_load("perfect.txt");

    initial->cell[0][0] = 0;
    initial->cell[8][8] = 0;
    
    board_t *solution = board_solve(initial);

    assert(board_issolved(solution));

    board_free(solution);
    board_free(initial);
    ++passed;
}

void test_solve_a_two_empty_rows_puzzle2(void) 
{
    board_t *initial = board_load("perfect.txt");

    initial->cell[1][0] = 0;
    initial->cell[1][1] = 0;
    initial->cell[1][2] = 0;
    initial->cell[1][3] = 0;
    initial->cell[1][4] = 0;
    initial->cell[1][5] = 0;
    initial->cell[1][7] = 0;
    initial->cell[1][8] = 0;
    
    initial->cell[2][0] = 0;
    initial->cell[2][1] = 0;
    initial->cell[2][2] = 0;
    initial->cell[2][3] = 0;
    initial->cell[2][4] = 0;
    initial->cell[2][5] = 0;
    initial->cell[2][7] = 0;
    initial->cell[2][8] = 0;

    board_t *solution = board_solve(initial);

    assert(board_issolved(solution));

    board_free(solution);
    board_free(initial);
    ++passed;
}
void test_solve_a_two_empty_rows_puzzle(void) 
{
    board_t *initial = board_load("perfect.txt");

    initial->cell[1][0] = 0;
    initial->cell[1][1] = 0;
    initial->cell[1][2] = 0;
    initial->cell[1][3] = 0;
    initial->cell[1][4] = 0;
    initial->cell[1][5] = 0;
    initial->cell[1][7] = 0;
    initial->cell[1][8] = 0;
    
    initial->cell[3][0] = 0;
    initial->cell[3][1] = 0;
    initial->cell[3][2] = 0;
    initial->cell[3][3] = 0;
    initial->cell[3][4] = 0;
    initial->cell[3][5] = 0;
    initial->cell[3][7] = 0;
    initial->cell[3][8] = 0;

    board_t *solution = board_solve(initial);

    assert(board_issolved(solution));

    board_free(solution);
    board_free(initial);
    ++passed;
}

void test_solve_a_puzzle(void) 
{
    board_t *initial = board_load("example.txt");
    
    board_t *solution = board_solve(initial);

    board_print_attempts();

    assert(board_issolved(solution));
    board_free(solution);
    board_free(initial);
    ++passed;
}

/*
 * unit tests for sudoku
 */

int main(int argc, char **argv)
{
    test_all_empty();
    test_duplicate_board();
    test_empty_board_is_not_solved();
    test_duplicate_in_row_is_not_solved();
    test_zeros_not_duplicate_in_row();
    test_find_duplicate_in_row();
    test_find_duplicate_in_2nd_row();
    test_find_duplicate_in_any_row();
    test_find_duplicate_in_col();
    test_find_duplicate_in_any_col();
    test_find_duplicate_in_box();
    test_find_duplicate_in_box_diagonal();
    test_find_duplicate_in_any_box();
    test_find_duplicate_in_box_corners();
    test_load_a_simple_puzzle();
    test_load_a_puzzle();
    test_get_next_avail_cell_first();
    test_get_next_avail_cell_middle();
    test_get_next_avail_cell_endline();
    test_get_next_avail_cell_end();
    test_check_a_perfect();
    test_solve_a_single_piece_puzzle() ;
    test_solve_a_two_piece_puzzle() ;
    test_solve_a_three_piece_puzzle() ;
    test_solve_a_two_piece_separate_puzzle();
    test_solve_a_two_empty_rows_puzzle();
    test_solve_a_two_empty_rows_puzzle2();
    test_solve_a_puzzle();

    printf("%d tests passed\n", passed);

    return 0;
}

