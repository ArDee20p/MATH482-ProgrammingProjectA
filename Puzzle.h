#pragma once
#include <iostream>
#include <omp.h>
#include <string>

#define M_PI            3.14159265358979323846
#define NUM_ROWS        14
#define NUM_COLORS      14
#define NUM_COLUMNS     3
#define NUM_PUZZLES     6
#define TARGET_PUZZLE   1

class Puzzle {
private:
    int slices[NUM_ROWS][NUM_COLUMNS] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    int occurences[NUM_COLORS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int timesModified[NUM_ROWS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::string message = " ";
    bool success = true;
    bool backtrack = false;

    int* rotateSlice(int* slice);
    int* flipSlice(int* slice, int a, int b);
    bool checkOccurences();
    void resetIteration();
    void printPuzzles(int i);
    void printSolution(int i);
    static int puzzleFunction(int n, int puzz);

public:
    static Puzzle* generatePuzzles(Puzzle puzzles[NUM_PUZZLES]);
    static void solvePuzzles(Puzzle puzzles[NUM_PUZZLES]);
};