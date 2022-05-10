#include "Puzzle.h"
#include <math.h>
#include <string>

    int* Puzzle::rotateSlice(int* slice) {
        int tmp;

        tmp = slice[0];
        slice[0] = slice[2];
        slice[2] = slice[1];
        slice[1] = tmp;

        return slice;
    }

    int* Puzzle::flipSlice(int* slice, int a, int b) {
        int tmp;

        tmp = slice[a];
        slice[a] = slice[b];
        slice[b] = tmp;

        return slice;
    }

    bool Puzzle::checkOccurences() {
        for (int i = 0; i < 14; i++) {
            if (occurences[i] > 1) {
                success = false;
                return success;
            }
        }
        success = true;
        return success;
    }

    void Puzzle::resetIteration() {
        for (int i = 0; i < NUM_ROWS; i++) {
            occurences[i] = 0;
        }
    }

    void Puzzle::printPuzzles(int i) {
        std::cout << "Puzzle " + std::to_string(i + 1) + "\n";
        for (int j = 0; j < NUM_ROWS; j++) {
            std::cout << "[";
            for (int k = 0; k < NUM_COLUMNS; k++) {
                std::cout << std::to_string(slices[j][k]);
                if (k < NUM_COLUMNS - 1) std::cout << ",\t";
            }
            std::cout << "]\n";
        }
        std::cout << "\n";
    }

    void Puzzle::printSolution(int i) {
        if (success) {
            message = "\nPuzzle " + std::to_string(i + 1) + " has a solution:\n";
            for (int i = 0; i < NUM_ROWS; i++) {
                message += "[";
                for (int j = 0; j < NUM_COLUMNS; j++) {
                    message += std::to_string(slices[i][j]);
                    if (j < NUM_COLUMNS - 1) message += ",\t";
                }
                message += "]\n";
            }
        }
        else
            message = "\nPuzzle " + std::to_string(i + 1) + " has no solution.";
    }

    int Puzzle::puzzleFunction(int n, int puzz) {
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

    Puzzle* Puzzle::generatePuzzles(Puzzle puzzles[NUM_PUZZLES]) {

        int i, j, k, n, num;
        bool check = false;
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
            std::cout << "All puzzles generated:\n";
            for (i = 0; i < NUM_PUZZLES; i++) {
                puzzles[i].printPuzzles(i);
            }
        }
        return puzzles;
    }

    void Puzzle::solvePuzzles(Puzzle puzzles[NUM_PUZZLES]) {
        int i, j, k, n;
        static int possibleFlips[6][2] = {
            { 0, 1 },
            { 1, 2 },
            { 0, 1 },
            { 1, 2 },
            { 0, 1 },
            { 1, 2 }
        };
#pragma omp parallel for num_threads(NUM_PUZZLES) private(i) private(j) private(k) private(n) shared(puzzles)
        for (i = 0; i < NUM_PUZZLES; i++) {
            for (j = 0; j < NUM_COLUMNS; j++) {
                for (k = 0; k < NUM_ROWS; k++) {
                    // mark down an occurence of integer n in column j of puzzle i.
                    n = puzzles[i].slices[k][j];
                    puzzles[i].occurences[n - 1]++;

                    // if there is more than one occurence of integer n in column j of puzzle i, modify row k.
                    if (puzzles[i].occurences[n - 1] > 1) {
                        //if (omp_get_thread_num() == TARGET_PUZZLE)
                        //    std::cout << "[Puzzle " << i + 1 << "] occurences of " << n << " in column " << j + 1 << " are " << puzzles[i].occurences[n - 1] << ".\n";

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
            std::cout << "Puzzle solutions:";
            for (int m = 0; m < 6; m++) {
                std::cout << puzzles[m].message;
            }
        }
    }
