#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
using namespace std;

void computeAll(const vector<long long>& arr) {
    double start, end;
    long long minVal, maxVal, sumVal;
    double avgVal;

    // Sequential
    start = omp_get_wtime();
    minVal = maxVal = arr[0]; sumVal = 0;
    for (auto val : arr) {
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
        sumVal += val;
    }
    avgVal = (double)sumVal / arr.size();
    end = omp_get_wtime();
    cout << "--- Sequential ---\n";
    cout << "Min: " << minVal << ", Max: " << maxVal << ", Sum: " << sumVal << ", Avg: " << avgVal << endl;
    cout << "Time: " << end - start << " sec\n\n";

    // Parallel
    start = omp_get_wtime();
    minVal = maxVal = arr[0]; sumVal = 0;
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sumVal)
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
        sumVal += arr[i];
    }
    avgVal = (double)sumVal / arr.size();
    end = omp_get_wtime();
    cout << "--- Parallel ---\n";
    cout << "Min: " << minVal << ", Max: " << maxVal << ", Sum: " << sumVal << ", Avg: " << avgVal << endl;
    cout << "Time: " << end - start << " sec\n";
}

int main() {
    cout << "Enter array size: ";
    long long N;
    cin >> N;
    vector<long long> arr;
    arr.reserve(N);
    for (int i = 0; i < N; ++i)
        arr.push_back(rand() % 1000);

    computeAll(arr);
    return 0;
}
