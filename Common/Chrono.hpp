#ifndef Chrono_hpp
#define Chrono_hpp

#include <chrono>
#include <ctime>

class Chrono
{
public:
    typedef size_t Tick;
    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    template <typename Ratio=std::milli>
    static Tick GetTicks (TimePoint=Clock::now());
    template <typename Ratio=std::milli>
    static Tick GetTicksAtStartup ();
    template <typename Ratio=std::milli>
    static Tick GetTicksRelativeToStartup (TimePoint=Clock::now());

private:
    static const TimePoint s_ticksAtApplicationStartup;	
};

template <typename Ratio>
inline Chrono::Tick Chrono::GetTicks (Chrono::TimePoint time)
{
    auto ticks = time.time_since_epoch();
    return Chrono::Tick(std::chrono::duration_cast< std::chrono::duration<double, Ratio> >(ticks).count());
}

template <typename Ratio>
inline Chrono::Tick Chrono::GetTicksAtStartup ()
{
    return GetTicks<Ratio>(s_ticksAtApplicationStartup);
}

template <typename Ratio>
inline Chrono::Tick Chrono::GetTicksRelativeToStartup (Chrono::TimePoint time)
{
    return GetTicks<Ratio>(time) - GetTicksAtStartup<Ratio>();
}

#endif