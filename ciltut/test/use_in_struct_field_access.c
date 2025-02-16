#include <stdio.h>

struct S {
    int x;
};

int main() {
    struct S s;
    s.x = 5;
    int y = s.x;
    return 0;
}
