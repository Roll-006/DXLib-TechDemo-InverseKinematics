#include "animator_base.hpp"

AnimatorBase::AnimatorBase(const std::shared_ptr<Modeler>& modeler, const std::string& obj_name) :
	m_obj_name		(obj_name),
	m_result_modeler(modeler)
{
	m_resource_modeler[BodyKind::kUpperBody] = std::make_shared<Modeler>(m_result_modeler->GetModelHandle());
	m_resource_modeler[BodyKind::kLowerBody] = std::make_shared<Modeler>(m_result_modeler->GetModelHandle());

	m_blend_rate[BodyKind::kUpperBody] = 1.0f;
	m_blend_rate[BodyKind::kLowerBody] = 1.0f;

	m_time_kind_data.emplace_back(std::make_tuple(BodyKind::kUpperBody, TimeKind::kPrev,	AnimTimeKindData()));
	m_time_kind_data.emplace_back(std::make_tuple(BodyKind::kUpperBody, TimeKind::kCurrent, AnimTimeKindData()));
	m_time_kind_data.emplace_back(std::make_tuple(BodyKind::kLowerBody, TimeKind::kPrev,	AnimTimeKindData()));
	m_time_kind_data.emplace_back(std::make_tuple(BodyKind::kLowerBody, TimeKind::kCurrent, AnimTimeKindData()));

	SetupStaticBone();
	DivideBone();
}

AnimatorBase::~AnimatorBase()
{
	for (auto& data : m_anim_data)
	{
		if (data.second.anim_handle != -1)
		{
			MV1DeleteModel(data.second.anim_handle);
			data.second.anim_handle = -1;
		}
	}

	m_resource_modeler.erase(BodyKind::kLowerBody);
	m_resource_modeler.erase(BodyKind::kUpperBody);
}

void AnimatorBase::AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop, const bool is_self_blend)
{
	// 上書き不可
	if (m_anim_data.count(kind)) { return; }

	int anim_handle = HandleKeeper::GetInstance()->LoadHandle(HandleKind::kAnim, file_path);
	if (anim_handle != -1)
	{
		m_anim_data[kind] = AnimKindData(anim_handle, index, tag, play_speed, is_loop, is_self_blend);
	}
}

void AnimatorBase::AddAnimHandle(const int kind, const int anim_handle, const int index, const std::string& tag, const float play_speed, const bool is_loop, const bool is_self_blend)
{
	// 上書き不可
	if (m_anim_data.count(kind)) { return; }

	if (anim_handle != -1)
	{
		m_anim_data[kind] = AnimKindData(anim_handle, index, tag, play_speed, is_loop, is_self_blend);
	}
}

void AnimatorBase::AttachAnim(const int next_kind, const BodyKind body_kind)
{	
	if (!CanAttachAnim(next_kind, body_kind)) { return; }

	// 指定のアニメーション・ボディに該当するデータを検索
	AnimTimeKindData	prev_time_data;
	AnimTimeKindData*	current_time_data		= nullptr;
	bool				is_seted_prev_data		= false;
	bool				is_seted_current_data	= false;
	for (auto& [body, time, data] : m_time_kind_data)
	{
		if (body == body_kind)
		{
			if (time == TimeKind::kPrev)	{ prev_time_data	= data;  is_seted_prev_data		= true; }
			if (time == TimeKind::kCurrent) { current_time_data	= &data; is_seted_current_data	= true; }
		}

		if (is_seted_prev_data && is_seted_current_data) { break; }
	}

	// データをシフト(Current ➡ Prev)
	for (auto& [body, time, data] : m_time_kind_data)
	{
		if (body == body_kind && time == TimeKind::kPrev)
		{
			if (current_time_data)
			{
				data = *current_time_data;
				prev_time_data = data;
				break;
			}
		}
	}

	// データをシフト(Next ➡ Current)
	if (current_time_data)
	{
		current_time_data->kind			= next_kind;
		current_time_data->attach_index = MV1AttachAnim(m_resource_modeler.at(body_kind)->GetModelHandle(), m_anim_data.at(next_kind).index, m_anim_data.at(next_kind).anim_handle, TRUE);
		current_time_data->total_time	= MV1GetAttachAnimTotalTime(m_resource_modeler.at(body_kind)->GetModelHandle(), current_time_data->attach_index);
		SetPlayStartTime(current_time_data, prev_time_data, body_kind);
	}

	// 前回のアニメーションが存在しない場合は、ブレンド済み(ブレンド率100%)とする
	m_blend_rate.at(body_kind) = prev_time_data.attach_index > -1 ? 0.0f : 1.0f;
}

void AnimatorBase::AttachResultAnim(const int next_kind)
{
	if (!CanResultAttachAnim()){ return; }

	AttachAnim(next_kind, BodyKind::kUpperBody);
	AttachAnim(next_kind, BodyKind::kLowerBody);

	// 下半身アニメーションの再生位置を引き継ぐ
	float				lower_play_timer	= 0.0f;
	AnimTimeKindData*	upper_body_data		= nullptr;
	for (auto& [body_kind, time_kind, data] : m_time_kind_data)
	{
		if (time_kind == TimeKind::kCurrent)
		{
			switch (body_kind)
			{
			case BodyKind::kUpperBody:
				upper_body_data = &data;
				break;

			case BodyKind::kLowerBody:
				lower_play_timer = data.play_timer;
				break;
			}
		}
	}
	if (upper_body_data) { upper_body_data->play_timer = lower_play_timer; }
}

void AnimatorBase::DetachAnim(const TimeKind time_kind, const BodyKind body_kind)
{
	for (auto& [body, time, data] : m_time_kind_data)
	{
		if (body == body_kind && time == time_kind && data.attach_index > -1)
		{
			MV1DetachAnim(m_resource_modeler.at(body_kind)->GetModelHandle(), data.attach_index);
			data.attach_index = -1;
			return;
		}
	}
}

float AnimatorBase::GetPlayRate(const BodyKind body_kind) const
{
	for (auto& [body, time, data] : m_time_kind_data)
	{
		if (body == body_kind && time == TimeKind::kCurrent)
		{
			return data.play_timer / data.total_time;
		}
	}

	return 0.0f;
}

bool AnimatorBase::IsPlayEnd(const BodyKind body_kind)
{
	for (auto& [body, time, data] : m_time_kind_data)
	{
		if (body == body_kind && time == TimeKind::kCurrent)
		{
			return data.play_timer == data.total_time;
		}
	}

	return false;
}

void AnimatorBase::PlayAnim()
{
	for (auto& [body_kind, time_kind, data] : m_time_kind_data)
	{
		// アニメーションが有効であった場合のみ再生
		if (data.attach_index > -1)
		{
			const auto blend_rate		= time_kind == TimeKind::kCurrent ? m_blend_rate.at(body_kind) : 1.0f - m_blend_rate.at(body_kind);
			const auto is_self_blend	= m_anim_data.at(data.kind).is_self_blend;
			const auto is_loop			= m_anim_data.at(data.kind).is_loop && !is_self_blend ? true : false;
			float play_speed			= m_anim_data.at(data.kind).play_speed * GetDeltaTime();
			math::Increase(data.play_timer, play_speed, data.total_time, is_loop);

			// 再生位置・ブレンド率を適用
			MV1SetAttachAnimTime     (m_resource_modeler.at(body_kind)->GetModelHandle(), data.attach_index, data.play_timer);
			MV1SetAttachAnimBlendRate(m_resource_modeler.at(body_kind)->GetModelHandle(), data.attach_index, blend_rate);
		}
	}

	CombineAnim();
}

void AnimatorBase::BlendAnim()
{
	// ブレンド率100%まで増加させる
	math::Increase(m_blend_rate.at(BodyKind::kUpperBody), kBlendSpeed * GetDeltaTime(), 1.0f, false);
	math::Increase(m_blend_rate.at(BodyKind::kLowerBody), kBlendSpeed * GetDeltaTime(), 1.0f, false);
  
	// ブレンドが完了した場合、PravAnimは不要なためデタッチする
	if (m_blend_rate.at(BodyKind::kUpperBody) == 1.0f) { DetachAnim(TimeKind::kPrev, BodyKind::kUpperBody); }
	if (m_blend_rate.at(BodyKind::kLowerBody) == 1.0f) { DetachAnim(TimeKind::kPrev, BodyKind::kLowerBody); }
}

void AnimatorBase::SetupStaticBone()
{
	const int model_handle = m_result_modeler->GetModelHandle();
	std::unordered_map<std::string, int> lower_body_bone_num;
	std::unordered_map<std::string, int> upper_body_bone_num;

	// 下半身のボーンを設定
	const auto begin_lower_body_bone_num = MV1SearchFrame(model_handle, BonePath.LEFT_UP_LEG);
	const auto   end_lower_body_bone_num = MV1SearchFrame(model_handle, BonePath.RIGHT_TOE_END);
	for (int i = begin_lower_body_bone_num; i <= end_lower_body_bone_num; ++i)
	{
		lower_body_bone_num[MV1GetFrameName(model_handle, i)] = i;
	}
	m_bone_numbers[BodyKind::kLowerBody] = lower_body_bone_num;

	// 上半身のボーンを設定
	const auto begin_upper_body_bone_num = MV1SearchFrame(model_handle, BonePath.LEFT_SHOULDER);
	const auto   end_upper_body_bone_num = MV1SearchFrame(model_handle, BonePath.RIGHT_HAND_PINKY_4);
	for (int i = begin_upper_body_bone_num; i <= end_upper_body_bone_num; ++i)
	{
		upper_body_bone_num[MV1GetFrameName(model_handle, i)] = i;
	}
	m_bone_numbers[BodyKind::kUpperBody] = upper_body_bone_num;
}

void AnimatorBase::DivideBone(const TCHAR* upper_body_end_bone)
{
	const int model_handle = m_result_modeler->GetModelHandle();

	// 下半身のボーンを設定
	const auto begin_lower_body_bone_index = MV1SearchFrame(model_handle, BonePath.HIPS);
	const auto   end_lower_body_bone_index = MV1SearchFrame(model_handle, upper_body_end_bone) - 1;
	for (int i = begin_lower_body_bone_index; i <= end_lower_body_bone_index; ++i)
	{
		m_bone_numbers[BodyKind::kLowerBody][MV1GetFrameName(model_handle, i)] = i;
		m_bone_numbers[BodyKind::kUpperBody].erase(MV1GetFrameName(model_handle, i));
	}

	// 上半身のボーンを設定
	const auto begin_upper_body_bone_index = MV1SearchFrame(model_handle, upper_body_end_bone);
	const auto   end_upper_body_bone_index = MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END_END);
	for (int i = begin_upper_body_bone_index; i <= end_upper_body_bone_index; ++i)
	{
		m_bone_numbers[BodyKind::kUpperBody][MV1GetFrameName(model_handle, i)] = i;
	}
}

void AnimatorBase::SetPlayStartTime(AnimTimeKindData* current_time_kind_data, const AnimTimeKindData& prev_time_kind_data, const BodyKind body_kind)
{
	if (m_anim_data.count(prev_time_kind_data.kind))
	{
		const std::string prev_tag		= m_anim_data.at(prev_time_kind_data    .kind).tag;
		const std::string current_tag	= m_anim_data.at(current_time_kind_data->kind).tag;

		if (m_anim_data.at(current_time_kind_data->kind).is_self_blend)
		{
			current_time_kind_data->play_timer = 0.0f;
			return;
		}

		// 同類アニメーションであった場合は再生率を引き継ぐ
		if (prev_tag == current_tag && prev_tag != AnimTag.NONE && current_tag != AnimTag.NONE)
		{
			m_prev_anim_play_rate[body_kind]   = prev_time_kind_data.play_timer / prev_time_kind_data.total_time;
			current_time_kind_data->play_timer = current_time_kind_data->total_time * m_prev_anim_play_rate[body_kind];
			return;
		}
	}

	current_time_kind_data->play_timer = 0.0f;
}

void AnimatorBase::CombineAnim()
{
	std::vector<std::tuple<BodyKind, int, MATRIX>> matrix;

	// 各ボーンの行列を取得
	for (const auto& [body_kind, bone_numbers] : m_bone_numbers)
	{
		for (const auto& [name, bone_num] : bone_numbers)
		{
			const auto m = MV1GetFrameLocalMatrix(m_resource_modeler.at(body_kind)->GetModelHandle(), bone_num);
			matrix.emplace_back(body_kind, bone_num, m);
		}
	}

	// リザルトモデルに適用
	for (const auto& [body_kind, bone_num, mat] : matrix)
	{
		MV1SetFrameUserLocalMatrix(m_result_modeler->GetModelHandle(), bone_num, mat);
	}
}

bool AnimatorBase::CanResultAttachAnim()
{
	// どちらかのブレンド率が1.0に到達していない場合はアタッチを許可しない
	if (m_blend_rate.at(BodyKind::kUpperBody) != 1.0f || m_blend_rate.at(BodyKind::kLowerBody) != 1.0f)
	{
		return false;
	}

	return true;
}

bool AnimatorBase::CanAttachAnim(const int next_kind, const BodyKind body_kind)
{
	// 指定のアニメーションが存在していなければ早期return
	if (!m_anim_data.count(next_kind))
	{
		return false;
	}

	// 現在のアニメーションと同じであった場合はアタッチを許可しない	
	const bool is_same = false;
	for (const auto& [body, time, data] : m_time_kind_data)
	{
		if (body == body_kind && time == TimeKind::kCurrent && data.kind == next_kind)
		{
			if (!(m_anim_data.at(next_kind).is_self_blend && IsPlayEnd(body_kind)))
			{
				return false;
			}
		}
	}

	// ブレンドが完了していない場合はアタッチを許可しない
	if (m_blend_rate.at(body_kind) != 1.0f)
	{
		return false;
	}

	return true;
}

float AnimatorBase::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_obj_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
