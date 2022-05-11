#include <stdio.h>
#include <stdlib.h>
#include "puzfile.h"

int findSol(int n, int colors, int puz[][3]) {
    char orientation[n];
    for (int i = 0; i < n; i++) orientation[i] = 0;

    char occurances[colors];

    while (1) {
        int bad = 0;
        for (int c = 0; c < 3; c++) {
            for (int i = 0; i < colors; i++) occurances[i] = 0;
            for (int s = 0; s < n; s++) {
                int color = puz[s][(c + orientation[s]) % 3];
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
                    int first = puz[s][0];
                    puz[s][0] = puz[s][1];
                    puz[s][1] = puz[s][2];
                    puz[s][2] = first;
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



int findSolPar(int n, int colors, int puz[][3], int pdeg) {
    int ploops = 1;
    for (int i = 0; i < pdeg; i++) ploops *= 3;

    int start = 1 + pdeg;
    int foundSol = 0;

    #pragma omp parallel for shared(puz, foundSol)
    for (int prefix = 0; prefix < ploops; prefix++) {
        char orientation[n];
        for (int i = 0; i < n; i++) orientation[i] = 0;

        char occurrences[colors];
        for (int r = prefix, i = 1; r > 0; r /= 3) {
            occurrences[i++] = r % 3;
        }

        while (1) {
            int bad = 0;
            for (int c = 0; c < 3; c++) {
                for (int i = 0; i < colors; i++) occurrences[i] = 0;
                for (int s = 0; s < n; s++) {
                    int color = puz[s][(c + orientation[s]) % 3];
                    occurrences[color - 1]++;

                    if (occurrences[color - 1] > 1) {
                        bad = 1;
                        break;
                    }
                }

                if (bad) break;
            }

            if (!bad) {
                #pragma omp critical
                if (!foundSol) {
                    for (int s = 0; s < n; s++) {
                        for (; orientation[s] > 0; orientation[s]--) {
                            int first = puz[s][0];
                            puz[s][0] = puz[s][1];
                            puz[s][1] = puz[s][2];
                            puz[s][2] = first;
                        }
                    }

                    foundSol = 1;
                }
            }

            if (foundSol) break;

            int done = 1;
            for (int i = start; i < n; i++) {
                if (orientation[i] < 2) {
                    orientation[i]++;
                    done = 0;
                    break;
                }

                orientation[i] = 0;
            }

            if (done) break;
        }
    }

    return foundSol;
}



int findSolFlip(int n, int colors, int puz[][3]) {
    char orientation[n];
    for (int i = 0; i < n; i++) orientation[i] = 0;

    char occurances[colors];

    while (1) {
        int bad = 0;
        for (int c = 0; c < 3; c++) {
            for (int i = 0; i < colors; i++) occurances[i] = 0;
            for (int s = 0; s < n; s++) {
                int ct = (c + orientation[s] % 3) % 3;
                if (orientation[s] >= 3) {
                    // swap cols 0,2
                    ct = 2 - ct;
                }

                int color = puz[s][ct];
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
                if (orientation[s] >= 3) {
                    int first = puz[s][0];
                    puz[s][0] = puz[s][2];
                    puz[s][2] = first;
                    orientation[s] -= 3;
                }

                for (; orientation[s] > 0; orientation[s]--) {
                    int first = puz[s][0];
                    puz[s][0] = puz[s][1];
                    puz[s][1] = puz[s][2];
                    puz[s][2] = first;
                }
            }

            return 1;
        }

        int done = 1;
        for (int i = 1; i < n; i++) {
            if (orientation[i] < 5) {
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


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Pass a puzzle file\n");
        return 1;
    }

    int (*puz)[3];
    int size = readPuzFile(argv[1], &puz);

    if (findSolPar(size, size, puz, 2)) {
        printf("Found solution for %s\n", argv[1]);
        writePuz(stdout, size, puz);
    } else {
        printf("No solution for puzzle %s\n", argv[1]);
        printf("Trying with flipping...\n");
        if (findSolFlip(size, size, puz)) {
            printf("Invalid puzzle %s\n");
        } else {
            writePuz(stdout, size, puz);
        }
    }

    free(puz);

    return 0;
}
