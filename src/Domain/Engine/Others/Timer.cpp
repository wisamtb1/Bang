#include "Timer.h"

#include <chrono>

Timer::Timer(std::function<void ()> func, float secDelay, int repeatCount, bool waitOneDelayTime) :
    m_func(func),
    m_secDelay(secDelay),
    m_repeatCount(repeatCount),
    m_waitOneDelayTime(waitOneDelayTime)
{
    m_timerThread = std::thread( std::bind(&Timer::TimerLoop, this) );
}

void Timer::TimerLoop() const
{
    int count = 0;
    while (m_stop);

    if (!m_waitOneDelayTime && !m_killed) { ++count; m_func(); }

    while ( !m_killed && !m_stop && (m_repeatCount == -1 || count < m_repeatCount) )
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(int(m_secDelay * 1000)));
        if (m_killed) { break; }
        m_func();
        ++count;
    }

    delete this;
}

void Timer::Start() const
{
    m_stop = false;
}

void Timer::Stop() const
{
    m_stop = true;
}

void Timer::Kill()
{
    m_killed = true;
}
