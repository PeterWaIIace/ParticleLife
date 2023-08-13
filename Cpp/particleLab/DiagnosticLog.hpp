#pragma once

#include <chrono>
#include <iostream>
#include <fstream>
#include <functional>

using namespace std::chrono;
using namespace std::chrono_literals;

inline long long timeit(std::function<void(void)> fn,int filter = 0, std::string name = "")
{
    auto start = high_resolution_clock::now();
    fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    if(duration.count() > filter)
        std::cout << name << duration.count() << "ms" << std::endl;
    return duration.count();
};

// TODO: add file saving