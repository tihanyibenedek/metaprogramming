#include <iostream>

using swap_fn = void(*)(void*, int const, int const);
using compare_fn = bool(*)(void*, int const, int const);

int partition(void* arr, int const low, int const high, compare_fn fcomp, swap_fn fswap)
{
    std::cout << "partition" << std::endl;

    int i = low - 1;
    for (int j = low; j <= high - 1; j++)
    {
        if (fcomp(arr, j, high))
        {
            i++;
            fswap(arr, i, j);
        }
    }

    fswap(arr, i+1, high);

    return i+1;
}

void quicksort(void* arr, int const low, int const high, compare_fn fcomp, swap_fn fswap)
{
    std::cout << "quicksort" << std::endl;

    if (low < high)
    {
        int const pi = partition(arr, low, high, fcomp, fswap);
        quicksort(arr, low, pi-1, fcomp, fswap);
        quicksort(arr, pi+1, high, fcomp, fswap);
    }
}

void swap_int(void* arr, int const i, int const j)
{
    std::cout << "swap_int" << std::endl;

    int* iarr = (int*)arr;
    int t = iarr[i];
    iarr[i] = iarr[j];
    iarr[j] = t;
}

bool less_int(void* arr, int const i, int const j)
{
    std::cout << "less_int" << std::endl;

    int* iarr = (int*)arr;
    return iarr[i] < iarr[j];
}

void print_arr(void* arr, int size)
{
    std::cout << "print_arr" << std::endl;

    int* iarr = (int*)arr;
    for(int i = 0; i < size; ++i)
    {
        std::cout << iarr[i] << " ";
    }
    std::cout << std::endl;
}

template<typename T>
T max(T const a, T const b)
{
    return a > b ? a : b;
}

template<typename T>
void swap(T* a, T* b)
{
    T t = *a;
    *a = *b;
    *b = t;
}

template<typename T>
int partition(T arr[], int const low, int const high)
{
    T pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high-1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i+1], &arr[high]);

    return i+1;
}

template<typename T>
void quicksort(T arr[], int const low, int const high)
{
    if (low < high)
    {
        int const pi = partition(arr, low, high);
        quicksort(arr, low, pi-1);
        quicksort(arr, pi+1, high);
    }
}

struct foo
{ };


int main()
{
    std::cout << "Understanding the need for templates" << std::endl;

    int arr[] = {13, 1, 8, 3, 5, 2, 1};
    int n = sizeof(arr)/sizeof(arr[0]);
    print_arr(arr, n);
    quicksort(arr, 0, n-1, less_int, swap_int);
    print_arr(arr, n);

    std::cout << "Writing your first templates" << std::endl;

    foo f1, f2;

    max(1, 2);
    max(1.0, 2.0);
    // max(f1, f2);

    int arr2[] = {13, 1, 8, 3, 5, 2, 1};
    int n2 = sizeof(arr)/sizeof(arr[0]);
    print_arr(arr2, n2);
    quicksort(arr2, 0, n2-1);
    print_arr(arr2, n2);

    std::cout << "Understanding template terminology" << std::endl;

    return 0;
}