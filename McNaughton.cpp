#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>

/*
* Author: Jaime Zafra Ruiz
* Date of implementation:  10/1/2023
* Algorithm: Mc Naughton's
* C++ version: 17
*/

typedef struct {
    size_t index;
    uint32_t processingTime;
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
        std::cout << '\n';
    }
    return jobs;
}

auto criterionFunction(const size_t numMachines, const std::vector<job>& jobs) {

    //Cmax* := max{ max{pj: 1≤j≤n}, (1 / m)Σpj }
    const auto maxProcessingTime = std::max_element(jobs.begin(), jobs.end(), [](const job& j1, const job& j2) {return j2.processingTime > j1.processingTime; })->processingTime;
    const uint32_t weightedTotalProcessingTime = std::accumulate(jobs.begin(), jobs.end(), 0, [](const auto prev, const job& j) { return prev + j.processingTime; }) / numMachines;
    return std::max(maxProcessingTime, weightedTotalProcessingTime);
}

size_t findMachine(const uint32_t Cmax, std::vector<std::vector<job>>& result) {
    
    for (size_t i = 0; i < result.size(); ++i) {
        if (std::accumulate(result[i].begin(), result[i].end(), 0, [](const auto prev, const job& job) { return prev + job.processingTime; }) < Cmax)
            return i;
    }
}

std::vector<std::vector<job>> McNaughton(const size_t numMachines, std::vector<job>& jobs) {

    std::vector<std::vector<job>> schedule(numMachines);
    const auto Cmax = criterionFunction(numMachines, jobs);

    for (size_t i = 0; i < jobs.size(); ++i) {

        const auto machineIndex = findMachine(Cmax, schedule);
        schedule[machineIndex].push_back(jobs[i]);

        const auto time = std::accumulate(schedule[machineIndex].begin(), schedule[machineIndex].end(), 0, 
            [](auto accum, const job& m) { return accum + m.processingTime; }
        );
        if (time <= Cmax) continue;

        const auto difference = time - Cmax;
        schedule[machineIndex].back().processingTime -= difference;
        jobs.insert(jobs.begin()+i+1, jobs[i]);
        jobs[i+1].processingTime = difference;
    }

    return schedule;
}

void printSchedule(const std::vector<std::vector<job>>& schedule) {

    std::cout << "Final schedule:\n";
    for (size_t i = 0; i < schedule.size(); ++i) {

        std::cout << "\tM" << i << ":\n";
        size_t time{ 0 };
        for (const auto& job : schedule[i]) {

            std::cout << "\t\tJ[" << job.index << "]:\n";
            std::cout << "\t\t\tStarting time: " << time << '\n';
            std::cout << "\t\t\tCompletion time: " << time + job.processingTime << '\n';
            time += job.processingTime;
        }
    }
}

int main()
{
    std::cout << "This program will construct a schedule of jobs using McNaughton’s algorithm.\n";
    std::cout << "Type the number of machines: ";
    const auto numMachines = readInput<size_t>();
    std::cout << "Type the number of jobs: ";
    const auto numJobs = readInput<size_t>();
    auto jobs = readJobs(numJobs);
    std::cout << "Value of criterion function: " << criterionFunction(numMachines, jobs) << '\n';

    const auto schedule = McNaughton(numMachines, jobs);
    printSchedule(schedule);
}