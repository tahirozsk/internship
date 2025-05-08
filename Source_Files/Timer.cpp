#include "Timer.h"
#include "Log.h"
#include <iostream>
#include <chrono>

Timer::Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

Timer::~Timer() { 
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    if (duration == 0) duration = 1; // Ensure non-zero display
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "[PERFORMANCE] Task completed in " << duration << " ms" << std::endl;
}