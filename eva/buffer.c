#include <stdio.h>

void buffer_overflow_example() {
    int buffer[5];
    for (int i = 0; i <= 5; i++) {
        buffer[i] = i * 2;
    }
}

int main() {
    buffer_overflow_example();
    return 0;
}
