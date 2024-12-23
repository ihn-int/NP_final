#ifndef __TIMER__
#define __TIMER__
#include <chrono>
#include <iostream>

using namespace std::chrono;

class Timer
{
public:
    Timer()
    {
        running = false;
    }
    bool start()
    {
        if (running)
        {
            return false;
        }
        beg = system_clock::now();
        timeStamp = 0;
        running = true;
        return true;
    }
    bool get_end(int check)
    {
        if (running && timeStamp >= check)
        {
            stop();
            return true;
        }
        return false;
    }
    long long get_second()
    {
        if (!running)
        {
            return -2;
        }
        system_clock::time_point current = system_clock::now();
        long long l = duration_cast<milliseconds>(current - beg).count() / 1000;
        if (l > timeStamp)
        {
            timeStamp++;
            return timeStamp;
        }
        return -1;
    }
    bool stop(bool restart = 0)
    {
        if (!running)
        {
            return false;
        }
        running = false;
        if (restart)
        {
            running = true;
            beg = system_clock::now();
        }
        return true;
    }

private:
    system_clock::time_point beg;
    bool running;
    long long timeStamp;
};

#endif