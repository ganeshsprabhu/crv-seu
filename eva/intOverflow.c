#include <stdio.h>
#include <limits.h>

void integer_overflow_example(int a, int b) {
    int result = a + b; // Potential overflow if a and b are too large
    printf("Result is: %d\n", result);
}

int main() {
    int x;
    scanf("%d", &x);
    integer_overflow_example(x, 1);
    return 0;
}
