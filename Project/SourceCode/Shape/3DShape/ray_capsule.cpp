#include "ray_capsule.hpp"

RayCapsule::RayCapsule(const Capsule& begin_capsule, const Capsule& end_capsule) : 
	ShapeBase		(ShapeKind::kRayCapsule),
	m_begin_capsule	(begin_capsule),
	m_end_capsule	(end_capsule),
	m_dir			(v3d::GetNormalizedV(m_end_capsule.GetSegment().GetBeginPos() - m_begin_capsule.GetSegment().GetBeginPos())),
	m_ray_length	(VSize(m_begin_capsule.GetSegment().GetBeginPos() - m_end_capsule.GetSegment().GetBeginPos()))
{
	// 処理なし
}

RayCapsule::RayCapsule(const Capsule& begin_capsule, const VECTOR& dir, const float ray_length) :
	ShapeBase		(ShapeKind::kRayCapsule),
	m_begin_capsule	(begin_capsule),
	m_end_capsule	(Capsule(m_begin_capsule.GetSegment().GetBeginPos() + dir * ray_length, m_begin_capsule.GetSegment().GetEndPos() + dir * ray_length, m_begin_capsule.GetRadius())),
	m_dir			(dir),
	m_ray_length	(ray_length)
{
	// 処理なし
}

RayCapsule::RayCapsule() :
	ShapeBase		(ShapeKind::kRayCapsule),
	m_begin_capsule	(Capsule()),
	m_end_capsule	(Capsule()),
	m_dir			(v3d::GetZeroV()),
	m_ray_length	(0.0f)
{
	// 処理なし
}

RayCapsule::~RayCapsule()
{
	// 処理なし
}

void RayCapsule::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	m_begin_capsule	.Draw(is_draw_frame, alpha_blend_num, frame_color);
	m_end_capsule	.Draw(is_draw_frame, alpha_blend_num, frame_color);

	if (is_draw_frame)
	{
		DrawLine3D(m_begin_capsule.GetSegment().GetBeginPos(),	m_end_capsule.GetSegment().GetBeginPos(),	frame_color);
		DrawLine3D(m_begin_capsule.GetSegment().GetEndPos(),	m_end_capsule.GetSegment().GetEndPos(),		frame_color);
	}
}

void RayCapsule::Move(const VECTOR& velocity)
{
	m_begin_capsule	.Move(velocity);
	m_end_capsule	.Move(velocity);
}

void RayCapsule::SetSegmentBeginPos(const VECTOR& segment_begin_pos, const bool is_set_begin_capsule, const bool is_alone)
{
	// 始点カプセルを基準にして線分の始点を設定
	if (is_set_begin_capsule)
	{
		m_begin_capsule	.SetSegmentBeginPos(segment_begin_pos, is_alone);
		m_end_capsule	.SetSegmentBeginPos(segment_begin_pos + m_dir * m_ray_length, is_alone);
	}
	// 終点カプセルを基準にして線分の始点を設定
	else
	{
		m_begin_capsule	.SetSegmentBeginPos(segment_begin_pos - m_dir * m_ray_length, is_alone);
		m_end_capsule	.SetSegmentBeginPos(segment_begin_pos, is_alone);
	}

	// 線分の始点間の向きを取得
	m_dir = v3d::GetNormalizedV(m_end_capsule.GetSegment().GetBeginPos() - m_begin_capsule.GetSegment().GetBeginPos());
}

void RayCapsule::SetSegmentEndPos(const VECTOR& segment_end_pos, const bool is_set_begin_capsule, const bool is_alone)
{
	// 始点カプセルを基準にして線分の終点を設定
	if (is_set_begin_capsule)
	{
		m_begin_capsule	.SetSegmentEndPos(segment_end_pos, is_alone);
		m_end_capsule	.SetSegmentEndPos(segment_end_pos + m_dir * m_ray_length, is_alone);
	}
	// 終点カプセルを基準にして線分の終点を設定
	else
	{
		m_begin_capsule	.SetSegmentEndPos(segment_end_pos - m_dir * m_ray_length, is_alone);
		m_end_capsule	.SetSegmentEndPos(segment_end_pos, is_alone);
	}

	// 線分の始点間の向きを取得
	m_dir = v3d::GetNormalizedV(m_end_capsule.GetSegment().GetBeginPos() - m_begin_capsule.GetSegment().GetBeginPos());
}

void RayCapsule::SetCapsuleLength(const float capsule_length)
{
	m_begin_capsule	.SetCapsuleLength(capsule_length);
	m_end_capsule	.SetCapsuleLength(capsule_length);
}

void RayCapsule::SetRayLength(const float ray_length)
{
	m_ray_length = ray_length;
	m_end_capsule.SetSegmentBeginPos(m_begin_capsule.GetSegment().GetBeginPos() + m_dir * m_ray_length, false);
}
