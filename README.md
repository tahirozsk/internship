# Event Processing System

A C++ multithreaded event processing system using a thread-safe queue and a thread pool. The system enables parallel processing of various event types with efficient task distribution, ensuring safe and scalable execution.

## Features

### Event Handling
- **Summation Events**: Computes the sum of numbers within a specified range (e.g., "1 10" sums numbers from 1 to 10).
- **Alphabet Events**: Prints the alphabet a specified number of times (e.g., "2" prints it twice).
- **Unknown Event Types**: Gracefully handles unrecognized event types by logging them with a default worker.

### Thread Management
- **Thread Pool**: Uses a dynamic thread pool initialized with the number of hardware threads (`std::thread::hardware_concurrency()` by default) for efficient task distribution.
- **Synchronization**: Employs `std::mutex` and `std::condition_variable` for thread-safe operations.

### Queue System
- **Thread-Safe Event Queue**: Manages events with a producer-consumer pattern using `EventQueue`.
- **Graceful Shutdown**: Supports a stop flag to terminate the system safely, ensuring all threads complete their tasks.

## Installation

Clone the repository:
```sh
git clone https://github.com/tahirozsk/internship.git
cd internship
```

Compile the program (requires a C++11-compliant compiler):
```sh
g++ -std=c++11 -pthread main.cpp -o event_processor
```

Run the application:
```sh
./event_processor
```

## Example Output
```sh
Event 1 (SUMMATION): Sum from 1 to 10 = 55
Event 2 (ALPHABET): Printing alphabet 2 times
abcdefghijklmnopqrstuvwxyz
abcdefghijklmnopqrstuvwxyz
Event 3 (SUMMATION): Sum from 5 to 15 = 110
Event 4 (UNKNOWN): Event type 'INVALID_TYPE' not recognized. Instructions: test
All events processed. Program terminated.
```

## Project Structure
```
internship/
├── src/
│   ├── main.cpp          # Main implementation with EventQueue and ThreadPool
├── build/                # Directory for compiled binaries (optional)
├── .gitignore
├── README.md             # This file
└── Makefile              # Optional: Add for easier compilation
```

### Notes on Structure
The provided code is contained in a single `main.cpp` file for simplicity. In a larger project, consider separating `EventQueue`, `ThreadPool`, and worker functions into dedicated header and source files (e.g., `event_queue.h`, `thread_pool.h`, `workers.h`).

## Usage
The system processes events added to the `EventQueue`. Each event consists of:
- **id**: Unique identifier (integer).
- **type**: Event type (string, e.g., "SUMMATION", "ALPHABET").
- **instructions**: Event-specific instructions (string).

Events are dispatched to a thread pool, which assigns tasks to available threads. Example events from the code:
- `Event{1, "SUMMATION", "1 10"}`: Sums numbers from 1 to 10.
- `Event{2, "ALPHABET", "2"}`: Prints the alphabet twice.
- `Event{4, "INVALID_TYPE", "test"}`: Logs an unrecognized event type.

## Security
- **Thread Safety**: Uses `std::mutex` and `std::lock_guard` to prevent race conditions when accessing shared resources (e.g., console output, queue).
- **Resource Management**: Thread pool ensures proper cleanup of threads on program termination via destructor.

## Contributing
1. Fork the repository.
2. Create a new branch:
   ```sh
   git checkout -b feature/your-feature
   ```
3. Make your changes and commit them:
   ```sh
   git commit -m "Add your message"
   ```
4. Push to your branch:
   ```sh
   git push origin feature/your-feature
   ```
5. Submit a pull request.

