#include <iostream>
 #include <queue>
 #include <thread>
 #include <mutex>
 #include <condition_variable>
 #include <string>
 
 using namespace std;
 
 // Mutex for console output to prevent interleaved printing
 mutex consoleMutex;
 
 // Event structure
 struct Event {
     int id;
     string type;
     string instructions;
 };
 
 // Thread-safe EventQueue class (simpler version)
 class EventQueue {
 private:
     queue<Event> events;
     mutex m;
     condition_variable cv;
     bool stop = false; // Initialize stop flag directly
 
 public:
     void addEvent(Event e) {
         lock_guard<mutex> lock(m); // Lock the mutex
         events.push(e);
         cv.notify_one(); // Notify one waiting thread
     }
 
     bool getEvent(Event& e) {
         unique_lock<mutex> lock(m); // Lock the mutex
         // Wait until there are events or the queue is stopped
         cv.wait(lock, [this]() { return !events.empty() || stop; });
 
         if (events.empty()) {
             return false; // Queue is empty and stopped
         }
 
         e = events.front(); // Get the front event
         events.pop();       // Remove it from the queue
         return true;
     }
 
     void setStop() {
         lock_guard<mutex> lock(m); // Lock the mutex
         stop = true;
         cv.notify_all(); // Notify all waiting threads
     }
 };
 
 // Worker function for summation task
 void summationWorker(EventQueue& eq) {
     while (true) {
         Event e;
         if (!eq.getEvent(e)) {
             break; // Exit if queue is empty and stopped
         }
 
         if (e.type == "SUMMATION") {
             // Parse instructions for start and end (format: "start end")
             int spacePos = e.instructions.find(' ');
             if (spacePos == string::npos) {
                 lock_guard<mutex> lock(consoleMutex); // Lock console output
                 cout << "Event " << e.id << " (SUMMATION): Invalid instructions format" << endl;
                 continue;
             }
 
             string startStr = e.instructions.substr(0, spacePos);
             string endStr = e.instructions.substr(spacePos + 1);
 
             // Convert strings to integers manually
             int start = 0, end = 0;
             for (char c : startStr) {
                 start = start * 10 + (c - '0');
             }
             for (char c : endStr) {
                 end = end * 10 + (c - '0');
             }
 
             long long sum = 0;
             for (int i = start; i <= end; i++) {
                 sum += i;
             }
 
             // Lock console output to prevent interleaving
             lock_guard<mutex> lock(consoleMutex);
             cout << "Event " << e.id << " (SUMMATION): Sum from " << start 
                  << " to " << end << " = " << sum << endl;
         } else {
             // If this is not a SUMMATION event, put it back in the queue
             eq.addEvent(e);
         }
     }
 }
 
 // Worker function for alphabet printing task
 void alphabetWorker(EventQueue& eq) {
     while (true) {
         Event e;
         if (!eq.getEvent(e)) {
             break; // Exit if queue is empty and stopped
         }
 
         if (e.type == "ALPHABET") {
             // Directly convert instructions string to integer (assuming valid input)
             int count = 0;
             for (char c : e.instructions) {
                 count = count * 10 + (c - '0');
             }
 
             // Lock console output to prevent interleaving
             lock_guard<mutex> lock(consoleMutex);
             cout << "Event " << e.id << " (ALPHABET): Printing alphabet " 
                  << count << " times" << endl;
             for (int i = 0; i < count; i++) {
                 cout << "abcdefghijklmnopqrstuvwxyz" << endl;
             }
         } else {
             // If this is not an ALPHABET event, put it back in the queue
             eq.addEvent(e);
         }
     }
 }
 
 int main() {
     EventQueue eq;
     
     // Start worker threads
     thread summationThread(summationWorker, ref(eq));
     thread alphabetThread(alphabetWorker, ref(eq));
 
     // Create and add sample events
     // Summation event: sum numbers from 1 to 10
     Event e1{1, "SUMMATION", "1 10"};
     eq.addEvent(e1);
 
     // Alphabet event: print alphabet 2 times
     Event e2{2, "ALPHABET", "2"};
     eq.addEvent(e2);
 
     // Summation event: sum numbers from 5 to 15
     Event e3{3, "SUMMATION", "5 15"};
     eq.addEvent(e3);
 
     // Wait for some time to allow processing
     this_thread::sleep_for(chrono::seconds(1));
 
     // Stop the queue and wait for threads to finish
     eq.setStop();
     summationThread.join();
     alphabetThread.join();
 
     // Lock console output for final message
     lock_guard<mutex> lock(consoleMutex);
     cout << "All events processed. Program terminated." << endl;
 
     return 0;
 }