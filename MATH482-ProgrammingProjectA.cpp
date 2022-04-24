//COMP482 - Programming Project Part A - Pizza Slice Instant Insanity - Spring 2022

#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>
#include <numeric>
#include <string>

using namespace std;
using namespace std::chrono;

int* rotateSlice(int* slice) {
    int tmp0;

    tmp0 = slice[0];
    slice[0] = slice[1];
    slice[1] = slice[2];
    slice[2] = tmp0;

    return slice;
}

int main()
{
    int i, j, k, n;
    bool success = true;
    int puzzleSlices[84][3] = {
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
    int occurences[14];
    int timesRotated[14];
    memset(occurences, 0, sizeof(occurences));
    memset(timesRotated, 0, sizeof(timesRotated));
    string messages[6];

#pragma omp parallel for num_threads(6) firstprivate(success) private(i) private(j) private(k) private(n) private(occurences) shared(puzzleSlices) shared(messages)
    for (j = 0; j < 3; j++) { // depth-first search
        for (i = 0 + (14 * omp_get_thread_num()); i < 14 + 14 * omp_get_thread_num(); i++) {
            n = puzzleSlices[i][j];
            occurences[n - 1]++;
            if (occurences[n - 1] > 1) { // collision found
                timesRotated[i]++;
                if (!(timesRotated[i] >= 2)) {
                    rotateSlice(puzzleSlices[i]);
                    i = 0 + (14 * omp_get_thread_num()); // reset row iterator
                    //memset(occurences, 0, sizeof(occurences)); // reset occurences array
                }
                else { // we've rotated this slice too many times, there's no solution.
                    success = false;
                }
            }
        }
        memset(occurences, 0, sizeof(occurences));

        if (success) {
            messages[omp_get_thread_num()] = "\nPuzzle " + to_string(omp_get_thread_num()) + " has a solution: [";
            for (i = 0 + (14 * omp_get_thread_num()); i < 14 + 14 * omp_get_thread_num(); i++) {
                messages[omp_get_thread_num()] += "[ ";
                for (k = 0; k < 3; k++) {
                    messages[omp_get_thread_num()] += " " + to_string(puzzleSlices[i][k]) + ",";
                }
                messages[omp_get_thread_num()] += "]";
            }
            messages[omp_get_thread_num()] += " ]\n";
        }
        else
            messages[omp_get_thread_num()] = "\nPuzzle " + to_string(omp_get_thread_num()) + " has no solution.";
    }
#pragma omp single
    {
        cout << "Puzzle solutions:";
        for (int i = 0; i < 6; i++) {
            cout << messages[i];
        }
    }
    system("PAUSE");
    return 0;
}