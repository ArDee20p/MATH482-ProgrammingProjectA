//COMP482 - Programming Project Part A - Pizza Slice Instant Insanity - Spring 2022

#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <math.h>
#include <sstream>

#define M_PI            3.14159265358979323846
#define NUM_ROWS        14
#define NUM_COLORS      14
#define NUM_COLUMNS     3
#define NUM_PUZZLES     6
#define TARGET_PUZZLE   1

using namespace std;

class Puzzle {
public:
    int slices[NUM_ROWS][NUM_COLUMNS];
    int solution[NUM_ROWS][NUM_COLUMNS] = {
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
    int occurences[NUM_COLORS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int collisions[NUM_COLORS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int timesModified[NUM_ROWS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    bool success = true;
    bool backtrack = false;
    string message = " ";
    
    int* rotateSlice(int* slice) {
        int tmp;

        tmp = slice[0];
        slice[0] = slice[2];
        slice[2] = slice[1];
        slice[1] = tmp;

        return slice;
    }

    int* flipSlice(int* slice, int a, int b) {
        int tmp;
        
        tmp = slice[a];
        slice[a] = slice[b];
        slice[b] = tmp;

        return slice;
    }

    bool checkOccurences() {
        for (int i = 0; i < 14; i++) {
            if (occurences[i] > 1) {
                success = false;
                return success;
            }
        }
        success = true;
        return success;
    }

    void resetIteration() {
        for (int i = 0; i < NUM_ROWS; i++) {
            occurences[i] = 0;
        }
    }

    void printPuzzles(int i) {
            cout << "Puzzle " + to_string(i + 1) + "\n";
            for (int j = 0; j < NUM_ROWS; j++) {
                cout << "[";
                for (int k = 0; k < NUM_COLUMNS; k++) {
                    cout << to_string(slices[j][k]);
                    if (k < NUM_COLUMNS - 1) cout << ",\t";
                }
                cout << "]\n";
            }
            cout << "\n";
    }

    void printSolution(int i) {
        if (success) {
            message = "\nPuzzle " + to_string(i + 1) + " has a solution:\n";
            for (int i = 0; i < NUM_ROWS; i++) {
                message += "[";
                for (int j = 0; j < NUM_COLUMNS; j++) {
                    message += to_string(slices[i][j]);
                    if (j < NUM_COLUMNS - 1) message += ",\t";
                }
                message += "]\n";
            }
        }
        else
            message = "\nPuzzle " + to_string(i + 1) + " has no solution.";
    }
};

Puzzle puzzles[NUM_PUZZLES];

int puzzleFunction(int n, int puzz) {
    switch (puzz) {
    case 0:
        return 1 + (int)floor(n * 4 * M_PI * M_PI) % 14;
        break;
    case 1:
        return 1 + (int)floor(n * 5 * exp(2)) % 14;
        break;
    case 2:
        return 1 + (int)floor(n * 4 * exp(3)) % 14;
        break;
    case 3:
        return 1 + (int)floor(n * 5 * exp(3)) % 14;
        break;
    case 4:
        return 1 + (int)floor(n * 10 * exp(2)) % 14;
        break;
    case 5:
        return 1 + (int)floor(n * 11 * exp(3)) % 14;
    }
}

static int flip1[2] = { 0, 1 };
static int flip2[2] = { 1, 2 };
static int possibleFlips[6][2] = {
    *flip1,
    *flip2,
    *flip1,
    *flip2,
    *flip1,
    *flip2
};

int main() {
    int i, j, k, n, num;
    bool check;

    //puzzle generation
#pragma omp parallel for num_threads(NUM_PUZZLES) private(i) private(j) private(k) private(n) private(check)
    for (i = 0; i < NUM_PUZZLES; i++) {
        n = 1;
        check = false;
        for (k = 0; k < NUM_ROWS; k++) {
            for (j = 0; j < NUM_COLUMNS; j++) {
                while (!check) {
                    num = puzzleFunction(n, i);
                    if (puzzles[i].occurences[num - 1] >= 3) {
                        check = false;
                        n++;
                    }
                    else if (puzzles[i].occurences[num - 1] < 3) {
                        puzzles[i].slices[k][j] = num;
                        n++;
                        puzzles[i].occurences[num - 1] += 1;
                        check = true;
                    }
                }
                check = false;
            }
        }
        puzzles[i].resetIteration();
    }

#pragma omp single
    {
        cout << "All puzzles generated:\n";
        for (i = 0; i < NUM_PUZZLES; i++) {
            puzzles[i].printPuzzles(i);
        }
    }

    //puzzle solving
#pragma omp parallel for num_threads(NUM_PUZZLES) private(i) private(j) private(k) private(n)
    for (i = 0; i < NUM_PUZZLES; i++) {
        for (j = 0; j < NUM_COLUMNS; j++) {
            for (k = 0; k < NUM_ROWS; k++) {
                // mark down an occurence of integer n in column j of puzzle i.
                n = puzzles[i].slices[k][j];
                puzzles[i].occurences[n - 1]++;

                // if there is more than one occurence of integer n in column j of puzzle i, modify row k.
                if (puzzles[i].occurences[n - 1] > 1) {
                    if (omp_get_thread_num() == TARGET_PUZZLE)
                        cout << "[Puzzle " << i + 1 << "] occurences of " << n << " in column " << j + 1 << " are " << puzzles[i].occurences[n - 1] << ".\n";
                   
                        puzzles[i].flipSlice(puzzles[i].slices[k], possibleFlips[puzzles[i].timesModified[k]][0], possibleFlips[puzzles[i].timesModified[k]][1]);
                        puzzles[i].timesModified[k]++;
                }
            }
            puzzles[i].resetIteration();
        }
        puzzles[i].printSolution(i);
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