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
#include <limits> // For std::numeric_limits

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
            // Wait for event to be processed before allowing menu redraw
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    };

    std::thread eventDispatcher(worker); // Start the event dispatcher thread

    // Dynamic event with user input
    int eventId = 1; 
    std::string type, instructions;

    int choice;
    do {
        // Declare idx and types outside the lock block
        int idx = 1;
        std::vector<std::string> types;
        // Synchronize menu display
        {
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cout << "\nChoose event type:\n";
            for (const auto& pair : eventHandlers) {
                if (pair.first != "UNKNOWN") {
                    std::cout << idx << ". " << pair.first << "\n";
                    types.push_back(pair.first);
                    idx++;
                }
            }
            std::cout << idx << ". Exit\n";
            std::cout << "Enter choice (1-" << idx << "): ";
            std::cout.flush(); // Ensure prompt is displayed immediately
        }

        // Clear input buffer thoroughly before reading choice
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cout << "Invalid input. Enter choice (1-" << idx << "): ";
        }

        if (choice == idx) break; // Exit input loop

        if (choice > 0 && choice < idx) {
            type = types[choice - 1];
            {
                std::lock_guard<std::mutex> lock(consoleMutex);
                std::cout << "Enter instructions: ";
                std::cout.flush();
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer before getline
            std::getline(std::cin, instructions); // Get full instruction line
            eq.addEvent(Event{eventId++, type, instructions});
            // Wait for event processing to complete before next prompt
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
        } else {
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cout << "Invalid choice. Try again.\n";
        }
    } while (true);

    // --- Cleanup ---
    eq.setStop();           // Signal no more events will be added
    eventDispatcher.join(); // Wait for event dispatcher to finish
    logMessage("All events processed. Program terminated.");
    return 0;
}