#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>

/*
* Author: Jaime Zafra Ruiz
* Date of implementation:  10/1/2023
* Algorithm: Campbell-Dudek-Smith’s
* C++ version: 17
*/

typedef struct job{
    size_t index{};
    std::vector<uint32_t> processingTime;
};

typedef struct johnsonJob{
    size_t index{};
    std::array<uint32_t, 2> processingTime;
};

typedef struct  schedule{
    size_t value{INT_MAX};
    std::vector<size_t> jobsIndexes;
};

template<typename T>
T readInput() {
    T obj{};
    std::cin >> obj;
    return obj;
}

auto readJobs(const size_t numMachines, const size_t numJobs) {

    std::vector<job> jobs(numJobs);

    for (size_t i = 0; i < numJobs; ++i) {
        std::cout << "Job[" << i << "]\n";
        jobs[i].index = i;
        jobs[i].processingTime = std::vector<uint32_t>(numMachines);
        
        std::cout << "\tProcessing Time:\n";
        for (size_t j = 0; j < numMachines; ++j) {
            std::cout << "\t\tM" << j << ": ";
            jobs[i].processingTime[j] = readInput<uint32_t>();
        }
        std::cout << '\n';
    }
    return jobs;
}

size_t criterionFunction(const std::vector<size_t>& jobsIndexes, const std::vector<job>& jobs) {

    size_t timeMachine1{0};
    size_t timeMachine2{0};

    for (const auto index : jobsIndexes) {

        timeMachine1 += jobs[index].processingTime.front();
        timeMachine2 = std::max(timeMachine1, timeMachine2);
        timeMachine2 += jobs[index].processingTime.back();
    }

    return timeMachine2;
}

schedule johnsonAlgorithm(const std::vector<johnsonJob>& johnsonJobs, const std::vector<job>& jobs) {

    std::vector<size_t> sequence1{};
    std::vector<size_t> sequence2{};
    for (size_t i = 0; i < johnsonJobs.size(); ++i) {
        if (johnsonJobs[i].processingTime.front() <= johnsonJobs[i].processingTime.back())
            sequence1.push_back(johnsonJobs[i].index);
        else
            sequence2.push_back(johnsonJobs[i].index);
    }
    std::sort(sequence1.begin(), sequence1.end(), [&](const auto& index1, const auto& index2) { return johnsonJobs[index1].processingTime.front() <= johnsonJobs[index2].processingTime.front(); });
    std::sort(sequence2.begin(), sequence2.end(), [&](const auto& index1, const auto& index2) { return johnsonJobs[index1].processingTime.back() >= johnsonJobs[index2].processingTime.back(); });
    
    sequence1.insert(sequence1.end(), sequence2.begin(), sequence2.end());

    const auto value = criterionFunction(sequence1, jobs);
    return { value, sequence1 };
}

schedule cdsAlgorithm(const std::vector<job>& jobs) {

    const auto numMachines = jobs.front().processingTime.size();
    schedule optimumSchedule{};

    for (size_t i = 0; i < numMachines - 1; ++i) {

        std::vector<johnsonJob> newJobs(jobs.size());

        for (size_t j = 0; j < jobs.size(); ++j) {
            
            newJobs[j].index = jobs[j].index;
            newJobs[j].processingTime.front() = std::accumulate(jobs[j].processingTime.begin(), jobs[j].processingTime.begin() + i + 1, 0);
            newJobs[j].processingTime.back() = std::accumulate(jobs[j].processingTime.rbegin(), jobs[j].processingTime.rbegin() + i + 1, 0);
        }

        const auto result = johnsonAlgorithm(newJobs, jobs);
        if (result.value < optimumSchedule.value) optimumSchedule = result;
    }
    return optimumSchedule;
}

void printSchedule(const schedule& schedule, const std::vector<job>& jobs) {

    const auto numMachines = jobs.front().processingTime.size();

    std::vector<uint32_t> timeMachines(jobs.size(), 0);

    std::cout << "Final schedule:\n";
    for (size_t i = 0; i < numMachines; ++i) {

        std::cout << "\tM" << i << ":\n";
        size_t time{ 0 };
        for (const auto jobIndex : schedule.jobsIndexes) {

            std::cout << "\t\tJ[" << jobIndex << "]:\n";
            if (time < timeMachines[jobIndex]) time = timeMachines[jobIndex];
            std::cout << "\t\t\tStarting time: " << time << '\n';
            time += jobs[jobIndex].processingTime[i];
            std::cout << "\t\t\tCompletion time: " << time << '\n';
            timeMachines[jobIndex] = time;
        }
    }
    std::cout << "\nValue of criterion function: " << timeMachines[schedule.jobsIndexes.back()] << '\n';
}

int main()
{
    std::cout << "This program will construct a schedule of jobs using Campbell-Dudek-Smiths algorithm.\n";
    std::cout << "Type the number of machines: ";
    const auto numMachines = readInput<size_t>();
    std::cout << "Type the number of jobs: ";
    const auto numJobs = readInput<size_t>();
    auto jobs = readJobs(numMachines, numJobs);

    const auto schedule = cdsAlgorithm(jobs);
    printSchedule(schedule, jobs);
}