#include "game_time_manager.hpp"

GameTimeManager::GameTimeManager() : 
	m_fps			(std::make_unique<FPS>()),
	m_time_scale  { { TimeScaleLayerKind::kNoneScale,	 1.0f },
					{ TimeScaleLayerKind::kWorld,		 1.0f },
					{ TimeScaleLayerKind::kPlayer,		 1.0f },
					{ TimeScaleLayerKind::kUI,			 1.0f },
					{ TimeScaleLayerKind::kAudio,		 1.0f },
					{ TimeScaleLayerKind::kEffect,		 1.0f },
					{ TimeScaleLayerKind::kCamera,		 1.0f } },
	m_change_timer{ { TimeScaleLayerKind::kNoneScale,	 0.0f },
					{ TimeScaleLayerKind::kWorld,		 0.0f },
					{ TimeScaleLayerKind::kPlayer,		 0.0f },
					{ TimeScaleLayerKind::kUI,			 0.0f },
					{ TimeScaleLayerKind::kAudio,		 0.0f },
					{ TimeScaleLayerKind::kEffect,		 0.0f },
					{ TimeScaleLayerKind::kCamera,		 0.0f } },
	m_change_time { { TimeScaleLayerKind::kNoneScale,	-1.0f },
					{ TimeScaleLayerKind::kWorld,		-1.0f },
					{ TimeScaleLayerKind::kPlayer,		-1.0f },
					{ TimeScaleLayerKind::kUI,			-1.0f },
					{ TimeScaleLayerKind::kAudio,		-1.0f },
					{ TimeScaleLayerKind::kEffect,		-1.0f },
					{ TimeScaleLayerKind::kCamera,		-1.0f } }
{

}

GameTimeManager::~GameTimeManager()
{


}

void GameTimeManager::Update()
{
	m_fps->Update();
	CalcTimeScaleChangeTime();
}

void GameTimeManager::Draw()
{
	m_fps->Draw();
}

void GameTimeManager::WaitTime()
{
	m_fps->Wait();
}

void GameTimeManager::InitTimeScale()
{
	for (auto& time_scale : m_time_scale)
	{
		time_scale.second = 1.0f;
	}
}

void GameTimeManager::SetTimeScale(const TimeScaleLayerKind layer_kind, const float time_scale, const float change_time)
{
	if (layer_kind == TimeScaleLayerKind::kNoneScale) { return; }

	m_time_scale	.at(layer_kind)	= time_scale;
	m_change_time	.at(layer_kind)	= change_time;
	m_change_timer	.at(layer_kind)	= 0.0f;
}

float GameTimeManager::GetDeltaTime(const TimeScaleLayerKind scale_layer_kind) const
{
	return m_fps->GetDeltaTime() * m_time_scale.at(scale_layer_kind);
}

void GameTimeManager::CalcTimeScaleChangeTime()
{
	const auto delta_time = GetDeltaTime(TimeScaleLayerKind::kNoneScale);

	for (auto& timer : m_change_timer)
	{
		// Žw’èŽžŠÔ‚ªÝ’è‚³‚ê‚Ä‚¢‚È‚¢ê‡‚ÍŽžŠÔ‚ði‚ß‚È‚¢
		if (m_change_time.at(timer.first) == -1) { continue; }

		if (timer.second > m_change_time.at(timer.first))
		{
			m_time_scale.at(timer.first) = 1.0f;
			m_change_time.at(timer.first) = -1;
		}

		timer.second += delta_time;
	}
}
