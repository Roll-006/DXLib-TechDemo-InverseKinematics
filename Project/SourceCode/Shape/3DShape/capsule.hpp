#pragma once
#include "../1DShape/segment.hpp"

class Capsule final : public ShapeBase
{
public:
    Capsule(const Segment& segment, const float radius);
    Capsule(const VECTOR& segment_begin_pos, const VECTOR& segment_end_pos, const float radius);
    Capsule(const VECTOR& segment_begin_pos, const VECTOR& dir, const float segment_length, const float radius);
    Capsule();
    ~Capsule() override;

    /// @brief 描画
    /// @param is_draw_frame フレームを描画させるかどうか
    /// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
    /// @param frame_color フレーム色
    void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

    void Move(const VECTOR& velocity) override;

    /// @brief 始点を設定する
    /// @param segment_begin_pos 始点
    /// @param is_alone 始点を単独で設定するか(true : 終点の変化なし、長さの変化あり, false : 終点の変化あり、長さの変化なし)
    void SetSegmentBeginPos(const VECTOR& segment_begin_pos, const bool is_alone) { m_segment.SetBeginPos(segment_begin_pos, is_alone); }
    /// @brief 終点を設定する
    /// @param segment_end_pos 終点
    /// @param is_alone 終点を単独で設定するか(true : 始点の変化なし、長さの変化あり, false : 始点の変化あり、長さの変化なし)
    void SetSegmentEndPos(const VECTOR& segment_end_pos, const bool is_alone) { m_segment.SetEndPos(segment_end_pos, is_alone); }

    /// @brief 向きを設定
    /// @brief 始点は変更されず終点が変化する
    void SetDir(const VECTOR& dir) { m_segment.SetDir(dir); }

    /// @brief カプセルの長さを設定
    /// @param length カプセルの長さ
    void SetCapsuleLength(const float capsule_length) { m_segment.SetLength(capsule_length - m_radius * 2.0f); }

    [[nodiscard]] const Segment& GetSegment() const { return m_segment; }
    [[nodiscard]] float GetRadius() const { return m_radius; }

private:
    static constexpr int kPolygonDetailed = 8; // ポリゴンの細かさ

    float   m_radius;
    Segment m_segment;
};
