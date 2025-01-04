#include "timer.h"



//====================================================
Timer::Timer() : running(false) {}

void Timer::start() {
    startTime = std::chrono::steady_clock::now();
    running = true;
}
//====================================================
void Timer::stop() {
    if (running) {
        endTime = std::chrono::steady_clock::now();
        running = false;
    }
}
//====================================================
double Timer::elapsed() const {
    using std::chrono::duration;
    using std::chrono::seconds;

    if (running) {
        // If running, calculate from now to startTime
        return duration<double>(std::chrono::steady_clock::now() - startTime).count();
    } else {
        // If stopped, calculate from endTime to startTime
        return duration<double>(endTime - startTime).count();
    }
}
//====================================================
void Timer::reset() {
    running = false;
    startTime = std::chrono::steady_clock::time_point();
    endTime = std::chrono::steady_clock::time_point();
}

