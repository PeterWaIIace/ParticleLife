#pragma once

#include <chrono>
#include <iostream>
#include <fstream>
#include <functional>

using namespace std::chrono;
using namespace std::chrono_literals;

// struct message{
//     // int messageType;
// };

// struct diagnosticLatencyMessage : public message
// {
//     steady_clock::time_point timestamp;
//     milliseconds latencyMS;
// };


void timeit(std::function<void(void)> fn,int filter = 0, std::string name = "")
{
    auto start = high_resolution_clock::now();
    fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    if(duration.count() > filter)
        std::cout << name << duration.count() << "ms" << std::endl;
};

// TODO: add file saving
