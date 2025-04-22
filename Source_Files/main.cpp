#include "Event.h"
#include "EventQueue.h"
#include "ThreadPool.h"
#include "Log.h"
#include "EventProcessors.h"
#include <iostream>
#include <thread>
#include <string>

int main() {
    logMessage("Program started");
    EventQueue eq;
    ThreadPool pool(4); 

    auto worker = [&eq, &pool]() {
        while (true) {
            Event e;
            if (!eq.getEvent(e)) {
                break;
            }

            logMessage("Dispatching event with ID: " + std::to_string(e.id));
            
            if (e.type == "SUMMATION") {
                pool.enqueue([e]() { processSummation(e); });
            } else if (e.type == "ALPHABET") {
                pool.enqueue([e]() { processAlphabet(e); });
            } else {
                // Handle unknown event types using the default worker
                pool.enqueue([e]() { processUnknown(e); });
            }
        }
    };

    std::thread eventDispatcher(worker); // Start the event dispatcher thread

    // Dynamic event with user input
    int eventId = 1; 
    std::string type, instructions;

    while (true) {
        // Display options
        std::cout << "\nChoose event type:\n";
        std::cout << "1. SUMMATION\n";
        std::cout << "2. ALPHABET\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter choice (1-3): ";

        int choice;
        std::cin >> choice;

        if (choice == 3) break; // Exit input loop

        if (choice == 1) {
            type = "SUMMATION";
            std::cout << "Enter range (e.g., 1 10): ";
        } else if (choice == 2) {
            type = "ALPHABET";
            std::cout << "Enter count (e.g., 2): ";
        } else {
            std::cout << "Invalid choice. Try again.\n";
            continue;
        }

        std::cin.ignore(); // Clear newline left in input buffer
        std::getline(std::cin, instructions); // Get full instruction line

        // Create and queue the event
        eq.addEvent(Event{eventId++, type, instructions});
    }

    // --- Cleanup ---
    eq.setStop();           // Signal no more events will be added
    eventDispatcher.join(); // Wait for event dispatcher to finish
    logMessage("All events processed. Program terminated.");
    return 0;
}