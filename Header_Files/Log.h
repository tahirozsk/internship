#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <mutex>

extern std::mutex consoleMutex;

// Logging utility
void logMessage(const std::string& message);

#endif // UTILS_H