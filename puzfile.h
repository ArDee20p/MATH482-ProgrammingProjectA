#pragma once
#ifdef __cplusplus
extern "C" {
#endif
    int writePuz(FILE *fp, int n, int puz[][3]);
    int writePuzFile(char *fname, int n, int puz[][3]);

    int readPuz(FILE *fp, int maxSize, int out[][3]);
    int readPuzFile(char *fname, int (**out)[3]);

    int getPuzSize(char *fname);
#ifdef __cplusplus
}
#endif
