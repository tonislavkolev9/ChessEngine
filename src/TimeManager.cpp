#include "TimeManager.h"
#include <chrono>
#include <atomic>

using clock_type = std::chrono::steady_clock;

static clock_type::time_point endTime;
static bool timeLimited = false;

static std::atomic<bool> stopRequested{false};

static const unsigned int TIME_CHECK_INTERVAL = 2048;
static unsigned int callCounter = 0;

void startSearchTimer(long long milliseconds)
{
    timeLimited = true;
    callCounter = 0;
    stopRequested.store(false, std::memory_order_relaxed);
    endTime = clock_type::now() + std::chrono::milliseconds(milliseconds);
}

void clearSearchTimer()
{
    timeLimited = false;
    callCounter = 0;
    stopRequested.store(false, std::memory_order_relaxed);
}

void requestSearchStop()
{
    stopRequested.store(true, std::memory_order_relaxed);
}

void clearSearchStop()
{
    stopRequested.store(false, std::memory_order_relaxed);
}

bool isSearchTimeUp()
{
    if (stopRequested.load(std::memory_order_relaxed))
        return true;

    if (!timeLimited)
        return false;

    callCounter++;
    if ((callCounter & (TIME_CHECK_INTERVAL - 1)) != 0)
        return false;

    return clock_type::now() >= endTime;
}
