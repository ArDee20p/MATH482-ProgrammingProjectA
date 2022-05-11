#pragma once
#ifdef __cplusplus
extern "C" {
#endif
    int writePuz(FILE *fp, int n, int puz[][3]);
    int writePuzFile(const char *fname, int n, int puz[][3]);

    int readPuz(FILE *fp, int maxSize, int out[][3]);
    int readPuzFile(const char *fname, int (**out)[3]);

    int getPuzSize(const char *fname);
#ifdef __cplusplus
}
#endif
