#include <stdio.h>

void null_pointer_example(int *ptr)
{
    if (ptr != NULL)
    {
        *ptr = 42;
    }
}

int main()
{
    int *ptr = NULL;
    null_pointer_example(ptr);
    return 0;
}
