#pragma once
#include "../../Base/shape_base.hpp"

class Cone final : public ShapeBase
{
public:
    Cone(const VECTOR& vertex, const VECTOR& dir, const float length, const float fov);
    Cone();
    ~Cone() override;

    /// @brief 描画
    /// @param is_draw_frame フレームを描画させるかどうか
    /// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
    /// @param frame_color フレーム色
    void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

    void Move(const VECTOR& velocity) override;

    void SetVertex  (const VECTOR& vertex)  { m_vertex  = vertex; }
    void SetDir     (const VECTOR& dir)     { m_dir     = dir; }


    #pragma region Getter
    [[nodiscard]] VECTOR GetVertex()    const { return m_vertex; }
    [[nodiscard]] VECTOR GetDir()       const { return m_dir; }
    [[nodiscard]] float  GetLength()    const { return m_length; }
    [[nodiscard]] float  GetRadius()    const { return m_radius; }
    [[nodiscard]] float  GetFOV()       const { return m_fov; }
    #pragma endregion

private:
    static constexpr int kPolygonDetailed = 16;     // ポリゴンの細かさ

    VECTOR m_vertex;
    VECTOR m_dir;
    float  m_length;
    float  m_radius;
    float  m_fov;
};
