#include<iostream>
#include<omp.h>
#include<vector>
#include<climits>
#include<cstdlib> // for rand()

using namespace std;

void minimum(vector<int> array){
    int min = INT_MAX;
    double start = omp_get_wtime();
    for(int i = 0; i < array.size(); i++){
        if(array[i] < min){
            min = array[i];
        } 
    }
    double end = omp_get_wtime();
    cout << endl << "Minimum Element: " << min << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;

    int min_ele = INT_MAX;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(min: min_ele)
    for(int i = 0; i < array.size(); i++){
        if(array[i] < min_ele){
            min_ele = array[i];
        } 
    }
    end = omp_get_wtime();
    cout << endl << "Minimum Element (Parallel Reduction): " << min_ele << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;
}

void maximum(vector<int> array){
    int max = INT_MIN;
    double start = omp_get_wtime();
    for(int i = 0; i < array.size(); i++){
        if(array[i] > max){
            max = array[i];
        } 
    }
    double end = omp_get_wtime();
    cout << endl << "Maximum Element: " << max << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;

    int max_ele = INT_MIN;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(max: max_ele)
    for(int i = 0; i < array.size(); i++){
        if(array[i] > max_ele){
            max_ele = array[i];
        } 
    }
    end = omp_get_wtime();
    cout << endl << "Maximum Element (Parallel Reduction): " << max_ele << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;
}

void sum(vector<int> array){
    int total = 0;
    double start = omp_get_wtime();
    for(int i = 0; i < array.size(); i++){
        total += array[i]; 
    }
    double end = omp_get_wtime();
    cout << endl << "Summation: " << total << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;

    total = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+: total)
    for(int i = 0; i < array.size(); i++){
        total += array[i]; 
    }
    end = omp_get_wtime();
    cout << endl << "Summation (Parallel Reduction): " << total << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;
}

void average(vector<int> array){
    float avg = 0;
    double start = omp_get_wtime();
    for(int i = 0; i < array.size(); i++){
        avg += array[i]; 
    }
    double end = omp_get_wtime();
    cout << endl << "Average: " << avg / array.size() << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;

    avg = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+: avg)
    for(int i = 0; i < array.size(); i++){
        avg += array[i]; 
    }
    end = omp_get_wtime();
    cout << endl << "Average (Parallel Reduction): " << avg / array.size() << endl;
    cout << "Time Taken: " << (end-start) << " seconds" << endl;
}

int main(){
    cout << "Enter number of elements in array: ";
    int N;
    cin >> N;

    const int MAX = 1000;
    vector<int> array;
    for(int i = 0; i < N; i++){
        array.push_back(rand() % MAX);
    }

    minimum(array);
    maximum(array);
    sum(array);
    average(array);

    return 0;
}
