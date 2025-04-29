#include "Event.h"
#include "EventQueue.h"
#include "ThreadPool.h"
#include "Log.h"
#include "EventProcessors.h"
#include <iostream>
#include <thread>
#include <string>
#include <unordered_map>
#include <functional>

int main() {
    logMessage("Program started");
    EventQueue eq;
    ThreadPool pool(4); 

    // Map event types to their handlers
    std::unordered_map<std::string, std::function<void(const Event&)>> eventHandlers;
    eventHandlers["SUMMATION"] = processSummation;
    eventHandlers["ALPHABET"] = processAlphabet;
    eventHandlers["UNKNOWN"] = processUnknown;

    auto worker = [&eq, &pool, &eventHandlers]() {
        while (true) {
            Event e;
            if (!eq.getEvent(e)) {
                break;
            }

            logMessage("Dispatching event with ID: " + std::to_string(e.id));

            auto it = eventHandlers.find(e.type);
            if (it != eventHandlers.end()) {
                pool.enqueue([handler=it->second, e]() { handler(e); });
            } else {
                pool.enqueue([handler=eventHandlers["UNKNOWN"], e]() { handler(e); });
            }
        }
    };

    std::thread eventDispatcher(worker); // Start the event dispatcher thread

    // Dynamic event with user input
    int eventId = 1; 
    std::string type, instructions;

    int choice;
    do {
        // Display options
        std::cout << "\nChoose event type:\n";
        // Dynamically display available event types
        int idx = 1;
        std::vector<std::string> types;
        for (const auto& pair : eventHandlers) {
            if (pair.first != "UNKNOWN") {
                std::cout << idx << ". " << pair.first << "\n";
                types.push_back(pair.first);
                idx++;
            }
        }
        std::cout << idx << ". Exit\n";
        std::cout << "Enter choice (1-" << idx << "): ";

        std::cin >> choice;

        if (choice == idx) break; // Exit input loop

        if (choice > 0 && choice < idx) {
            type = types[choice - 1];
            std::cout << "Enter instructions: ";
            std::cin.ignore(); // Clear newline left in input buffer
            std::getline(std::cin, instructions); // Get full instruction line
            eq.addEvent(Event{eventId++, type, instructions});
        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    } while (true);

    // --- Cleanup ---
    eq.setStop();           // Signal no more events will be added
    eventDispatcher.join(); // Wait for event dispatcher to finish
    logMessage("All events processed. Program terminated.");
    return 0;
}