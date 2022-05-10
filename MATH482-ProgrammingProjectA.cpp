// COMP482 - Programming Project Part A - Pizza Slice Instant Insanity - Spring 2022
// James Lord | Ben Burrill | Leo Hacopian | Nathanael Ochoa | Sabrina Padilla | Oscar Ulloa | Steven Condor

#include "Puzzle.h"

int main() {
    Puzzle puzzles[NUM_PUZZLES];

    Puzzle::generatePuzzles(puzzles);
    Puzzle::solvePuzzles(puzzles);

    return 0;
}