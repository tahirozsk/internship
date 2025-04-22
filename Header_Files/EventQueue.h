#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Event.h"

class EventQueue {
private:
    std::queue<Event> events;
    std::mutex m;
    std::condition_variable cv;
    bool stop = false;

public:
    void addEvent(Event e);
    bool getEvent(Event& e);
    void setStop();
};

#endif // EVENT_QUEUE_H