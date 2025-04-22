#ifndef EVENT_H
#define EVENT_H

#include <string>

// Event structure for tasks to be processed
struct Event {
    int id;
    std::string type;
    std::string instructions;
};

#endif // EVENT_H