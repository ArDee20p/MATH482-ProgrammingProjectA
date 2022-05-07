#include <stdio.h>
#include <stdlib.h>

void printPuz(int n, int *puz) {
    for (int s = 0; s < n; s++) {
        for (int c = 0; c < 3; c++) {
            printf("%d\t", puz[s * 3 + c]);
        }

        printf("\n");
    }
}

int findSol(int n, int colors, int *puz) {
    char orientation[n];
    for (int i = 0; i < n; i++) orientation[i] = 0;

    char occurances[colors];

    while (1) {
        int bad = 0;
        for (int c = 0; c < 3; c++) {
            for (int i = 0; i < colors; i++) occurances[i] = 0;
            for (int s = 0; s < n; s++) {
                int color = puz[3 * s + (c + orientation[s]) % 3];
                occurances[color - 1]++;

                if (occurances[color - 1] > 1) {
                    bad = 1;
                    break;
                }
            }

            if (bad) break;
        }

        if (!bad) {
            for (int s = 0; s < n; s++) {
                for (; orientation[s] > 0; orientation[s]--) {
                    int first = puz[3 * s];
                    puz[3 * s] = puz[3 * s + 1];
                    puz[3 * s + 1] = puz[3 * s + 2];
                    puz[3 * s + 2] = first;
                }
            }

            return 1;
        }

        int done = 1;
        for (int i = 1; i < n; i++) {
            if (orientation[i] < 2) {
                orientation[i]++;
                done = 0;
                break;
            }

            orientation[i] = 0;
        }

        if (done) {
            return 0;
        }
    }
}


int puzzleSlices[6 * 14 * 3] = {
    //Puzzle One
    12,     9,      7,
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
    12,     8,      3,
    //Puzzle Two
    9,      4,      13,
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
    10,     5,      14,
    //Puzzle Three
    11,     7,      4,
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
    1,      4,      1,
    //Puzzle Four
    3,      5,      8,
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
    12,     7,      10,
    //Puzzle Five
    4,      8,      12,
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
    13,     3,      7,
    //Puzzle Six
    11,     8,      5,
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
    6,      3,      14
};


int main() {
    for (int i = 0; i < 6; i++) {
        int *puz = &puzzleSlices[3 * 14 * i];
        if (findSol(14, 14, puz)) {
            printf("Found solution for puzzle %d\n", i+1);
            printPuz(14, puz);
        } else {
            printf("No solution for puzzle %d\n", i+1);
        }
    }
}