#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// Performance monitoring utility
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    Timer();
    ~Timer();
};

#endif // TIMER_H