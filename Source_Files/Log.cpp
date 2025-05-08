#include "Log.h"
#include <iostream>
#include <mutex>
#include <thread>

std::mutex consoleMutex;
thread_local std::string logBuffer;

void logMessage(const std::string& message) {
    logBuffer += "[LOG] " + message + "\n";
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << logBuffer;
    std::cout.flush(); // Ensure output is flushed
    logBuffer.clear();
}