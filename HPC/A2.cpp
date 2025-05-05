// #include <iostream>
// #include <omp.h>
// #include <vector>
// #include <chrono>

// using namespace std;
// using namespace chrono;

// void merge(int arr[], int low, int mid, int high, int temp[]) {
//     int i = low, j = mid + 1, k = low;
//     while (i <= mid && j <= high) {
//         if (arr[i] <= arr[j]) temp[k++] = arr[i++];
//         else temp[k++] = arr[j++];
//     }
//     while (i <= mid) temp[k++] = arr[i++];
//     while (j <= high) temp[k++] = arr[j++];
//     for (int i = low; i <= high; ++i) arr[i] = temp[i];
// }

// void parallel_merge_sort(int arr[], int low, int high, int temp[], int depth) {
//     if (low < high) {
//         int mid = (low + high) / 2;
//         if (depth <= 4) {
//             #pragma omp parallel sections
//             {
//                 #pragma omp section
//                 parallel_merge_sort(arr, low, mid, temp, depth + 1);
//                 #pragma omp section
//                 parallel_merge_sort(arr, mid + 1, high, temp, depth + 1);
//             }
//         } else {
//             parallel_merge_sort(arr, low, mid, temp, depth + 1);
//             parallel_merge_sort(arr, mid + 1, high, temp, depth + 1);
//         }
//         merge(arr, low, mid, high, temp);
//     }
// }

// void sequential_merge_sort(int arr[], int low, int high, int temp[]) {
//     if (low < high) {
//         int mid = (low + high) / 2;
//         sequential_merge_sort(arr, low, mid, temp);
//         sequential_merge_sort(arr, mid + 1, high, temp);
//         merge(arr, low, mid, high, temp);
//     }
// }

// int main() {
//     int SIZE;
//     cout << "Enter size of array: ";
//     cin >> SIZE;

//     vector<int> original(SIZE);
//     for (int i = 0; i < SIZE; ++i)
//         original[i] = rand() % 100000;

//     // Sequential Merge Sort
//     vector<int> seq_arr = original;
//     vector<int> temp_seq(SIZE);
//     auto start_seq = high_resolution_clock::now();
//     sequential_merge_sort(seq_arr.data(), 0, SIZE - 1, temp_seq.data());
//     auto end_seq = high_resolution_clock::now();
//     cout << "Sequential Merge Sort Time: "
//          << duration<double>(end_seq - start_seq).count() << " seconds\n";

//     // Parallel Merge Sort
//     vector<int> par_arr = original;
//     vector<int> temp_par(SIZE);
//     auto start_par = high_resolution_clock::now();
//     parallel_merge_sort(par_arr.data(), 0, SIZE - 1, temp_par.data(), 0);
//     auto end_par = high_resolution_clock::now();
//     cout << "Parallel Merge Sort Time: "
//          << duration<double>(end_par - start_par).count() << " seconds\n";

//     return 0;
// }


#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <chrono>
using namespace std;

void sequential_bubble_sort(const vector<int>& arr) {
    vector<int> array = arr;
    double start = omp_get_wtime();
    int size = array.size();
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(array[j] > array[j+1]) {
                swap(array[j], array[j+1]);
            }
        }
    }
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end - start << " seconds\n";
}

void parallel_bubble_sort(const vector<int>& arr) {
    vector<int> array = arr;
    double start = omp_get_wtime();
    int size = array.size();
    for(int k = 0; k < size; k++) {
        if(k % 2 == 0) {
            #pragma omp parallel for
            for(int i = 1; i < size - 1; i += 2) {
                if(array[i] > array[i+1]) {
                    swap(array[i], array[i+1]);
                }
            }
        } else {
            #pragma omp parallel for
            for(int i = 0; i < size - 1; i += 2) {
                if(array[i] > array[i+1]) {
                    swap(array[i], array[i+1]);
                }
            }
        }
    }
    double end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << " seconds\n";
}

void merge(vector<int>& array, int low, int mid, int high) {
    vector<int> temp;
    int i = low, j = mid + 1;
    while(i <= mid && j <= high) {
        if(array[i] <= array[j])
            temp.push_back(array[i++]);
        else
            temp.push_back(array[j++]);
    }
    while(i <= mid) temp.push_back(array[i++]);
    while(j <= high) temp.push_back(array[j++]);

    for(int k = low; k <= high; ++k)
        array[k] = temp[k - low];
}

void mergesort(vector<int>& array, int low, int high) {
    if(low < high) {
        int mid = (low + high) / 2;
        mergesort(array, low, mid);
        mergesort(array, mid+1, high);
        merge(array, low, mid, high);
    }
}

void parallel_mergesort(vector<int>& array, int low, int high, int depth = 0) {
    if(low < high) {
        int mid = (low + high) / 2;
        if(depth <= 3) {  // limit recursion depth to avoid too many threads
            #pragma omp parallel sections
            {
                #pragma omp section
                parallel_mergesort(array, low, mid, depth + 1);

                #pragma omp section
                parallel_mergesort(array, mid+1, high, depth + 1);
            }
        } else {
            mergesort(array, low, mid);
            mergesort(array, mid+1, high);
        }
        merge(array, low, mid, high);
    }
}

void perform_merge_sort(const vector<int>& arr) {
    vector<int> array = arr;
    double start = omp_get_wtime();
    mergesort(array, 0, array.size()-1);
    double end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " seconds\n";
}

void perform_parallel_merge_sort(const vector<int>& arr) {
    vector<int> array = arr;
    double start = omp_get_wtime();
    parallel_mergesort(array, 0, array.size()-1);
    double end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " seconds\n";
}

int main() {
    omp_set_num_threads(8); // Adjust this based on your CPU cores

    int SIZE;
    cout << "Enter array size (>=10000 recommended): ";
    cin >> SIZE;

    vector<int> original_array(SIZE);
    for(int i = 0; i < SIZE; ++i)
        original_array[i] = rand() % 100000;

    cout << "\nSorting algorithms on array of size " << SIZE << "...\n\n";

    sequential_bubble_sort(original_array);
    parallel_bubble_sort(original_array);
    perform_merge_sort(original_array);
    perform_parallel_merge_sort(original_array);

    return 0;
}
