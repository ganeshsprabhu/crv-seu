#include <stdio.h>

void out_of_bounds_example(int index) {
    int array[10];
    // if (index >= 0 && index < 10) {
        array[index] = 5;
    // }
}

int main() {
    out_of_bounds_example(12);
    return 0;
}
