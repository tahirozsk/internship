# Event Processing System

A C++ multithreaded event processing system using a thread-safe queue. The system allows parallel processing of different types of events, ensuring safe and efficient execution.

## Features

### Event Handling
- **Summation Events**: Computes the sum of numbers within a given range.
- **Alphabet Events**: Prints the alphabet a specified number of times.

### Thread Management
- Utilizes `std::thread` for parallel processing.
- Synchronization with `mutex` and `condition_variable` to ensure thread safety.

### Queue System
- Thread-safe event queue for managing tasks.
- Graceful termination with a stop flag to ensure safe exit.

## Installation

Clone the repository:
```
git clone <repository-url>
cd event_processing_system
```

Compile the program:
```
g++ -std=c++11 -pthread main.cpp -o event_processor
```

Run the application:
```
./event_processor
```

## Example Output
```
Event 1 (SUMMATION): Sum from 1 to 10 = 55
Event 2 (ALPHABET): Printing alphabet 2 times
abcdefghijklmnopqrstuvwxyz
abcdefghijklmnopqrstuvwxyz
Event 3 (SUMMATION): Sum from 5 to 15 = 110
All events processed. Program terminated.
```

## Project Structure
```
event_processing_system/
├── src/
│   ├── main.cpp
│   ├── event_queue.h
│   ├── event_queue.cpp
│   ├── workers.h
│   ├── workers.cpp
├── build/
├── .gitignore
├── README.md
└── Makefile
```

## Security
- Thread-safe queue implementation.
- Proper synchronization to prevent race conditions.

## Contributing
1. Fork the repository.
2. Create a new branch.
3. Make your changes.
4. Submit a pull request.

## License
This project is licensed under the MIT License - see the LICENSE file for details.