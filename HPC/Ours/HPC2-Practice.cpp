#include <iostream>
#include <omp.h>

using namespace std;

void sequential_bubble_sort(int arr[], int size)
{
    int array[size];
    for (int i = 0; i < size; i++)
    {
        array[i] = arr[i];
    }

    double start = omp_get_wtime();
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
        }
    }
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort:\n";
    cout << endl;
    cout << "Time Required: " << end - start << endl;
}

void parallel_bubble_sort(int arr[], int size)
{
    int array[size];
    for (int i = 0; i < size; i++)
    {
        array[i] = arr[i];
    }

    double start = omp_get_wtime();
    for (int k = 0; k < size; k++)
    {
        if (k % 2 == 0)
        {
#pragma omp parallel for
            for (int i = 1; i < size - 1; i += 2)
            {
                if (array[i] > array[i + 1])
                    swap(array[i], array[i + 1]);
            }
        }
        else
        {
#pragma omp parallel for
            for (int i = 0; i < size - 1; i += 2)
            {
                if (array[i] > array[i + 1])
                    swap(array[i], array[i + 1]);
            }
        }
    }
    double end = omp_get_wtime();
    cout << "Parallel Bubble Sort:\n";
    cout << "Time Required: " << end - start << endl;
}

void merge(int array[], int low, int mid, int high, int size)
{
    int temp[size];
    int i = low;
    int j = mid + 1;
    int k = 0;

    while ((i <= mid) && (j <= high))
    {
        if (array[i] >= array[j])
        {
            temp[k] = array[j];
            k++;
            j++;
        }
        else
        {
            temp[k] = array[i];
            k++;
            i++;
        }
    }

    while (i <= mid)
    {
        temp[k] = array[i];
        k++;
        i++;
    }

    while (j <= high)
    {
        temp[k] = array[j];
        k++;
        j++;
    }

    k = 0;
    for (int i = low; i <= high; i++)
    {
        array[i] = temp[k];
        k++;
    }
}

void mergesort(int array[], int low, int high, int size)
{
    if (low < high)
    {
        int mid = (low + high) / 2;

        // Parallelize the recursive merge sort with tasks
#pragma omp task shared(array) if ((high - low) > 1000) // Adjust depth to parallelize
        {
            mergesort(array, low, mid, size);
        }

#pragma omp task shared(array) if ((high - low) > 1000)
        {
            mergesort(array, mid + 1, high, size);
        }

        // Synchronize tasks and perform merge
#pragma omp taskwait
        merge(array, low, mid, high, size);
    }
}

void perform_merge_sort(int arr[], int size)
{
    int array[size];
    for (int i = 0; i < size; i++)
    {
        array[i] = arr[i];
    }

    double start = omp_get_wtime();
    mergesort(array, 0, size - 1, size);
    double end = omp_get_wtime();

    cout << "Merge Sort:\n";
    cout << "Time Required: " << end - start << endl;
}

void p_mergesort(int array[], int low, int high, int size)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            mergesort(array, low, mid, size);
#pragma omp section
            mergesort(array, mid + 1, high, size);
        }

        merge(array, low, mid, high, size);
    }
}

void perform_p_merge_sort(int arr[], int size)
{
    int array[size];
    for (int i = 0; i < size; i++)
    {
        array[i] = arr[i];
    }

    double start = omp_get_wtime();
    // Start parallel region and parallelize the merge sort task
#pragma omp parallel
    {
#pragma omp single
        {
            mergesort(array, 0, size - 1, size);
        }
    }
    double end = omp_get_wtime();

    cout << "Parallel Merge Sort:\n";
    cout << "Time Required: " << end - start << endl;
}

int main()
{
    int SIZE;
    int MAX = 20000;
    cout << "Enter size of array: ";
    cin >> SIZE;
    int array[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        array[i] = rand() % MAX;
    }
    sequential_bubble_sort(array, SIZE);
    parallel_bubble_sort(array, SIZE);
    perform_merge_sort(array, SIZE);
    perform_p_merge_sort(array, SIZE);

    return 0;
}
