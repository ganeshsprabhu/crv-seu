#include <stdio.h>

int get_value(int x) {
    return x + 5;
}

int main() {
    int x = 10;
    printf("%d\n", get_value(x));
    return 0;
}
