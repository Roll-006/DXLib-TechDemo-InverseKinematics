#pragma once
#include <DxLib.h>
#include "../Base/single_instance_base.hpp"

class FPS final : public SingleInstanceBase<FPS>
{
public:
	FPS();
	~FPS();

	void Update();
	void Draw() const;
	void Wait() const;

	[[nodiscard]] float GetDeltaTime() { return m_delta_time; }

private:
	static constexpr int   kFPS				= 60;
	static constexpr int   kMaxWaitTimeMill	= 1000;
	static constexpr float kMillSecond		= 1000.0f;
	static constexpr float kMicroSecond		= 1000000.0f;

	int			m_fps;
	LONGLONG	m_first_time;		// 1ƒtƒŒ[ƒ€–Ú‚ÌŒv‘ªŠÔ
	LONGLONG	m_current_time;
	LONGLONG	m_prev_time;
	int			m_frame_count;
	float		m_average_fps;
	float		m_delta_time;
	float		m_elapsed_time;
};
