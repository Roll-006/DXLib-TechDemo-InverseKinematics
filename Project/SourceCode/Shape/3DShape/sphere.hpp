#pragma once
#include "../../Base/shape_base.hpp"

class Sphere final : public ShapeBase
{
public:
    Sphere(const VECTOR& pos, const float radius);
    Sphere();
    ~Sphere() override;

    /// @brief 描画
    /// @param is_draw_frame フレームを描画させるかどうか
    /// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
    /// @param frame_color フレーム色
    void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

    void Move(const VECTOR& velocity) override;

    void SetPos     (const VECTOR& pos)  { m_pos = pos; }
    void SetRadius  (const float radius) { m_radius = radius; }

    [[nodiscard]] VECTOR GetPos()    const { return m_pos; }
    [[nodiscard]] float  GetRadius() const { return m_radius; }

private:
    static constexpr int kPolygonDetailed = 16;     // ポリゴンの細かさ

    VECTOR m_pos;
    float  m_radius;
};
