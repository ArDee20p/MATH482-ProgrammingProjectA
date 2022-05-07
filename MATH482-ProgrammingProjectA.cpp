//COMP482 - Programming Project Part A - Pizza Slice Instant Insanity - Spring 2022

#include <iostream>
#include <string>
#define NUM_ROWS 14
#define NUM_COLORS 14
#define NUM_COLUMNS 3
#define NUM_PUZZLES 6

using namespace std;

class Puzzle {
public:
    int slices[NUM_ROWS][NUM_COLUMNS];
    int occurences[NUM_COLORS];
    int timesRotated[NUM_ROWS];
    bool success;
    string message;
    
    int* rotateSlice(int* slice) {
        int tmp;

        tmp = slice[0];
        slice[0] = slice[1];
        slice[1] = slice[2];
        slice[2] = tmp;

        return slice;
    }

    int* flipSlice(int* slice, int a, int b) {
        int tmp;
        
        tmp = slice[a];
        slice[a] = slice[b];
        slice[b] = tmp;

        return slice;
    }
};

Puzzle puzzles[NUM_PUZZLES] = {
        Puzzle({
        { 12,     9,      7,
        4,      2,      13,
        11,     8,      6,
        3,      1,      12,
        10,     7,      5,
        2,      14,     11,
        9,      6,      4,
        1,      13,     10,
        7,      5,      2,
        14,     11,     9,
        6,      4,      1,
        13,     10,     8,
        5,      3,      14,
        12,     8,      3, },
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        true,
        " "
    }),
        Puzzle({
        { 9,      4,      13,
        8,      3,      12,
        7,      2,      11,
        6,      1,      10,
        5,      14,     9,
        4,      13,     8,
        2,      11,     6,
        1,      10,     5,
        14,     9,      4,
        13,     8,      3,
        12,     7,      2,
        11,     6,      1,
        3,      12,     7,
        10,     5,      14 },
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        true,
        " "
    }),
        Puzzle({
        { 11,     7,      4,
        14,     10,     7,
        3,      13,     10,
        6,      2,      13,
        9,      5,      2,
        12,     8,      5,
        1,      11,     8,
        4,      14,     11,
        7,      3,      14,
        10,     6,      3,
        13,     9,      6,
        2,      12,     9,
        5,      12,     8,
        1,      4,      1 },
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        true,
        " "
    }),
        Puzzle({
        { 3,      5,      8,
        10,     13,     1,
        3,      6,      8,
        11,     13,     2,
        4,      6,      9,
        11,     14,     2,
        5,      7,      9,
        12,     14,     3,
        5,      8,      10,
        12,     1,      6,
        11,     13,     1,
        4,      9,      14,
        2,      4,      7,
        12,     7,      10 },
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        true,
        " "
    }),
        Puzzle({
        {4,      8,      12,
        2,      6,      10,
        14,     4,      8,
        11,     1,      5,
        9,      13,     3,
        7,      11,     1,
        4,      8,      12,
        2,      6,      10,
        14,     11,     1,
        5,      9,      13,
        3,      7,      12,
        2,      6,      10,
        14,     5,      9,
        13,     3,      7},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        true,
        " "
    }),
        Puzzle({
        {  11,     8,      5,
        2,      13,     10,
        7,      4,      1,
        12,     9,      6,
        3,      14,     11,
        8,      4,      1,
        12,     9,      6,
        3,      14,     11,
        8,      5,      2,
        13,     10,     7,
        4,      1,      12,
        5,      2,      13,
        10,     7,      9,
        6,      3,      14 },
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        true,
        " "
    }),
};

int main()
{
    int i, j, k, n;
#pragma omp parallel for num_threads(NUM_PUZZLES) private(i) private(j) private(k) private(n)
    for (i = 0; i < NUM_PUZZLES; i++) {
        for (j = 0; j < NUM_COLUMNS; j++) {
            for (k = 0; k < NUM_ROWS; k++) {
                // mark down an occurence of integer n in column j of puzzle i.
                n = puzzles[i].slices[k][j];
                puzzles[i].occurences[n - 1]++;

                // if there is more than one occurence of integer n in column j of puzzle i, rotate row k.
                if (puzzles[i].occurences[n - 1] > 1) {
                    puzzles[i].timesRotated[k]++;
                    if (puzzles[i].timesRotated[k] < 3) {
                        puzzles[i].rotateSlice(puzzles[i].slices[k]);

                        // reset iterators and occurence count.
                        k = j = 0;
                        memset(puzzles[i].occurences, 0, NUM_COLORS);
                    }
                    // we've rotated slice k too many times, so there's no solution.
                    else {
                        puzzles[i].success = false;
                    }
                }
            }
            //once we are done iterating through the rows of column j, reset the occurences counter.
            for (int p = 0; p < 14; p++) {
                puzzles[i].occurences[p] = 0;
            }
        }

            if (puzzles[i].success /*|| true*/) {
                puzzles[i].message = "\nPuzzle " + to_string(i+1) + " has a solution:\n";
                for (int l = 0; l < NUM_ROWS; l++) {
                    puzzles[i].message += "[";
                    for (int m = 0; m < NUM_COLUMNS; m++) {
                        puzzles[i].message += to_string(puzzles[i].slices[l][m]);
                        if (m < NUM_COLUMNS - 1) puzzles[i].message += ",\t";
                    }
                    puzzles[i].message += "]\n";
                }
            }
            else
                puzzles[i].message = "\nPuzzle " + to_string(i + 1) + " has no solution.";
        }
#pragma omp single
    {
    cout << "Puzzle solutions:";
    for (int m = 0; m < 6; m++) {
        cout << puzzles[m].message;
    }
    }
    return 0;
}