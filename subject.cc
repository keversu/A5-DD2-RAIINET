#include "subject.h"
#include "observer.h"
#include <algorithm>

void Subject::attach(std::shared_ptr<Observer> o) {
    observers.push_back(o); // Share ownership of the observer
}

void Subject::detach(std::shared_ptr<Observer> o) {
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void Subject::notifyObservers() const {
    for (const auto& o : observers) {
        o->notify(); // Notify each observer
    }
}

