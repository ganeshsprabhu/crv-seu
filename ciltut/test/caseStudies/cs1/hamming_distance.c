#include <stdio.h>
#include <stdlib.h>

// Function to compute the Hamming distance between two integers
int hamming_distance(int a, int b) {
    int dist = 0;
    int val = a ^ b; // XOR the two values to find differing bits

    // Count the number of 1's in the result (Hamming distance)
    while (val != 0) {
        dist += (val & 1);  // Add 1 if the least significant bit is set
        val >>= 1;          // Right shift by 1
    }

    return dist;
}

int main(int argc, char *argv[]) {
    // Ensure two command-line arguments are provided (x1 and x2)
    if (argc != 3) {
        printf("Usage: %s <x1> <x2>\n", argv[0]);
        return 1;
    }

    // Parse x1 and x2 from the command-line arguments
    int x1 = atoi(argv[1]);
    int x2 = atoi(argv[2]);

    // Calculate the Hamming distance between x1 and x2
    int dist = hamming_distance(x1, x2);

    // Check if the bit flip is a Single Event Upset (SEU) or Non-SEU (multiple bits)
    if (dist == 1) {
        printf("Hamming distance: %d -> Single bit flip detected (SEU)\n", dist);
    } else if (dist > 1) {
        printf("Hamming distance: %d -> Multiple bit flip detected (Non-SEU)\n", dist);
    } else {
        printf("Hamming distance: %d -> No bit flip detected (No SEU)\n", dist);
    }

    return 0;
}
