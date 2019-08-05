bool anyTrue(const double a[], int n)
{
    if (n <= 0)
        return false;
    if (somePredicate(*a))
        return true;

    return anyTrue(a + 1, n - 1);
}

int countTrue(const double a[], int n)
{

    if (n <= 0)
        return 0;

    return somePredicate(*a) ? 1 + countTrue(a + 1, n - 1) : countTrue(a + 1, n - 1);
}

int firstTrue(const double a[], int n)
{
    if (n <= 0)
        return -1;

    int pos = firstTrue(a + 1, n - 1);

    if (pos == -1)
        return somePredicate(*a) ? 0 : -1;

    return somePredicate(*a) ? 0 : 1 + pos;
}

int positionOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;

    if (n == 1)
        return 0;

    int mid = n / 2;

    int min1 = positionOfMin(a, mid);
    int min2 = positionOfMin(a + mid, n - mid) + mid;

    return (a[min1] < a[min2]) ? min1 : min2;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return true;
    if (n1 < n2)
        return false;

    return (a1[0] == a2[0]) ? includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1) : includes(a1 + 1, n1 - 1, a2, n2);
}