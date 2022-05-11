#include <stdio.h>
#include <stdlib.h>
#include "puzfile.h"

int writePuz(FILE *fp, int n, int puz[][3]) {
   for (int i = 0; i < n; i++) {
       if (fprintf(fp, "%d\t%d\t%d\n", puz[i][0], puz[i][1], puz[i][2]) < 3) {
           return 0;
       }
   }

   return 1;
}

int writePuzFile(char *fname, int n, int puz[][3]) {
    FILE *fp = fopen(fname, "w");
    if (!fp) return 0;
    int result = writePuz(fp, n, puz);
    fclose(fp);
    return result;
}

int readPuz(FILE *fp, int maxSize, int out[][3]) {
    int s;
    for (s = 0; s < maxSize; s++) {
        if (fscanf(fp, "%d\t%d\t%d\n", &out[s][0], &out[s][1], &out[s][2]) < 3)
            break;
    }

    return s;
}

int readPuzFile(char *fname, int (**out)[3]) {
    int size = getPuzSize(fname);
    int (*puz)[3] = malloc(size * 3 * sizeof(int));
    if (!puz) return -1;

    FILE *fp = fopen(fname, "r");
    if (!fp) return -1;

    int sizeRead = readPuz(fp, size, puz);
    fclose(fp);

    if (sizeRead != size) return -1;

    *out = puz;
    return sizeRead;
}

int getPuzSize(char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) return -1;

    int nlines = 0;
    for (int ch; (ch = fgetc(fp)) != EOF;) {
        if (ch == '\n') nlines++;
    }

    return nlines;
}

