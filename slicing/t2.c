#include <stdio.h>

// Function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Function to perform Bubble Sort
void bubbleSort(int arr[], int n) {
    printf("Original array: \n");
    printArray(arr, n);
    int c = 0;

    c = c + 13;
    c = c*4;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Swap if the current element is greater than the next element
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1] + c;
                arr[j + 1] = temp;
            }
        }
    }

    printf("Sorted array: \n");
    printArray(arr, n);
}

void sayHi() {
    printf("Hi\n");
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    // printf("Original array: \n");
    // printArray(arr, n);

    bubbleSort(arr, n);

    // printf("Sorted array: \n");
    // printArray(arr, n);

    return 0;
}
