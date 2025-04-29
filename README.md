# Dynamic Event Processing System

A modern, menu-driven C++ event processing application that allows you to easily add and handle new event types. The system is designed for flexibility, clarity, and extensibility.

## Features

- **Dynamic Event Handling:**  
  Register new event types and handlers with a single line of code. The menu updates automatically.
- **Menu-Driven User Interface:**  
  Users can select event types and input instructions interactively.
- **Extensible Design:**  
  Add new event types by simply creating a handler function and registering it.
- **Logging and Performance Monitoring:**  
  All actions are logged, and event processing time is measured.

## How It Works

1. The program displays a menu of available event types.
2. The user selects an event type and enters instructions.
3. The corresponding handler function processes the event and displays output.
4. The menu reappears for further input or exit.

## Adding a New Event Type

1. **Define a Handler Function:**  
   In `EventProcessors.cpp` and declare it in `EventProcessors.h`, e.g.:
   ```cpp
   void processMyEvent(const Event& e) {
       // Your logic here
   }
   ```
2. **Register the Handler:**  
   In `main.cpp`, add:
   ```cpp
   eventHandlers["MY_EVENT"] = processMyEvent;
   ```
   That's it! "MY_EVENT" will appear in the menu.

## Build & Run (Windows)

1. Open a terminal (PowerShell or Command Prompt).
2. Navigate to the project directory.
3. Build using:
   ```sh
   make
   ```
4. Run the application:
   ```sh
   .\bin\event_processor
   ```

## Example Usage

```
Choose event type:
1. ALPHABET
2. SUMMATION
3. Exit
Enter choice (1-3): 1
Enter instructions: 2
[LOG] Dispatching event with ID: 1
[LOG] Processing ALPHABET event with ID: 1
Event 1 (ALPHABET): Printing alphabet 2 times
abcdefghijklmnopqrstuvwxyz
abcdefghijklmnopqrstuvwxyz
[PERFORMANCE] Task completed in 0 ms

Choose event type:
1. ALPHABET
2. SUMMATION
3. Exit
Enter choice (1-3): 3
[LOG] All events processed. Program terminated.
```

## Project Structure

```
internship/
├── Header_Files/         # Header files (Event, EventQueue, etc.)
├── Source_Files/         # Source code (.cpp files)
├── bin/                  # Compiled executable
├── obj/                  # Object files
├── README.md
├── makefile
└── .gitignore
```

## Contributing

1. Fork the repository.
2. Create a new branch:
   ```sh
   git checkout -b feature/your-feature
   ```
3. Make your changes and commit:
   ```sh
   git commit -m "Describe your change"
   ```
4. Push and open a pull request.

---

**Note:**  
This project is designed for clarity and extensibility. For advanced use cases, you can re-enable multithreading or event queueing as needed.

---
