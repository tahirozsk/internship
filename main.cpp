#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

using namespace std;

mutex consoleMutex;

// Logging utility
void logMessage(const string& message) {
    lock_guard<mutex> lock(consoleMutex);
    cout << "[LOG] " << message << endl;
}

// Performance monitoring utility
class Timer {
private:
    chrono::high_resolution_clock::time_point start_time;

public:
    Timer() : start_time(chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        lock_guard<mutex> lock(consoleMutex);
        cout << "[PERFORMANCE] Task completed in " << duration << " ms" << endl;
    }
};

struct Event {
    int id;
    string type;
    string instructions;
};

class EventQueue {
private:
    queue<Event> events;
    mutex m;
    condition_variable cv;
    bool stop = false;

public:
    void addEvent(Event e) {
        lock_guard<mutex> lock(m);
        events.push(e);
        cv.notify_one();
    }

    bool getEvent(Event& e) {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this]() { return !events.empty() || stop; });

        if (events.empty()) {
            return false;
        }

        e = events.front();
        events.pop();
        return true;
    }

    void setStop() {
        lock_guard<mutex> lock(m);
        stop = true;
        cv.notify_all();
    }
};

class ThreadPool {
private:
    vector<thread> threads_;
    queue<function<void()> > tasks_;
    mutex queue_mutex_;
    condition_variable cv_;
    bool stop_ = false;

public:
    ThreadPool(size_t num_threads = thread::hardware_concurrency()) {
        for (size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(queue_mutex_);
                        cv_.wait(lock, [this] {
                            return !tasks_.empty() || stop_;
                        });

                        if (stop_ && tasks_.empty()) {
                            return;
                        }

                        task = move(tasks_.front());
                        tasks_.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queue_mutex_);
            stop_ = true;
        }

        cv_.notify_all();

        for (auto& thread : threads_) {
            thread.join();
        }
    }

    void enqueue(function<void()> task) {
        {
            unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace(move(task));
        }
        cv_.notify_one();
    }
};

void processSummation(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing SUMMATION event with ID: " + to_string(e.id));
    int spacePos = e.instructions.find(' ');
    if (spacePos == string::npos) {
        lock_guard<mutex> lock(consoleMutex);
        cout << "Event " << e.id << " (SUMMATION): Invalid instructions format" << endl;
        return;
    }

    string startStr = e.instructions.substr(0, spacePos);
    string endStr = e.instructions.substr(spacePos + 1);
    
    int start = stoi(startStr);
    int end = stoi(endStr);
    
    long long sum = 0;
    for (int i = start; i <= end; i++) {
        sum += i;
    }
    lock_guard<mutex> lock(consoleMutex);
    cout << "Event " << e.id << " (SUMMATION): Sum from " << start 
         << " to " << end << " = " << sum << endl;
}

void processAlphabet(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing ALPHABET event with ID: " + to_string(e.id));
    int count = 0;
    for (char c : e.instructions) {
        count = count * 10 + (c - '0');
    }

    lock_guard<mutex> lock(consoleMutex);
    cout << "Event " << e.id << " (ALPHABET): Printing alphabet " 
         << count << " times" << endl;
    for (int i = 0; i < count; i++) {
        cout << "abcdefghijklmnopqrstuvwxyz" << endl;
    }
}

// New default worker function for unknown event types
void processUnknown(Event e) {
    Timer timer; // Start performance monitoring
    logMessage("Processing UNKNOWN event with ID: " + to_string(e.id));
    lock_guard<mutex> lock(consoleMutex);
    cout << "Event " << e.id << " (UNKNOWN): Event type '" << e.type 
         << "' not recognized. Instructions: " << e.instructions << endl;
}

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

            logMessage("Dispatching event with ID: " + to_string(e.id));
            
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

  
    thread eventDispatcher(worker);

    
    Event e1{1, "SUMMATION", "1 10"};
    eq.addEvent(e1);

    Event e2{2, "ALPHABET", "2"};
    eq.addEvent(e2);

    Event e3{3, "SUMMATION", "5 15"};
    eq.addEvent(e3);

    
    Event e4{4, "INVALID_TYPE", "test"};
    eq.addEvent(e4);


    this_thread::sleep_for(chrono::seconds(1));


    eq.setStop();

    // Wait for event dispatcher to finish
    eventDispatcher.join();

    logMessage("All events processed. Program terminated.");
    return 0;
}