#include <iostream>
#include <omp.h>
#include <vector>
#include <climits>

using namespace std;

void minimum(vector<int> array)
{
    int min = INT_MAX;
    double startTime = omp_get_wtime();
    for (auto i = array.begin(); i != array.end(); i++)
    {
        if (*i < min)
            min = *i;
    }
    double endTime = omp_get_wtime();
    cout << "\n\n\nUsing Sequential ->";
    cout << "\n\nMinimum Element in Array: " << min;
    cout << "\n\nTime Taken using Sequential: " << endTime - startTime;

    int minElem = INT_MAX;
    double start = omp_get_wtime();
#pragma omp parallel for reduction(min : minElem)
    for (auto i = array.begin(); i != array.end(); i++)
    {
        if (*i < minElem)
            minElem = *i;
    }
    double end = omp_get_wtime();
    cout << "\n\n\nUsing Parallel Reduction ->";
    cout << "\n\nMinimum Element in Array: " << min;
    cout << "\n\nTime Taken using Parallel Redn.: " << end - start;
}

void maximum(vector<int> array)
{
    int max = INT_MIN;
    double startTime = omp_get_wtime();
    for (auto i = array.begin(); i != array.end(); i++)
    {
        if (*i > max)
            max = *i;
    }
    double endTime = omp_get_wtime();
    cout << "\n\n\nUsing Sequential ->";
    cout << "\n\nMaximum Element in Array: " << max;
    cout << "\n\nTime Taken using Sequential: " << endTime - startTime;

    int maxElem = INT_MIN;
    double start = omp_get_wtime();
#pragma omp parallel for reduction(max : maxElem)
    for (auto i = array.begin(); i != array.end(); i++)
    {
        if (*i > maxElem)
            maxElem = *i;
    }
    double end = omp_get_wtime();
    cout << "\n\n\nUsing Parallel Reduction ->";
    cout << "\n\nMaximum Element in Array: " << maxElem;
    cout << "\n\nTime Taken using Parallel Redn.: " << endTime - startTime;
}

void summation(vector<int> array)
{
    int sum = 0;
    double start = omp_get_wtime();
    for (auto i = array.begin(); i != array.end(); i++)
    {
        sum += *i;
    }
    double end = omp_get_wtime();
    cout << "\n\n\nUsing Sequential ->";
    cout << "\n\nSummation is: " << sum;
    cout << "\n\nTime Taken using Sequential: " << end - start;

    sum = 0;
    start = omp_get_wtime();
#pragma omp parallel for reduction(+ : sum)
    for (auto i = array.begin(); i != array.end(); i++)
    {
        sum += *i;
    }
    end = omp_get_wtime();
    cout << "\n\n\nUsing Parallel ->";
    cout << "\n\nSummation is: " << sum;
    cout << "\n\nTime Taken using Parallel: " << end - start;
}

void average(vector<int> array)
{
    int sum = 0;
    double start = omp_get_wtime();
    for (auto i = array.begin(); i != array.end(); i++)
    {
        sum += *i;
    }
    double end = omp_get_wtime();
    cout << "\n\n\nUsing Sequential ->";
    cout << "\n\nAverage is: " << sum / array.size();
    cout << "\n\nTime Taken using Sequential: " << end - start;

    sum = 0;
    start = omp_get_wtime();
#pragma omp parallel for reduction(+ : sum)
    for (auto i = array.begin(); i != array.end(); i++)
    {
        sum += *i;
    }
    end = omp_get_wtime();
    cout << "\n\n\nUsing Parallel ->";
    cout << "\n\nAverage is: " << sum / array.size();
    cout << "\n\nTime Taken using Parallel: " << end - start;
}

int main()
{
    int MAX = 1000;
    int N;
    cout << "Enter the Number of Elements in Array: ";
    cin >> N;

    vector<int> array;
    for (int i = 0; i < N; i++)
    {
        array.push_back(rand() % MAX);
    }

    minimum(array);
    maximum(array);
    summation(array);
    average(array);

    return 0;
}