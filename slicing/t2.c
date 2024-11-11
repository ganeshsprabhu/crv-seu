#include <stdio.h>

int xyz = 1348;

// Function to print the array
void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void bubbleSort(int arr[], int n, int k)
{
    printf("Original array: \n");
    printArray(arr, n);
    int c = 0;

    c = c + 13;
    c = c * 4;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("Sorted array: \n");
    printArray(arr, n);
}

void sayHi()
{
    printf("Hi\n");
}

int main()
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: \n");
    printArray(arr, n);

    bubbleSort(arr, n, xyz);

    // printf("Sorted array: \n");
    // printArray(arr, n);

    return 0;
}
