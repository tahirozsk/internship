#include "EventProcessors.h"
#include "Timer.h"
#include "Log.h"
#include <iostream>
#include <string>

void processSummation(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing SUMMATION event with ID: " + std::to_string(e.id));
    int spacePos = e.instructions.find(' ');
    if (static_cast<size_t>(spacePos) == std::string::npos) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Event " << e.id << " (SUMMATION): Invalid instructions format" << std::endl;
        return;
    }

    std::string startStr = e.instructions.substr(0, spacePos);
    std::string endStr = e.instructions.substr(spacePos + 1);
    
    int start = std::stoi(startStr);
    int end = std::stoi(endStr);
    
    long long sum = 0;
    for (int i = start; i <= end; i++) {
        sum += i;
    }
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "Event " << e.id << " (SUMMATION): Sum from " << start 
         << " to " << end << " = " << sum << std::endl;
}

void processAlphabet(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing ALPHABET event with ID: " + std::to_string(e.id));
    int count = 0;
    for (char c : e.instructions) {
        count = count * 10 + (c - '0');
    }

    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "Event " << e.id << " (ALPHABET): Printing alphabet " 
         << count << " times" << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << "abcdefghijklmnopqrstuvwxyz" << std::endl;
    }
}

void processUnknown(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing UNKNOWN event with ID: " + std::to_string(e.id));
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "Event " << e.id << " (UNKNOWN): Event type '" << e.type 
         << "' not recognized. Instructions: " << e.instructions << std::endl;
}