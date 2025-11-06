#include "collision_calculator.hpp"

#pragma region 衝突判定
/// @brief 点と直線の衝突判定
bool collision::IsCollidedPointAndLine           (const Point&       point,          const Line&         line,           std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndLine(point.GetPos(), line, intersection);
}
bool collision::IsCollidedPointAndLine           (const VECTOR&      point,          const Line&         line,           std::optional<VECTOR>& intersection)
{
    const bool is_hit = math::GetDistancePointToLine(point, line) < math::kEpsilonLow;

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsCollidedPointAndLine           (const VECTOR&      point,          const Line&         line)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndLine(point, line, intersection);
}

/// @brief 点と線分の衝突判定
bool collision::IsCollidedPointAndSegment        (const Point&       point,          const Segment&      segment,        std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndSegment(point.GetPos(), segment, intersection);
}
bool collision::IsCollidedPointAndSegment        (const VECTOR&      point,          const Segment&      segment,        std::optional<VECTOR>& intersection)
{
    const bool is_hit = math::GetDistancePointToSegment(point, segment) < math::kEpsilonLow;

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsCollidedPointAndSegment        (const VECTOR&      point,          const Segment&      segment)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndSegment(point, segment, intersection);
}

/// @brief 点と平面(無限に広がる面)の衝突判定
bool collision::IsCollidedPointAndPlane          (const Point&       point,          const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndPlane(point.GetPos(), plane, intersection);
}
bool collision::IsCollidedPointAndPlane          (const VECTOR&      point,          const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    const bool is_hit = math::GetDistancePointToPlane(point, plane) < math::kEpsilonLow;

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsCollidedPointAndPlane          (const VECTOR&      point,          const Plane&        plane)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndPlane(point, plane, intersection);
}

/// @brief 点と三角形の衝突判定
bool collision::IsCollidedPointAndTriangle       (const Point&       point,          const Triangle&     triangle,       std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndTriangle(point.GetPos(), triangle, intersection);
}
bool collision::IsCollidedPointAndTriangle       (const VECTOR&      point,          const Triangle&     triangle,       std::optional<VECTOR>& intersection)
{
    const Plane plane = Plane(triangle.GetCentroid(), triangle.GetNormalVector());
    if (!IsCollidedPointAndPlane(point, plane))
    {
        intersection = std::nullopt;
        return false;
    }

    const VECTOR v1 = triangle.GetPos(0) - triangle.GetPos(1);
    const VECTOR v2 = triangle.GetPos(1) - triangle.GetPos(2);
    const VECTOR v3 = triangle.GetPos(2) - triangle.GetPos(0);

    const VECTOR n_v1 = math::GetNormalVector(v1, point - triangle.GetPos(1));
    const VECTOR n_v2 = math::GetNormalVector(v2, point - triangle.GetPos(2));
    const VECTOR n_v3 = math::GetNormalVector(v3, point - triangle.GetPos(0));

    const bool is_hit = (triangle.GetNormalVector() != n_v1
                      && triangle.GetNormalVector() != n_v2
                      && triangle.GetNormalVector() != n_v3);

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsCollidedPointAndTriangle       (const VECTOR&      point,          const Triangle&     triangle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndTriangle(point, triangle, intersection);
}

/// @brief 点と四角形の衝突判定
bool collision::IsCollidedPointAndSquare         (const Point&       point,          const Square&       square,         std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndSquare(point.GetPos(), square, intersection);
}
bool collision::IsCollidedPointAndSquare         (const VECTOR&      point,          const Square&       square,         std::optional<VECTOR>& intersection)
{
    const bool is_hit = IsCollidedPointAndTriangle(point, square.GetTriangle(0)) || IsCollidedPointAndTriangle(point, square.GetTriangle(1));

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsCollidedPointAndSquare         (const VECTOR&      point,          const Square&       square)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndSquare(point, square, intersection);
}

/// @brief 点とOBBの衝突判定(OBB内に点がある場合も含む)
bool collision::IsCollidedPointAndOBB            (const Point&       point,          const OBB&          obb,            std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndOBB(point.GetPos(), obb, intersection);
}
bool collision::IsCollidedPointAndOBB            (const VECTOR&      point,          const OBB&          obb,            std::optional<VECTOR>& intersection)
{
    for (int i = 0; i < 6; ++i)
    {
        const auto square   = obb.GetSquare(static_cast<box::SquareKind>(i));
        const auto plane    = Plane(square.GetCentroid(), square.GetNormalVector());
        if(math::IsPointAheadOfPlane(point, plane))
        {
            intersection = std::nullopt;
            return false;
        }
    }

    intersection = std::optional<VECTOR>(point);
    return true;
}
bool collision::IsCollidedPointAndOBB            (const VECTOR&      point,          const OBB&          obb)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndOBB(point, obb, intersection);
}

/// @brief 点とカプセルの衝突判定
bool collision::IsCollidedPointAndCapsule        (const Point&       point,          const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndCapsule(point.GetPos(), capsule, intersection);
}
bool collision::IsCollidedPointAndCapsule        (const VECTOR&      point,          const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    const float distance = math::GetDistancePointToSegment(point, capsule.GetSegment());
    const bool  is_hit   = distance <= capsule.GetRadius();

    intersection = is_hit ? std::optional<VECTOR>(point) : std::nullopt;

    return is_hit;
}
bool collision::IsCollidedPointAndCapsule        (const VECTOR&      point,          const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndCapsule(point, capsule, intersection);
}

/// @brief 点と円錐の衝突判定
bool collision::IsCollidedPointAndCone           (const Point&       point,          const Cone&         cone,           std::optional<VECTOR>& intersection)
{
    return IsCollidedPointAndCone(point.GetPos(), cone, intersection);
}
bool collision::IsCollidedPointAndCone           (const VECTOR&      point,          const Cone&         cone,           std::optional<VECTOR>& intersection)
{
    // 頂点から点へのベクトル
    const auto vertex_to_point = point - cone.GetVertex();

    // 軸方向への投影距離を計算
    const auto projection_length = VDot(vertex_to_point, cone.GetDir());

    if (projection_length < 0.0f)               { return false; }   // 円錐の後方にある場合
    if (projection_length > cone.GetLength())   { return false; }   // 円錐の長さを超えている場合

    // 頂点付近の特別処理（数値誤差回避）
    if (projection_length < math::kEpsilonLow)
    {
        intersection = point;
        return true;
    }

    // その高さでの円錐半径を線形補間で計算
    const auto radius_at_height = (projection_length / cone.GetLength()) * cone.GetRadius();

    // 軸からの距離の二乗を計算（平方根を避ける）
    const auto axis_projection = cone.GetDir() * projection_length;
    const auto radial_vector = vertex_to_point - axis_projection;
    const auto distance_from_axis_sq = VSquareSize(radial_vector);

    // 半径の二乗と比較（平方根計算を回避）
    const auto radius_at_height_sq = radius_at_height * radius_at_height;

    bool is_inside = distance_from_axis_sq <= radius_at_height_sq;
    if (is_inside && intersection) { intersection = point; }

    return is_inside;
}
bool collision::IsCollidedPointAndCone           (const VECTOR&      point,          const Cone&         cone)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPointAndCone(point, cone, intersection);
}

/// @brief 直線と直線の衝突判定
bool collision::IsCollidedLineAndLine            (const Line&        line1,          const Line&         line2,          std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceLineToLine(line1, line2) < math::kEpsilonLow;
}
bool collision::IsCollidedLineAndLine            (const Line&        line1,          const Line&         line2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedLineAndLine(line1, line2, intersection);
}

/// @brief 直線と直線の衝突判定
bool collision::IsCollidedLineAndPlane           (const Line&        line,           const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const VECTOR v = line.GetPos() - plane.GetPos();

    const bool is_vertical = math::IsVertical(line.GetDir(), plane.GetNormalVector());
    const bool is_contain_plane = math::IsVertical(v, plane.GetNormalVector());

    return is_contain_plane || !is_vertical;
}
bool collision::IsCollidedLineAndPlane           (const Line&        line,           const Plane&        plane)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedLineAndPlane(line, plane, intersection);
}

/// @brief 直線と直線の衝突判定
bool collision::IsCollidedSegmentAndSegment      (const Segment&     segment1,       const Segment&      segment2,       std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceSegmentToSegment(segment1, segment2) < math::kEpsilonLow;
}
bool collision::IsCollidedSegmentAndSegment      (const Segment&     segment1,       const Segment&      segment2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSegmentAndSegment(segment1, segment2, intersection);
}

/// @brief 線分と平面(無限に広がる面)の衝突判定
bool collision::IsCollidedSegmentAndPlane        (const Segment&     segment,        const Plane&        plane,          std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceSegmentToPlane(segment, plane) < math::kEpsilonLow;
}
bool collision::IsCollidedSegmentAndPlane        (const Segment&     segment,        const Plane&        plane)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSegmentAndPlane(segment, plane, intersection);
}

/// @brief 線分と三角形の衝突判定
bool collision::IsCollidedSegmentAndTriangle     (const Segment&     segment,        const Triangle&     triangle,       std::optional<VECTOR>& intersection)
{
    // 三角形を平面に拡張
    const Plane plane = Plane(triangle.GetCentroid(), triangle.GetNormalVector());

    // 平面と衝突していない場合は衝突なし
    if (!IsCollidedSegmentAndPlane(segment, plane))
    {
        intersection = std::nullopt;
        return false;
    }

    const VECTOR v1 = segment.GetBeginPos() - triangle.GetPos(0);
    const VECTOR v2 = segment.GetEndPos()   - triangle.GetPos(0);
    const float  d1 = math::GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float  d2 = math::GetDistancePointToPlane(segment.GetEndPos(),   plane);
    const float  a  = d1 / (d1 + d2);
    const VECTOR v3 = (1 - a) * v1 + a * v2;

    // 平面との交点
    intersection = triangle.GetPos(0) + v3;

    // 球に拡張した交点が三角形と衝突しているか
    const Sphere sphere(*intersection, kPointCollisionRadius);
    return IsCollidedTriangleAndSphere(triangle, sphere);
}
bool collision::IsCollidedSegmentAndTriangle     (const Segment&     segment,        const Triangle&     triangle)
{
    std::optional<VECTOR> intersection = std::nullopt;
    return IsCollidedSegmentAndTriangle(segment, triangle, intersection);
}

/// @brief 線分と四角形の衝突判定
bool collision::IsCollidedSegmentAndSquare       (const Segment&     segment,        const Square&       square,         std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return IsCollidedSegmentAndTriangle(segment, square.GetTriangle(0)) || IsCollidedSegmentAndTriangle(segment, square.GetTriangle(1));
}
bool collision::IsCollidedSegmentAndSquare       (const Segment&     segment,        const Square&       square)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSegmentAndSquare(segment, square, intersection);
}

//bool collision::IsCollidedSegmentAndCircumference(const Segment* segment, const Circle* circle)
//{
//    Plane plane = Plane(circle->GetPos(), circle->GetNormalVector(), 0); // 円がある平面
//    auto intersection = math::GetIntersectionSegmentAndPlane(segment, &plane); // 交点
//
//    // 線分が平面と交点を持つ場合
//    if (intersection)
//    {
//        float minus_nan_ind = intersection->x;
//
//        // 一点で交わっている場合
//        if (std::isfinite(minus_nan_ind))
//        {
//            float distance = VSize(*intersection - circle->GetPos());
//
//            if (distance == circle->GetRadius())
//            {
//                return true;
//            }
//        }
//        // 線分と平面が同一平面上にある場合
//        else
//        {
//            float begin_distance = VSize(segment->GetBeginPosition() - circle->GetPos());
//            float end_distance = VSize(segment->GetEndPosition() - circle->GetPos());
//
//            // 始点から円の中心の距離と、終点から円の中心の距離が、半径より短い場合 ➡ 非衝突
//            if (begin_distance < circle->GetRadius() && end_distance < circle->GetRadius())
//            {
//                return false;
//            }
//
//            // どちらか片方の距離が半径より短く、もう片方が半径より長い場合 ➡ 衝突
//            if ((begin_distance < circle->GetRadius() && end_distance > circle->GetRadius())
//                || (begin_distance > circle->GetRadius() && end_distance < circle->GetRadius()))
//            {
//                return true;
//            }
//
//            // 始点と終点がともに円の外にある場合
//            // 線分と円の中心の最短距離が、半径以下の場合 ➡ 衝突
//            if (math::GetDistancePointToSegment(circle->GetPos(), segment) <= circle->GetRadius())
//            {
//                return true;
//            }
//            return false;
//        }
//    }
//    return false;
//}

/// @brief 線分と球の衝突判定
bool collision::IsCollidedSegmentAndSphere       (const Segment&     segment,        const Sphere&       sphere,         std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const auto diff     = segment.GetBeginPos() - sphere.GetPos();
    const auto dot_proj = VDot(diff, segment.GetDir());
    const auto diff_pos = VSquareSize(diff) - sphere.GetRadius() * sphere.GetRadius();

    const auto discriminant = dot_proj * dot_proj - diff_pos;
    if (discriminant < 0.0f)
    {
        return false;
    }

    const auto sqrt_d = sqrt(discriminant);
    const auto t1 = -dot_proj - sqrt_d; // 入口
    const auto t2 = -dot_proj + sqrt_d; // 出口

    const bool isInside = VSquareSize(diff) < sphere.GetRadius() * sphere.GetRadius();

    if (isInside)
    {
        // 内部なら出口側を採用
        if (t2 >= 0.0f && t2 <= segment.GetLength())
        {
            intersection = segment.GetBeginPos() + segment.GetDir() * t2;
        }
        return true;
    }
    else
    {
        // 外部なら入口側を採用
        if (t1 >= 0.0f && t1 <= segment.GetLength())
        {
            intersection = segment.GetBeginPos() + segment.GetDir() * t1;
            return true;
        }
    }

    return false;
}
bool collision::IsCollidedSegmentAndSphere       (const Segment&     segment,        const Sphere&       sphere)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSegmentAndSphere(segment, sphere, intersection);
}

/// @brief 線分とカプセルの衝突判定
bool collision::IsCollidedSegmentAndCapsule      (const Segment&     segment,        const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    // TODO : 見やすく変更

    intersection = std::nullopt;

    const auto ab           = capsule.GetSegment().GetEndPos() - capsule.GetSegment().GetBeginPos();
    const auto a0           = segment.GetBeginPos() - capsule.GetSegment().GetBeginPos();
    const auto dot_ab_ab    = VDot(ab, ab);
    const auto dot_ab_dir   = VDot(ab, segment.GetDir());
    const auto dot_ab_a0    = VDot(ab, a0);
    const auto dot_a0_dir   = VDot(a0, segment.GetDir());
    const auto dot_a0_a0    = VDot(a0, a0);

    // 2次方程式係数（円柱部との交差）
    const float a = dot_ab_ab - dot_ab_dir * dot_ab_dir;
    const float b = dot_ab_ab * dot_a0_dir - dot_ab_dir * dot_ab_a0;
    const float c = dot_ab_ab * dot_a0_a0  - dot_ab_a0  * dot_ab_a0
        - capsule.GetRadius() * capsule.GetRadius() * dot_ab_ab;

    const auto is_begin_inside = math::IsPointInsideCapsule(segment.GetBeginPos(), capsule);
    const auto is_end_inside = math::IsPointInsideCapsule(segment.GetEndPos(), capsule);

    std::vector<std::pair<float, VECTOR>> candidates;

    // 円柱部
    const float disc = b * b - a * c;
    if (disc >= 0.0f && fabs(a) > 1e-6f)
    {
        const auto sqrtDisc = sqrt(disc);
        const auto t1 = (-b - sqrtDisc) / a;
        const auto t2 = (-b + sqrtDisc) / a;

        auto CheckAndAdd = [&](float t)
        {
                if (t < 0.0f || t > segment.GetLength()) { return; }
            VECTOR pos = segment.GetBeginPos() + segment.GetDir() * t;

            // カプセル軸方向への射影を確認
            float proj = VDot(pos - capsule.GetSegment().GetBeginPos(), ab) / dot_ab_ab;
            if (proj >= 0.0f && proj <= 1.0f)
            {
                candidates.emplace_back(t, pos);
            }
        };

        CheckAndAdd(t1);
        CheckAndAdd(t2);
    }

    // 球部
    auto IntersectSphere = [&](const Sphere& sphere)
    {
        const auto diff = segment.GetBeginPos() - sphere.GetPos();
        const auto b = VDot(diff, segment.GetDir());            // (diff·dir)
        const auto c = VDot(diff, diff) - sphere.GetRadius() * sphere.GetRadius();
        const auto disc = b * b - c;
        if (disc < 0.0f) { return; }

        const auto sqrtDisc = sqrtf(disc);
        const auto t1 = -b - sqrtDisc;
        const auto t2 = -b + sqrtDisc;

        auto CheckAndAdd = [&](float t)
        {
            if (t >= 0.0f && t <= segment.GetLength()) {
                VECTOR pos = segment.GetBeginPos() + segment.GetDir() * t;
                candidates.emplace_back(t, pos);
            }
        };

        CheckAndAdd(t1);
        CheckAndAdd(t2);
    };

    IntersectSphere(Sphere(capsule.GetSegment().GetBeginPos(), capsule.GetRadius()));
    IntersectSphere(Sphere(capsule.GetSegment().GetEndPos(), capsule.GetRadius()));

    if (candidates.empty())
    {
        // 内部にある場合もヒット扱い
        return is_begin_inside || is_end_inside;
    }

    if (is_begin_inside)
    {
        // 内部から出る：t が最大の交点を取る
        auto it = std::max_element(candidates.begin(), candidates.end(), [](auto& lhs, auto& rhs)
        {
            return lhs.first < rhs.first;
        });
        intersection = it->second;
    }
    else
    {
        // 外部から入る：t が最小の交点を取る
        auto it = std::min_element(candidates.begin(), candidates.end(), [](auto& lhs, auto& rhs)
        {
             return lhs.first < rhs.first;
        });
        intersection = it->second;
    }

    return true;
}
bool collision::IsCollidedSegmentAndCapsule      (const Segment&     segment,        const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSegmentAndCapsule(segment, capsule, intersection);
}

/// @brief 線分とモデルの衝突判定
bool collision::IsCollidedSegmentAndModel        (const Segment&     segment,        const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_LineDim(model_handle, -1, segment.GetBeginPos(), segment.GetEndPos());
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        Triangle triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]);

        triangles[i] = triangle;
        distance.emplace_back(std::make_pair(i, math::GetDistancePointToTriangle(segment.GetBeginPos(), triangle)));
    }

    // 距離が最も近い三角形との交点を取得
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.emplace_back(triangles.at(dist.first));
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsCollidedSegmentAndTriangle(segment, hit_triangles.at(0), intersection);
}
bool collision::IsCollidedSegmentAndModel        (const Segment&     segment,        const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles;
    return IsCollidedSegmentAndModel(segment, model_handle, intersection, hit_triangles);
}
bool collision::IsCollidedSegmentAndModel        (const Segment&     segment,        const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;
    return IsCollidedSegmentAndModel(segment, model_handle, intersection);
}

/// @brief 平面(無限に広がる面)とカプセルの衝突判定
bool collision::IsCollidedPlaneAndCapsule        (const Plane&       plane,          const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistancePlaneToCapsule(plane, capsule) < math::kEpsilonLow;
}
bool collision::IsCollidedPlaneAndCapsule        (const Plane&       plane,          const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedPlaneAndCapsule(plane, capsule, intersection);
}

/// @brief 三角形と三角形の衝突判定
bool collision::IsCollidedTriangleAndTriangle    (const Triangle&    triangle1,      const Triangle&     triangle2,      std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceTriangleToTriangle(triangle1, triangle2) < math::kEpsilonLow;
}
bool collision::IsCollidedTriangleAndTriangle    (const Triangle&    triangle1,      const Triangle&     triangle2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedTriangleAndTriangle(triangle1, triangle2, intersection);
}

/// @brief 三角形と球の衝突判定
bool collision::IsCollidedTriangleAndSphere      (const Triangle&    triangle,       const Sphere&       sphere,         std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistancePointToTriangle(sphere.GetPos(), triangle) <= sphere.GetRadius();
}
bool collision::IsCollidedTriangleAndSphere      (const Triangle&    triangle,       const Sphere&       sphere)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedTriangleAndSphere(triangle, sphere, intersection);
}

/// @brief 三角形とカプセルの衝突判定
bool collision::IsCollidedTriangleAndCapsule     (const Triangle&    triangle,       const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    return math::GetDistanceSegmentToTriangle(capsule.GetSegment(), triangle) <= capsule.GetRadius();
}
bool collision::IsCollidedTriangleAndCapsule     (const Triangle&    triangle,       const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedTriangleAndCapsule(triangle, capsule, intersection);
}

/// @brief 三角形と光線カプセルの衝突判定
bool collision::IsCollidedTriangleAndRayCapsule  (const Triangle&    triangle,       const RayCapsule&   ray_capsule,    std::optional<VECTOR>& intersection)
{
    intersection    = std::nullopt;
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        if (IsCollidedTriangleAndCapsule(triangle, capsule, intersection))
        {
            return true;
        }

        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsCollidedTriangleAndRayCapsule  (const Triangle&    triangle,       const RayCapsule&   ray_capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedTriangleAndRayCapsule(triangle, ray_capsule, intersection);
}

/// @brief 三角形とモデルの衝突判定
bool collision::IsCollidedTriangleAndModel       (const Triangle&    triangle,       const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_Triangle(model_handle, -1, triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2));
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        hit_triangles.emplace_back(Triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]));

        triangles[i] = hit_triangles.at(i);
        distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToTriangle(triangle, hit_triangles.at(i))));
    }

    // 距離が最も近い三角形との交点を取得
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.at(loop_count) = triangles.at(dist.first);
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsCollidedTriangleAndTriangle(triangle, hit_triangles.at(0), intersection);
}
bool collision::IsCollidedTriangleAndModel       (const Triangle&    triangle,       const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles{};
    return IsCollidedTriangleAndModel(triangle, model_handle, intersection, hit_triangles);
}
bool collision::IsCollidedTriangleAndModel       (const Triangle&    triangle,       const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedTriangleAndModel(triangle, model_handle, intersection);
}

/// @brief 四角形とカプセルの衝突判定
bool collision::IsCollidedSquareAndCapsule       (const Square&      square,         const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
   intersection = std::nullopt;

    const float distance1 = math::GetDistanceSegmentToTriangle(capsule.GetSegment(), square.GetTriangle(0));
    const float distance2 = math::GetDistanceSegmentToTriangle(capsule.GetSegment(), square.GetTriangle(1));

    return distance1 <= capsule.GetRadius() || distance2 <= capsule.GetRadius();
}
bool collision::IsCollidedSquareAndCapsule       (const Square&      square,         const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSquareAndCapsule(square, capsule, intersection);
}

/// @brief 四角形と光線カプセルの衝突判定
bool collision::IsCollidedSquareAndRayCapsule    (const Square&      square,         const RayCapsule&   ray_capsule,    std::optional<VECTOR>& intersection)
{
    intersection    = std::nullopt;
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        if (IsCollidedSquareAndCapsule(square, capsule, intersection))
        {
            return true;
        }

        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsCollidedSquareAndRayCapsule    (const Square&      square,         const RayCapsule&   ray_capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSquareAndRayCapsule(square, ray_capsule, intersection);
}

/// @brief 球と球の衝突判定
bool collision::IsCollidedSphereAndSphere        (const Sphere&      sphere1,        const Sphere&       sphere2,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const VECTOR p1       = sphere1.GetPos();
    const VECTOR p2       = sphere2.GetPos();
    const float  distance = VSize(p2 - p1);

    return distance <= sphere1.GetRadius() + sphere2.GetRadius();
}
bool collision::IsCollidedSphereAndSphere        (const Sphere&      sphere1,        const Sphere&       sphere2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSphereAndSphere(sphere1, sphere2, intersection);
}

/// @brief 球とカプセルの衝突判定 
bool collision::IsCollidedSphereAndCapsule       (const Sphere&      sphere,         const Capsule&      capsule,        std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const VECTOR p = sphere.GetPos();
    const float distance = math::GetDistancePointToSegment(p, capsule.GetSegment());

    return distance <= sphere.GetRadius() + capsule.GetRadius();
}
bool collision::IsCollidedSphereAndCapsule       (const Sphere&      sphere,         const Capsule&      capsule)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSphereAndCapsule(sphere, capsule, intersection);
}

/// @brief 球とモデルの衝突判定 
bool collision::IsCollidedSphereAndModel         (const Sphere&      sphere,         const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_Sphere(model_handle, -1, sphere.GetPos(), sphere.GetRadius());
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        hit_triangles.emplace_back(Triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]));

        triangles[i] = hit_triangles.at(i);
        distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToSphere(hit_triangles.at(i), sphere)));
    }

    // 距離が最も近い三角形との交点を取得
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.at(loop_count) = triangles.at(dist.first);
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsCollidedTriangleAndSphere(hit_triangles.at(0), sphere, intersection);
}
bool collision::IsCollidedSphereAndModel         (const Sphere&      sphere,         const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles{};
    return IsCollidedSphereAndModel(sphere, model_handle, intersection, hit_triangles);
}
bool collision::IsCollidedSphereAndModel         (const Sphere&      sphere,         const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedSphereAndModel(sphere, model_handle, intersection);
}

/// @brief カプセルとカプセルの衝突判定
bool collision::IsCollidedCapsuleAndCapsule      (const Capsule&     capsule1,       const Capsule&      capsule2,       std::optional<VECTOR>& intersection)
{
    intersection = std::nullopt;

    const float distance = math::GetDistanceSegmentToSegment(capsule1.GetSegment(), capsule2.GetSegment());
    return distance <= capsule1.GetRadius() + capsule2.GetRadius();
}
bool collision::IsCollidedCapsuleAndCapsule      (const Capsule&     capsule1,       const Capsule&      capsule2)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedCapsuleAndCapsule(capsule1, capsule2, intersection);
}

/// @brief カプセルとモデルの衝突判定
bool collision::IsCollidedCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle,   std::optional<VECTOR>& intersection, std::vector<Triangle>& hit_triangles)
{
    hit_triangles.clear();

    const auto hit_result = MV1CollCheck_Capsule(model_handle, -1, capsule.GetSegment().GetBeginPos(), capsule.GetSegment().GetEndPos(), capsule.GetRadius());
    if (!hit_result.HitNum)
    {
        intersection = std::nullopt;
        return false;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        hit_triangles.emplace_back(Triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]));

        triangles[i] = hit_triangles.at(i);
        distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToCapsule(hit_triangles.at(i), capsule)));
    }

    // 三角形を距離が近い順にソート
    distance = algorithm::Sort(distance, SortKind::kAscending);
    int loop_count = 0;
    for (const auto& dist : distance)
    {
        hit_triangles.at(loop_count) = triangles.at(dist.first);
        ++loop_count;
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return collision::IsCollidedTriangleAndCapsule(hit_triangles.at(0), capsule, intersection);
}
bool collision::IsCollidedCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle,   std::optional<VECTOR>& intersection)
{
    std::vector<Triangle> hit_triangles{};
    return IsCollidedCapsuleAndModel(capsule, model_handle, intersection, hit_triangles);
}
bool collision::IsCollidedCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle,   MV1_COLL_RESULT_POLY_DIM& hit_result)
{
    const Segment segment = capsule.GetSegment();
    hit_result = MV1CollCheck_Capsule(model_handle, -1, segment.GetBeginPos(), segment.GetEndPos(), capsule.GetRadius());
    return hit_result.HitNum;
}
bool collision::IsCollidedCapsuleAndModel        (const Capsule&     capsule,        const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedCapsuleAndModel(capsule, model_handle, intersection);
}

/// @brief 光線カプセルとモデルの衝突判定
bool collision::IsCollidedRayCapsuleAndModel     (const RayCapsule&  ray_capsule,    const int           model_handle,   std::optional<VECTOR>& intersection)
{
    intersection    = std::nullopt;
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        if (MV1CollCheck_Capsule(model_handle, -1, capsule.GetSegment().GetBeginPos(), capsule.GetSegment().GetEndPos(), capsule.GetRadius()).HitNum)
        {
            return true;
        }
        
        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsCollidedRayCapsuleAndModel     (const RayCapsule&  ray_capsule,    const int           model_handle,   MV1_COLL_RESULT_POLY_DIM& hit_result)
{
    Capsule capsule = ray_capsule.GetBeginCapsule();

    // 速度を分割
    VECTOR velocity = ray_capsule.GetDir() * ray_capsule.GetRayLength();
    velocity *= (1.0f / kRayCapsuleDivisionNum);

    for (int i = 0; i < kRayCapsuleDivisionNum + 1; ++i)
    {
        hit_result = MV1CollCheck_Capsule(model_handle, -1, capsule.GetSegment().GetBeginPos(), capsule.GetSegment().GetEndPos(), capsule.GetRadius());
        if (hit_result.HitNum)
        {
            return true;
        }

        capsule.Move(velocity);
    }

    return false;
}
bool collision::IsCollidedRayCapsuleAndModel     (const RayCapsule&  ray_capsule,    const int           model_handle)
{
    std::optional<VECTOR> intersection = std::nullopt;

    return IsCollidedRayCapsuleAndModel(ray_capsule, model_handle, intersection);
}

//bool collision::IsCollidedCircumferenceAndCapsuleLowPrecision(const Circle* circle, const Capsule* capsule)
//{
//    Plane plane = Plane(circle->GetPos(), circle->GetNormalVector(), 0);
//
//    if (!IsCollidedPlaneAndCapsule(&plane, capsule)) { return false; }
//
//    // 始点から平面の最短距離座標を取得
//    float distance1 = math::GetDistancePointToPlane(capsule->GetSegment()->GetBeginPosition(), &plane);
//    VECTOR v = plane.GetNormalVector() * distance1;
//    if (math::IsPointAheadOfPlane(capsule->GetSegment()->GetBeginPosition(), &plane))
//    {
//        v *= -1;
//    }
//    VECTOR p = capsule->GetSegment()->GetBeginPosition() + v;
//
//    // 斜辺の長さを取得
//    VECTOR hypotenuse_direction = capsule->GetSegment()->GetDir();
//    float angle1 = math::GetAngleBetweenTwoVector(capsule->GetSegment()->GetDir(), v);
//    float angle2 = math::GetAngleBetweenTwoVector(-capsule->GetSegment()->GetDir(), v);
//    float hypotenuse_length = distance1 / cos(angle1);
//    if (angle1 > angle2)
//    {
//        hypotenuse_length = distance1 / cos(angle2);
//        hypotenuse_direction *= -1;
//    }
//
//    // TODO : 欠陥あり
//    // カプセルの上部が少しだけ平面に衝突した場合や、カプセルが傾いて衝突した場合に計算がずれる
//    // 交点を取得し、半径を考慮し衝突判定を行う
//    VECTOR intersection = capsule->GetSegment()->GetBeginPosition() + hypotenuse_direction * hypotenuse_length;
//    float distance2 = VSize(circle->GetPos() - intersection);
//
//    return distance2 >= circle->GetRadius() - capsule->GetRadius()
//        && distance2 <= circle->GetRadius() + capsule->GetRadius();
//}
#pragma endregion


#pragma region 押し戻し(衝突時の有効な速度ベクトルを取得)
VECTOR collision::PushBackSphereAndTriangle (const VECTOR& velocity, const Sphere&  dynamic_sphere,  const Triangle& static_triangle)
{
    // 未来の球を取得
    Sphere future_sphere = dynamic_sphere;
    future_sphere.Move(velocity);

    //// 未来の座標と衝突しているかを判定
    //if (!IsCollidedTriangleAndSphere(static_triangle, dynamic_sphere))
    //{
    //    return velocity;
    //}

    // 未来の座標から生成されたカプセルと衝突しているかを判定
    if (!IsCollidedTriangleAndCapsule(static_triangle, Capsule(dynamic_sphere.GetPos(), future_sphere.GetPos(), dynamic_sphere.GetRadius())))
    {
        // 衝突していない場合、そのまま返す
        return velocity;
    }

    // 未来の座標と平面の距離を取得
    const Plane plane = Plane(static_triangle.GetCentroid(), static_triangle.GetNormalVector());
    VECTOR future_pos = future_sphere.GetPos();
    const float future_distance_to_square = math::GetDistancePointToTriangle(future_pos, static_triangle);

    // 線分の位置からどちら側に位置修正するべきか
    VECTOR closest_dir = plane.GetNormalVector();
    if (math::IsPointAheadOfPlane(future_pos, plane))
    {
        closest_dir *= -1;
    }

    // 本来の到達地点までのvelocityを取得
    future_pos += closest_dir * future_distance_to_square;
    future_pos += plane.GetNormalVector() * dynamic_sphere.GetRadius();
    return future_pos - dynamic_sphere.GetPos();
}

VECTOR collision::PushBackSphereAndModel    (const VECTOR& velocity, const Sphere&  dynamic_sphere,  const int       model_handle)
{
    VECTOR valid_velocity = velocity;

    // 未来の球を取得
    Sphere future_sphere = dynamic_sphere;
    future_sphere.Move(velocity);

    // 未来の座標から生成されたカプセルと衝突しているかを判定
    const auto hit_result = MV1CollCheck_Capsule(model_handle, -1, dynamic_sphere.GetPos(), dynamic_sphere.GetPos(), future_sphere.GetRadius());
    if (!hit_result.HitNum)
    {
        return velocity;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との現在の距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>	current_distance;
    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        Triangle triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]);

        triangles[i] = triangle;
        current_distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToSphere(triangle, dynamic_sphere)));
    }

    // 距離が近い順に押し戻す
    current_distance = algorithm::Sort(current_distance, SortKind::kAscending);
    for (const auto& distance : current_distance)
    {
        valid_velocity = collision::PushBackSphereAndTriangle(valid_velocity, dynamic_sphere, triangles.at(distance.first));
    }

    MV1CollResultPolyDimTerminate(hit_result);
    return valid_velocity;
}

VECTOR collision::PushBackCapsuleAndTriangle(const VECTOR& velocity, const Capsule& dynamic_capsule, const Triangle& static_triangle)
{
    // 未来のカプセルを取得
    auto future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    // 未来の座標と衝突しているかを判定
    if (!IsCollidedTriangleAndCapsule(static_triangle, future_capsule))
    {
        return velocity;
    }

    // 三角形の平面を定義
    const auto plane = Plane(static_triangle.GetCentroid(), static_triangle.GetNormalVector());

    // カプセルの線分の両端点を取得
    const auto current_begin_pos            = dynamic_capsule.GetSegment().GetBeginPos();
    const auto current_end_pos              = dynamic_capsule.GetSegment().GetEndPos();
    const auto future_begin_pos             = future_capsule .GetSegment().GetBeginPos();
    const auto future_end_pos               = future_capsule .GetSegment().GetEndPos();

    // 各点から平面への距離を計算
    auto       plane_to_begin_pos_distance  = math::GetDistancePointToPlane(future_begin_pos, plane);
    auto       plane_to_end_pos_distance    = math::GetDistancePointToPlane(future_end_pos,   plane);
    const auto is_begin_pos_ahead_of_plane  = math::IsPointAheadOfPlane(future_begin_pos, plane);
    const auto is_end_pos_ahead_of_plane    = math::IsPointAheadOfPlane(future_end_pos, plane);

    // 始点と終点のどちらを基準にするかを判定
    float penetration_depth         = 0.0f;
    float result_penetration_depth  = 0.0f;
    if (is_begin_pos_ahead_of_plane && is_end_pos_ahead_of_plane)
    {
        penetration_depth           = min(plane_to_begin_pos_distance, plane_to_end_pos_distance);
        result_penetration_depth    = dynamic_capsule.GetRadius() - penetration_depth;
    }
    else if (!is_begin_pos_ahead_of_plane && !is_end_pos_ahead_of_plane)
    {
        penetration_depth           = max(plane_to_begin_pos_distance, plane_to_end_pos_distance);
        result_penetration_depth    = dynamic_capsule.GetRadius() + penetration_depth;
    }
    else if (is_begin_pos_ahead_of_plane && !is_end_pos_ahead_of_plane)
    {
        penetration_depth           = plane_to_end_pos_distance;
        result_penetration_depth    = dynamic_capsule.GetRadius() + penetration_depth;
    }
    else
    {
        penetration_depth           = plane_to_begin_pos_distance;
        result_penetration_depth    = dynamic_capsule.GetRadius() + penetration_depth;
    }

    // 始点と終点のどちらが近かったかを判定
    const auto is_closest_begin_pos = result_penetration_depth == plane_to_begin_pos_distance ? true : false;

    // 貫通していない場合は元のvelocityを返す
    if (result_penetration_depth <= 0.0f)
    {
        return velocity;
    }

    // 移動方向と平面法線の関係を確認
    const auto move_dir = v3d::GetNormalizedV(velocity);
    const auto dot = VDot(move_dir, plane.GetNormalVector());

    // 平面に向かって移動していない場合は処理しない
    if (dot >= 0.0f)
    {
        return velocity;
    }

    // 押し戻し距離を計算（法線方向への最小移動距離）
    const auto push_back_distance   = result_penetration_depth / abs(dot);
    const auto push_back_vector     = move_dir * push_back_distance;

    // 押し戻し後の有効な移動ベクトル
    const auto valid_velocity       = velocity - push_back_vector;

    // 壁滑り処理
    // 押し戻されたベクトルを平面に投影して滑りベクトルを作成
    const auto slide_vector         = push_back_vector - plane.GetNormalVector() * VDot(push_back_vector, plane.GetNormalVector());

    // 最終的な移動ベクトル = 有効な移動 + 滑り移動
    auto result_velocity            = valid_velocity + slide_vector;
    const auto additional_pushback  = plane.GetNormalVector() * (dynamic_capsule.GetRadius() * 0.01f); // 1%のマージン

    // 結果の検証：最終位置で衝突が解消されているか確認
    auto result_capsule             = dynamic_capsule;
    result_capsule.Move(result_velocity);

    for (int i = 0; i < 3; ++i)
    {
        // まだ衝突している場合は追加の調整
        if (IsCollidedTriangleAndCapsule(static_triangle, result_capsule))
        {
            result_velocity += additional_pushback;
            result_capsule.Move(result_velocity);
            continue;
        }
        break;
    }

    ////平面に近い点を三角形がある平面上に投影し三角形に含まれていない場合、カプセルを沈める
    //const auto p = is_closest_begin_pos ? result_capsule.GetSegment().GetBeginPos() : result_capsule.GetSegment().GetEndPos();
    //if (!math::IsProjectedPointInTriangle(p, static_triangle))
    //{
    //    // MEMO : 「tmp」と名前が付いた変数はその場で破棄され使用用途はない
    //    float tmp_t1, tmp_t2;
    //    VECTOR tmp_h, closest_pos_on_edge1, closest_pos_on_edge2, closest_pos_on_edge3;
    //    std::vector<std::pair<VECTOR, float>> closest_pos;
    //
    //    // 各辺上のカプセルに最も近い座標を取得
    //    const auto distance1 = math::GetDistanceSegmentToSegment(result_capsule.GetSegment(), static_triangle.GetEdge(0), tmp_h, closest_pos_on_edge1, tmp_t1, tmp_t2);
    //    const auto distance2 = math::GetDistanceSegmentToSegment(result_capsule.GetSegment(), static_triangle.GetEdge(1), tmp_h, closest_pos_on_edge2, tmp_t1, tmp_t2);
    //    const auto distance3 = math::GetDistanceSegmentToSegment(result_capsule.GetSegment(), static_triangle.GetEdge(2), tmp_h, closest_pos_on_edge3, tmp_t1, tmp_t2);
    //    closest_pos.emplace_back(std::make_pair(closest_pos_on_edge1, distance1));
    //    closest_pos.emplace_back(std::make_pair(closest_pos_on_edge2, distance2));
    //    closest_pos.emplace_back(std::make_pair(closest_pos_on_edge3, distance3));
    //    closest_pos = algorithm::Sort(closest_pos, SortKind::kAscending);
    //
    //    const auto segment = Segment(closest_pos.at(0).first, static_triangle.GetNormalVector(), dynamic_capsule.GetSegment().GetLength());
    //
    //    std::optional<VECTOR> intersection = std::nullopt;
    //    const auto tmp_is_hit = IsCollidedSegmentAndCapsule(segment, result_capsule, intersection);
    //
    //    if (intersection)
    //    {
    //        DrawSphere3D(*intersection, 0.5f, 8, 0xffffff, 0xffffff, TRUE);
    //        DrawLine3D(segment.GetBeginPos(), segment.GetEndPos(), 0xffffff);
    //
    //        const auto offset = VSize(*intersection - segment.GetBeginPos());
    //        result_velocity -= static_triangle.GetNormalVector() * offset;
    //    }
    //}

    return result_velocity;
}

VECTOR collision::PushBackCapsuleAndSquare  (const VECTOR& velocity, const Capsule& dynamic_capsule, const Square&   static_square)
{
    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    // 未来の座標と衝突しているかを判定
    if (!IsCollidedSquareAndCapsule(static_square, future_capsule))
    {
        return velocity;
    }

    // 未来の座標と平面の距離を取得
    const Plane plane = Plane(static_square.GetCentroid(), static_square.GetNormalVector());
    VECTOR future_pos = future_capsule.GetSegment().GetBeginPos();
    const float future_distance_to_square = math::GetDistancePointToSquare(future_pos, static_square);

    // 線分の位置からどちら側に位置修正するべきか
    VECTOR closest_dir = plane.GetNormalVector();
    if (math::IsPointAheadOfPlane(future_pos, plane))
    {
        closest_dir *= -1;
    }

    // 本来の到達地点までのvelocityを取得
    future_pos += closest_dir * future_distance_to_square;
    future_pos += plane.GetNormalVector() * dynamic_capsule.GetRadius();
    return future_pos - dynamic_capsule.GetSegment().GetBeginPos();
}

VECTOR collision::PushBackCapsuleAndSphere(const VECTOR& velocity, const Capsule& dynamic_capsule, const Sphere& static_sphere)
{
    const auto segment          = dynamic_capsule.GetSegment();
    const auto segment_axis     = segment.GetEndPos() - segment.GetBeginPos();
    const auto squared_lenght   = segment.GetLength() * segment.GetLength();
    const auto center_to_begin  = static_sphere.GetPos() - dynamic_capsule.GetSegment().GetBeginPos();

    // カプセルの軸上での最近点のパラメータ t を計算 (0 ≤ t ≤ 1)
    float t = 0.0f;
    if (squared_lenght > 0.0f)
    {
        t = VDot(center_to_begin, segment_axis) / squared_lenght;
        t = max(0.0f, min(1.0f, t)); // 0-1にクランプ
    }

    // カプセル軸上の最近点
    const auto closest_pos = dynamic_capsule.GetSegment().GetBeginPos() + segment_axis * t;

    // 最近点から球の中心へのベクトル
    const auto delta    = static_sphere.GetPos() - closest_pos;
    const auto distance = VSize(delta);

    // 衝突判定用の半径の和
    const auto radius_sum = dynamic_capsule.GetRadius() + static_sphere.GetRadius();

    // 衝突していない場合は元の速度を返す
    if (distance >= radius_sum) { return velocity; }

    // めり込み量
    const auto penetration = radius_sum - distance;

    // 通常の押し戻し方向（球の中心からカプセルへ）
    // 完全に重なっている場合は上方向に押し戻す
    const auto push_dir = distance > math::kEpsilonLow ? delta * (-1.0f / distance) : VGet(0.0f, 1.0f, 0.0f);

    // 既存の速度と押し戻し速度を合成して返す
    return velocity + push_dir * penetration;
}

VECTOR collision::PushBackCapsuleAndCapsule(const VECTOR& velocity, const Capsule& dynamic_capsule, const Capsule& static_capsule)
{
    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    // 未来のカプセルと衝突しているかを判定
    if (!IsCollidedCapsuleAndCapsule(future_capsule, static_capsule))
    {
        return velocity;
    }

    VECTOR h1, h2;
    float tmp_t1, tmp_t2;
    const auto tmp_distance = math::GetDistanceSegmentToSegment(dynamic_capsule.GetSegment(), static_capsule.GetSegment(), h1, h2, tmp_t1, tmp_t2);
    
    const auto distance_v       = h1 - h2;
    const auto distance_square  = VDot(distance_v, distance_v);
    const auto distance         = sqrt(distance_square);
    const auto normalized_v     = distance > 0.0f ? v3d::GetNormalizedV(distance_v) : VGet(0.0f, 1.0f, 0.0f);

    // 押し戻しベクトル
    const auto penetration = dynamic_capsule.GetRadius() + static_capsule.GetRadius() - distance;
    const auto push_back_v = normalized_v * penetration;

    // 押し戻したあとの有効な移動ベクトル (スライド)
    const auto vn = VDot(velocity, normalized_v);

    // velocityからめり込み成分を削除する
    // vn >= 0 の場合は衝突面から離れているためそのまま返す
    return vn < 0.0f ? velocity - normalized_v * vn : velocity;
}

VECTOR collision::PushBackCapsuleAndOBB     (const VECTOR& velocity, const Capsule& dynamic_capsule, const OBB&      static_obb)
{
    VECTOR valid_velocity = velocity;
    std::vector<std::pair<box::SquareKind, float>> current_distance;
    std::vector<std::pair<box::SquareKind, float>> future_distance;

    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    for (int i = 0; i < BoxData::kSquareNum; ++i)
    {
        // 現在の座標とすべての四角形の距離を取得
        const auto current_dist = math::GetDistancePointToSquare(
            dynamic_capsule.GetSegment().GetBeginPos(), static_obb.GetSquare(static_cast<box::SquareKind>(i)));
        current_distance.emplace_back(std::make_pair(static_cast<box::SquareKind>(i), current_dist));

        // 未来の座標とすべての四角形の距離を取得
        const auto future_dist = math::GetDistancePointToSquare(
            future_capsule.GetSegment().GetBeginPos(), static_obb.GetSquare(static_cast<box::SquareKind>(i)));
        future_distance.emplace_back(std::make_pair(static_cast<box::SquareKind>(i), future_dist));
    }

    // 現在の距離が近い順にソート
    // 距離が同じ場合は、同じもの同士で未来の座標が近い順にソート
    current_distance = algorithm::Sort(current_distance, future_distance, SortKind::kAscending, SortKind::kAscending);

    // 移動前の座標と距離が近い四角形から順番に押し戻す
    for (const auto& dist : current_distance)
    {
        valid_velocity = PushBackCapsuleAndSquare(valid_velocity, dynamic_capsule, static_obb.GetSquare(static_cast<box::SquareKind>(dist.first)));
    }

    return valid_velocity;
}

VECTOR collision::PushBackCapsuleAndModel   (const VECTOR& velocity, const Capsule& dynamic_capsule, const int       model_handle)
{
    VECTOR valid_velocity = velocity;
    MV1_COLL_RESULT_POLY_DIM hit_result;

    // 未来のカプセルを取得
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);

    // 未来の座標と衝突しているかを判定
    if (!IsCollidedCapsuleAndModel(future_capsule, model_handle, hit_result))
    {
        return velocity;
    }

    // ヒットしたポリゴンから三角形を生成
    // 三角形との現在の距離を取得
    std::unordered_map<int, Triangle>   triangles;
    std::vector<std::pair<int, float>>  current_distance;
    std::vector<std::pair<int, float>>  current_angle;

    for (int i = 0; i < hit_result.HitNum; ++i)
    {
        Triangle triangle(hit_result.Dim[i].Position[0], hit_result.Dim[i].Position[2], hit_result.Dim[i].Position[1]);

        triangles[i] = triangle;
        current_distance.emplace_back(std::make_pair(i, math::GetDistanceTriangleToCapsule(triangle, dynamic_capsule)));
        current_angle   .emplace_back(std::make_pair(i, math::GetAngleBetweenTwoVector(v3d::GetNormalizedV(velocity), triangle.GetNormalVector())));
    }

    // 距離が近い順に押し戻す
    // 距離が同じであればvelocityと法線の角度が大きい順に押し出す
    current_distance = algorithm::Sort(current_distance, current_angle, SortKind::kAscending, SortKind::kDescending);

    for (const auto& sorted : current_distance)
    {
        valid_velocity = collision::PushBackCapsuleAndTriangle(valid_velocity, dynamic_capsule, triangles.at(sorted.first));
    }

    return valid_velocity;
}
#pragma endregion
