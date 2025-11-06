#include "sphere.hpp"

Sphere::Sphere(const VECTOR& pos, const float radius):
    ShapeBase   (ShapeKind::kSphere),
    m_pos       (pos),
    m_radius    (radius)
{
    // ˆ—‚È‚µ
}

Sphere::Sphere() :
    ShapeBase   (ShapeKind::kSphere),
    m_pos       (v3d::GetZeroV()),
    m_radius    (0.0f)
{
    // ˆ—‚È‚µ
}

Sphere::~Sphere()
{
    // ˆ—‚È‚µ
}

void Sphere::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
    if (alpha_blend_num > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
        DrawSphere3D(m_pos, m_radius, kPolygonDetailed, 0xffffff, 0xffffff, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    if (is_draw_frame)
    {
        DrawSphere3D(m_pos, m_radius, kPolygonDetailed, frame_color, 0xffffff, FALSE);
    }
}

void Sphere::Move(const VECTOR& velocity)
{
    m_pos += velocity;
}
