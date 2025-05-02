#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include <omp.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

// Sequential Versions
int sequentialMin(const vector<int>& vec) {
    int min_val = vec[0];
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i] < min_val)
            min_val = vec[i];
    }
    return min_val;
}

int sequentialMax(const vector<int>& vec) {
    int max_val = vec[0];
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i] > max_val)
            max_val = vec[i];
    }
    return max_val;
}

int sequentialSum(const vector<int>& vec) {
    int sum = 0;
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum;
}

float sequentialAverage(const vector<int>& vec) {
    int sum = sequentialSum(vec);
    return float(sum) / vec.size();
}

// ==========================================================================================

// Parallel Versions
int parallelMin(const vector<int>& vec) {
    int min_val = vec[0];
    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] < min_val)
            min_val = vec[i];
    }
    return min_val;
}

int parallelMax(const vector<int>& vec) {
    int max_val = vec[0];
    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > max_val)
            max_val = vec[i];
    }
    return max_val;
}

int parallelSum(const vector<int>& vec) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum;
}

float parallelAverage(const vector<int>& vec) {
    int sum = parallelSum(vec);
    return float(sum) / vec.size();
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> vec(n);

    srand(time(0));
    for (int i = 0; i < n; ++i) {
        vec[i] = rand() % 1000000;
    }

    cout << "Vector contains the elements: ";
    for(auto i:vec){
        cout<<i<<" ";
    }
    cout<<endl;

    // Sequential timing
    auto start_seq = high_resolution_clock::now();
    int min_seq = sequentialMin(vec);
    int max_seq = sequentialMax(vec);
    int sum_seq = sequentialSum(vec);
    float avg_seq = sequentialAverage(vec);
    auto end_seq = high_resolution_clock::now();
    auto time_seq = duration_cast<milliseconds>(end_seq - start_seq);

    // Parallel timing
    auto start_par = high_resolution_clock::now();
    int min_par = parallelMin(vec);
    int max_par = parallelMax(vec);
    int sum_par = parallelSum(vec);
    float avg_par = parallelAverage(vec);
    auto end_par = high_resolution_clock::now();
    auto time_par = duration_cast<milliseconds>(end_par - start_par);

    // Output results
    cout << "\n--- Sequential Results ---" << endl;
    cout << "Min: " << min_seq << ", Max: " << max_seq << ", Sum: " << sum_seq << ", Avg: " << avg_seq << endl;
    cout << "Time taken (Sequential): " << time_seq.count() << " ms" << endl;

    cout << "\n--- Parallel Results ---" << endl;
    cout << "Min: " << min_par << ", Max: " << max_par << ", Sum: " << sum_par << ", Avg: " << avg_par << endl;
    cout << "Time taken (Parallel): " << time_par.count() << " ms" << endl;

    return 0;
}
