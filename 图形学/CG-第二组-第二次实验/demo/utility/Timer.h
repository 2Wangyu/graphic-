//
// Created by 分子轨道mo法 on 25-4-16.
// D指导的计时器类
//

#ifndef CG_E2_TIMER_H
#define CG_E2_TIMER_H

#include <windows.h>
#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>

class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;
    using TimePoint = Clock::time_point;
    using Callback = std::function<void()>;

    // 基本构造函数
    Timer() = default;

    // 立即启动计时器
    void start() {
        if (!m_running) {
            m_start_time = Clock::now();
            m_paused_time.reset();
            m_running = true;
        }
    }

    // 暂停计时
    void pause() {
        if (m_running && !m_paused) {
            m_paused_time = Clock::now();
            m_paused = true;
        }
    }

    // 继续计时
    void resume() {
        if (m_running && m_paused) {
            m_start_time += (Clock::now() - *m_paused_time);
            m_paused_time.reset();
            m_paused = false;
        }
    }

    // 重置计时器
    void reset() {
        m_start_time = Clock::now();
        m_paused_time.reset();
        m_paused = false;
        m_running = false;
    }

    // 获取经过时间（毫秒）
    template<typename T = std::chrono::milliseconds>
    auto elapsed() const {
        if (!m_running) return T(0);
        if (m_paused) {
            return std::chrono::duration_cast<T>(*m_paused_time - m_start_time);
        }
        return std::chrono::duration_cast<T>(Clock::now() - m_start_time);
    }

    // 单次定时回调（异步执行）
    template<typename Rep, typename Period>
    void set_timeout(const std::chrono::duration<Rep, Period> &delay, Callback callback) {
        std::thread([=] {
            std::this_thread::sleep_for(delay);
            if (!m_stop_flag.load()) {
                callback();
            }
        }).detach();
    }

    // 周期性定时回调（异步执行）
    template<typename Rep, typename Period>
    void set_interval(const std::chrono::duration<Rep, Period> &interval, Callback callback) {
        m_stop_flag.store(false);
        std::thread([=] {
            while (!m_stop_flag.load()) {
                auto start = Clock::now();
                callback();
                std::this_thread::sleep_until(start + interval);
            }
        }).detach();
    }

    // 停止所有定时任务
    void stop() {
        m_stop_flag.store(true);
    }

    // 平台相关高精度计时（示例实现）
    static double high_resolution_time() {
#if defined(_WIN32)
        LARGE_INTEGER freq, counter;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&counter);
        return static_cast<double>(counter.QuadPart) / freq.QuadPart * 1000.0;
#else
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
#endif
    }

protected:
    TimePoint m_start_time;
    std::optional<TimePoint> m_paused_time;
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_paused{false};
    std::atomic<bool> m_stop_flag{false};
};

class GameTimer : public Timer {
public:
    // 开始帧计时
    void startFrame() {
        if (!m_running) {
            start();
            m_last_frame = m_start_time;
        }
    }

    // 获取当前帧的deltaTime（单位：秒）
    float tick() {
        if (!m_running) return 0.0f;

        const auto now = Clock::now();
        const auto delta = now - m_last_frame;
        m_last_frame = now;

        // 限制最大delta时间（避免卡顿导致异常）
        constexpr float MAX_DELTA = 1.0f / 10.0f; // 最低10FPS
        m_delta_time = std::chrono::duration<float>(delta).count();
        m_delta_time = std::min(m_delta_time, MAX_DELTA);

        return m_delta_time;
    }

    // 获取平滑处理的deltaTime（可选）
    float getSmoothDelta() const {
        constexpr float SMOOTH_FACTOR = 0.95f;
        return m_smooth_delta * SMOOTH_FACTOR + m_delta_time * (1 - SMOOTH_FACTOR);
    }

    // 帧率计算
    float fps() const {
        return 1.0f / m_delta_time;
    }

private:
    TimePoint m_last_frame;
    float m_delta_time = 0.0f;
    float m_smooth_delta = 0.0167f; // 初始60FPS
};

#endif //CG_E2_TIMER_H
