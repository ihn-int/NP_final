#ifndef __TIMER__
#define __TIMER__
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
    Timer()
    {
        running = false;
    }
    bool begin()
    {
        if (running)
        {
            return false;
        }
        start = system_clock::now();
        return true;
    }
    long long get()
    {
        if (!running)
        {
            return -1;
        }
        system_clock::time_point current = system_clock::now();
        return duration_cast<milliseconds>(start - current).count();
    }
    bool close(bool restart = 0)
    {
        if (!running)
        {
            return false;
        }
        if (restart)
        {
            start = system_clock::now();
        }
        return true;
    }

private:
    system_clock::time_point start;
    bool running;
};

#endif