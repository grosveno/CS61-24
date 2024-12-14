#include <cstdlib>
#include <cstdio>

int main() {
    int cnt = 0;
    while (fgetc(stdin) != EOF) {
        cnt++;
    }
    fprintf(stdout, "%d\n", cnt);
    exit(0);
}