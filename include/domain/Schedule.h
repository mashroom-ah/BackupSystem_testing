#pragma once

#include <chrono>

class Schedule {
private:
    int intervalSeconds;
    bool enabled;
    std::chrono::system_clock::time_point lastRun;

public:
    Schedule(int intervalSeconds = 0, bool enabled = false);

    int getInterval() const;
    bool isEnabled() const;

    std::chrono::system_clock::time_point getLastRun() const;

    void setEnabled(bool value);
    void updateLastRun(std::chrono::system_clock::time_point time);

    bool shouldRun(std::chrono::system_clock::time_point now) const;
};
