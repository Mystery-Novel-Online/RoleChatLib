#pragma once
#include <chrono>
#include <vector>

namespace rolechat::benchmark
{

struct TimerResult {
    bool running = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    double duration = 0.0f;
};

class Timer {
public:
    Timer(const std::string& a_name, bool a_silent = false);
    ~Timer();

    void start();
    void stop();
    void restart();
    TimerResult& runningResult() { return m_resultData; }
private:
    std::string m_name;
    bool m_silentDebug = false;

    TimerResult m_resultData;
    std::vector<TimerResult> m_recordedResults;
};

}