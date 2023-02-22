#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>

/*
* Author: Jaime Zafra Ruiz
* Date of implementation:  7/1/2023
* Algorithm: WSPT
* C++ version: 17
*/

typedef struct {
    size_t index;
    uint32_t processingTime;
    uint32_t weight;
} job;

template<typename T>
T readInput() {
    T obj{};
    std::cin >> obj;
    return obj;
}

auto readJobs(size_t numJobs) {

    std::vector<job> jobs(numJobs);

    for (size_t i = 0; i < numJobs; ++i) {
        std::cout << "Job[" << i << "]\n";
        jobs[i].index = i;
        std::cout << "\tProcessing Time: ";
        jobs[i].processingTime = readInput<uint32_t>();
        std::cout << "\tWeight: ";
        jobs[i].weight = readInput<uint32_t>();
        std::cout << '\n';
    }
    return jobs;
}

bool wspt(const job& j1, const job& j2) {
    return (j1.processingTime * j2.weight < j2.processingTime * j1.weight);
}

//ΣwjCj
float criterionFunction(const std::vector<job>& jobs) {
    
    uint32_t time{ 0 };
    float res{ 0 };
    for (const auto& job : jobs) {
        time += job.processingTime;
        res += job.weight * time;
    }
    return res;
}

int main()
{
    std::cout << "This program will construct a schedule of jobs using WSPT rule.\n";
    std::cout << "Type the number of jobs: ";
    const auto numJobs = readInput<size_t>();
    auto jobs = readJobs(numJobs);

    //std::sort which manages to sort the vector in O(nlogn): https://en.cppreference.com/w/cpp/algorithm/sort
    //(It uses quickSort and/or mergeSort)
    std::sort(jobs.begin(), jobs.end(), wspt);

    std::cout << "Final schedule:\n\tM0: ( ";
    for (const auto j : jobs)
        std::cout << 'J' << j.index << ' ';
    std::cout << ")\n";
    std::cout << "Value of criterion function: " << criterionFunction(jobs);
}