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
        logMessage("Event " + std::to_string(e.id) + " (SUMMATION): Invalid instructions format");
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
    logMessage("Event " + std::to_string(e.id) + " (SUMMATION): Sum from " + std::to_string(start) 
               + " to " + std::to_string(end) + " = " + std::to_string(sum));
}

void processAlphabet(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing ALPHABET event with ID: " + std::to_string(e.id));
    int count = 0;
    for (char c : e.instructions) {
        count = count * 10 + (c - '0');
    }

    logMessage("Event " + std::to_string(e.id) + " (ALPHABET): Printing alphabet " 
               + std::to_string(count) + " times");
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < count; i++) {
        logMessage(alphabet);
    }
}

void processUnknown(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing UNKNOWN event with ID: " + std::to_string(e.id));
    logMessage("Event " + std::to_string(e.id) + " (UNKNOWN): Event type '" + e.type 
               + "' not recognized. Instructions: " + e.instructions);
}