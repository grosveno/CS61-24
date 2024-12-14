#include <cstdio>
#include <cctype>

int main() {
    unsigned long nl = 0, nw = 0, nc = 0;
    bool pre_space = true;
    while (true) {
        int ch = fgetc(stdin);
        if (ch == EOF) {
            break;
        }

        nc++;
        bool this_space = isspace((unsigned)ch);
        if (pre_space && !this_space) {
            nw++;
        }
        if (ch == '\n') {
            nl++;
        }
        
        pre_space = isspace((unsigned)ch);
    }
    fprintf(stdout, "%8lu %7lu %7lu\n", nl, nw, nc);
}