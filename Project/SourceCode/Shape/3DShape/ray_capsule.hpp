#pragma once
#include "../3DShape/capsule.hpp"

/// @brief 光線カプセル
class RayCapsule final : public ShapeBase
{
public:
    /// @brief コンストラクタ
    /// @brief 始点カプセルと終点カプセルは常に平行である必要あり
    /// @param begin_capsule 始点カプセル
    /// @param end_capsule 終点カプセル
    RayCapsule(const Capsule& begin_capsule, const Capsule& end_capsule);
    /// @brief コンストラクタ
    /// @brief 始点カプセルと終点カプセルは常に平行である必要あり
    /// @param begin_capsule 始点カプセル
    /// @param dir 始点カプセルから終点カプセルへの方向
    /// @param ray_length カプセルの線分同士の長さ(距離)
    RayCapsule(const Capsule& begin_capsule, const VECTOR& dir, const float ray_length);
    RayCapsule();
    ~RayCapsule() override;

    /// @brief 描画
    /// @param is_draw_frame フレームを描画させるかどうか
    /// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
    /// @param frame_color フレーム色
    void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

    void Move(const VECTOR& velocity) override;

    /// @brief 始点を設定する
    /// @brief 始点カプセルと終点カプセルの線分の始点が同時に設定させる
    /// @param segment_begin_pos 始点
    /// @param is_set_begin_capsule 始点カプセルの値で設定するか(true : 始点カプセル, false : 終点カプセル)
    /// @param is_alone 始点を単独で設定するか(true : 終点の変化なし、長さの変化あり, false : 終点の変化あり、長さの変化なし)
    void SetSegmentBeginPos(const VECTOR& segment_begin_pos, const bool is_set_begin_capsule, const bool is_alone);
    /// @brief 終点を設定する
    /// @brief 始点カプセルと終点カプセルの線分の終点が同時に設定させる
    /// @param segment_end_pos 終点
    /// @param is_set_begin_capsule 始点カプセルの値で設定するか(true : 始点カプセル, false : 終点カプセル)
    /// @param is_alone 終点を単独で設定するか(true : 始点の変化なし、長さの変化あり, false : 始点の変化あり、長さの変化なし)
    void SetSegmentEndPos  (const VECTOR& segment_end_pos,   const bool is_set_begin_capsule, const bool is_alone);

    /// @brief カプセルの長さを設定
    /// @param length カプセルの長さ
    void SetCapsuleLength(const float capsule_length);

    /// @brief 光線の長さを設定
    /// @brief 始点を基準として長さを変更する
    /// @param ray_length カプセルの線分同士の長さ(距離)
    void SetRayLength(const float ray_length);

    [[nodiscard]] const Capsule& GetBeginCapsule()  const { return m_begin_capsule; }
    [[nodiscard]] const Capsule& GetEndCapsule()    const { return m_end_capsule; }
    [[nodiscard]] VECTOR         GetDir()           const { return m_dir; }
    [[nodiscard]] float          GetRayLength()     const { return m_ray_length; }

private:
    Capsule m_begin_capsule;
    Capsule m_end_capsule;
    VECTOR  m_dir;
    float   m_ray_length;
};
