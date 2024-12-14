#include <cstring>
#include <cassert>
#include <cstdio>

char* mystrstr(const char* s1, const char* s2) {
    if (!*s2) {
        return (char*)s1;
    }
    for (size_t i = 0; *(s1 + i); i++) {
        size_t j = 0;
        while (*(s2 + j) && *(s2 + j) == *(s1 + i + j)) {
            j++;
        }
        if (!*(s2 + j)) {
            return (char*)(s1 + i);
        }
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    assert(argc == 3);
    printf("strstr(\"%s\", \"%s\") = %p\n",
           argv[1], argv[2], strstr(argv[1], argv[2]));
    printf("mystrstr(\"%s\", \"%s\") = %p\n",
           argv[1], argv[2], mystrstr(argv[1], argv[2]));
    assert(strstr(argv[1], argv[2]) == mystrstr(argv[1], argv[2]));
}