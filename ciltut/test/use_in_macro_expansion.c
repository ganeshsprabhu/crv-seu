#include <stdio.h>

#define SQUARE(a) ((a) * (a))

int main() {
    int x = 5;
    int y = SQUARE(x);
    printf("%d\n", y);
    return 0;
}
