#include "rolechat/util/BenchmarkTimer.h"

#include <iostream>
#include <__msvc_ostream.hpp>

using namespace rolechat::benchmark;

Timer::Timer(const std::string &a_name, bool a_silent)
    : m_name(a_name)
    , m_silentDebug(a_silent) {
}

Timer::~Timer() {
    stop();
}

void Timer::start() {
    m_resultData.running = true;
    m_resultData.startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    if (!m_resultData.running) {
        return;
    }

    m_resultData.endTime = std::chrono::high_resolution_clock::now();
    m_resultData.running = false;

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_resultData.startTime);
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_resultData.endTime);

    auto duration = end - start;
    double ms = std::chrono::duration<double, std::milli>(duration).count();
    m_resultData.duration = ms;

    if (!m_silentDebug) {
        std::cout << "[Benchmark] [" << m_name <<"] " << ms << "ms"<<  std::endl;
    }
}

void Timer::restart() {
    stop();
    start();
}
