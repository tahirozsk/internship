#include "Log.h"
#include <iostream>
#include <mutex>

std::mutex consoleMutex;

void logMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "[LOG] " << message << std::endl;
}