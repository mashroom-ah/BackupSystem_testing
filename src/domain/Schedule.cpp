#include "domain/Schedule.h"

Schedule::Schedule(int intervalSeconds, bool enabled)
    : intervalSeconds(intervalSeconds), enabled(enabled),
    lastRun(std::chrono::system_clock::now()) {}

int Schedule::getInterval() const { return intervalSeconds; }

bool Schedule::isEnabled() const { return enabled; }

std::chrono::system_clock::time_point Schedule::getLastRun() const {
    return lastRun;
}

void Schedule::setEnabled(bool value) { enabled = value; }

void Schedule::updateLastRun(std::chrono::system_clock::time_point time) {
    lastRun = time;
}

bool Schedule::shouldRun(std::chrono::system_clock::time_point now) const {
    return enabled &&
           std::chrono::duration_cast<std::chrono::seconds>(now - lastRun).count() >= intervalSeconds;
}
