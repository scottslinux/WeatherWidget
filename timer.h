#pragma once
#include <chrono> // For std::chrono

// Class to create a timer for use in other projects


class Timer {
private:
    std::chrono::steady_clock::time_point startTime;  // Start time point
    std::chrono::steady_clock::time_point endTime;    // End time point
    bool running; // Whether the timer is active
    std::chrono::steady_clock::duration duration;

public:
    Timer();             // Constructor
    void start();        // Start the timer
    void stop();         // Stop the timer
    double elapsed() const; // Elapsed time in seconds
    void reset();        // Reset the timer
};

