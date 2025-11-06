#include "fps.hpp"

FPS::FPS() :
    m_fps           (kFPS),
    m_first_time    (0),
    m_current_time  (0),
    m_prev_time     (0),
    m_frame_count   (1),
    m_average_fps   (0.0f),
    m_delta_time    (0.0f),
    m_elapsed_time  (0.0f)
{
    m_first_time = m_current_time = m_prev_time = GetNowHiPerformanceCount();
}

FPS::~FPS()
{

}

void FPS::Update()
{
    m_current_time  = GetNowHiPerformanceCount();
    m_delta_time    = (m_current_time - m_prev_time) / kMicroSecond;
    m_prev_time     = m_current_time;

    if (m_frame_count == m_fps)
    {
        float total_frame_t = static_cast<float>(m_current_time - m_first_time);
        float calc_average  = total_frame_t / m_fps;

        m_average_fps   = kMicroSecond / calc_average;
        m_first_time    = GetNowHiPerformanceCount();
        m_frame_count   = 1;
        return;
    }
    ++m_frame_count;

    m_elapsed_time += m_delta_time;
}

void FPS::Draw() const
{
    DrawFormatString(0,  0, 0xffffff, "FPS : %f", m_average_fps);
}

void FPS::Wait() const
{
    float wait_t = kMicroSecond / m_fps * m_frame_count - (m_current_time - m_first_time);
    wait_t /= kMillSecond;

    if (wait_t > 0 && wait_t <= kMaxWaitTimeMill)
    {
        WaitTimer(static_cast<int>(wait_t));
    }
}
