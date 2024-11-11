#include <stdio.h>

void division_by_zero_example(int divisor) {
    int result;
    // if (divisor != 0) {
        result = 100 / divisor;
        printf("Result is: %d\n", result);
    // }
}

int main() {
    division_by_zero_example(0); // Unsafe if called with zero
    return 0;
}
