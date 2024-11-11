#include <stdio.h>

void constant_folding_example() {
    int a = 3 + 5; // SCF can fold this to int a = 8;
    printf("Value of a: %d\n", a);
}

void constant_propagation_example() {
    int x = 10;     // SCF identifies x as a constant value
    int y = x * 2;  // SCF replaces y = x * 2 with y = 20
    printf("Value of y: %d\n", y);
}

void redundant_branch_example() {
    int x = 5;
    if (x > 0) { // SCF knows x > 0 is always true here
        printf("x is positive\n");
    } else {
        printf("x is non-positive\n"); // This code is unreachable and will be removed
    }
}

void compile_time_constant_example() {
    const int a = 7;
    const int b = 3;
    int result = a * b; // SCF can replace this with result = 21
    printf("Result: %d\n", result);
}

int main() {
    constant_folding_example();
    constant_propagation_example();
    redundant_branch_example();
    compile_time_constant_example();
    return 0;
}
