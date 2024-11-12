int notUsedInMainDirectly(int x)
{
    return x;
}

int compare(int a, int b)
{
    if (a > b)
    {
        return notUsedInMainDirectly(-1);
    }

    return notUsedInMainDirectly(1);
}

int main()
{
    int a = 5;
    int b = 10;

    if (a > 10 || compare(a, b) == 1)
    {
        return a;
    }

    // ! Statement never reached as compare(a, b) == 1 is satisfied
    return b;
}
