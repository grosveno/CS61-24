#include <cstdio>

int main() {
    unsigned long cnt = 0;
    while (fgetc(stdin) != EOF) {
        cnt++;
    }
    fprintf(stdout, "%lu\n", cnt);
    exit(0);
}