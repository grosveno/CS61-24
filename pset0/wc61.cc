#include <cstdlib>
#include <cstdio>
#include <cctype>

int main() {
    int cnt_chars = 0;
    int cnt_words = 0;
    int cnt_lines = 0;
    int input = fgetc(stdin);
    char pre = ' ';
    while (input != EOF) {
        cnt_chars++;
        char c = input;
        if (isspace(pre) && isalpha(c)) {
            cnt_words++;
        } else if (c == '\n') {
            cnt_lines++;
        }
        input = fgetc(stdin);
        pre = c;
    }
    if (pre != '\n' && cnt_chars != 0) {
        cnt_lines++;
        cnt_words--;
    }
    fprintf(stdout, "%10d%10d%10d\n", cnt_words, cnt_lines, cnt_chars);
    exit(0);
}