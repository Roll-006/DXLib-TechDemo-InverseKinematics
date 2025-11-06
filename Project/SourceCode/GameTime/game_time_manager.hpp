#pragma once
#include <memory>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "fps.hpp"

enum class TimeScaleLayerKind
{
	kNoneScale,	// 等倍
	kWorld,		// ゲーム全体(敵, 環境, 物理)
	kPlayer,	// プレイヤー
	kUI,		// UI
	kAudio,		// サウンド
	kEffect,	// エフェクト
	kCamera,	// カメラ
};

class GameTimeManager final : public SingletonBase<GameTimeManager>
{
public:
	void Update();
	void Draw();
	void WaitTime();

	void InitTimeScale();

	/// @brief タイムスケールを設定する
	/// @param layer_kind レイヤー (TimeScaleLayerKind::kNoneScaleの指定は許可しない)
	/// @param time_scale タイムスケール
	/// @param change_time 指定したタイムスケールを適用する時間
	void SetTimeScale(const TimeScaleLayerKind layer_kind, const float time_scale, const float change_time = -1.0f);

	[[nodiscard]] float GetDeltaTime(const TimeScaleLayerKind scale_layer_kind) const;
	[[nodiscard]] float GetTimeScale(const TimeScaleLayerKind scale_layer_kind) const { return m_time_scale.at(scale_layer_kind); }

private:
	GameTimeManager();
	~GameTimeManager() override;

	void CalcTimeScaleChangeTime();

private:
	std::unique_ptr<FPS>							m_fps;
	std::unordered_map<TimeScaleLayerKind, float>	m_time_scale;
	std::unordered_map<TimeScaleLayerKind, float>	m_change_time;
	std::unordered_map<TimeScaleLayerKind, float>	m_change_timer;

	friend SingletonBase<GameTimeManager>;
};
