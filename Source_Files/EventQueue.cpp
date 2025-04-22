#include "EventQueue.h"

void EventQueue::addEvent(Event e) {
    std::lock_guard<std::mutex> lock(m);
    events.push(e);
    cv.notify_one();
}

bool EventQueue::getEvent(Event& e) {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() { return !events.empty() || stop; });

    if (events.empty()) {
        return false;
    }

    e = events.front();
    events.pop();
    return true;
}

void EventQueue::setStop() {
    std::lock_guard<std::mutex> lock(m);
    stop = true;
    cv.notify_all();
}