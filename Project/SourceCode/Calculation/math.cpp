#include "../IncludeList/shape.hpp"
#include "../Part/transform.hpp"
#include "math.hpp"

#pragma region 変換
MATRIX math::ConvertQuaternionToRotMatrix(const Quaternion& q)
{
    MATRIX m = MGetIdent();

	//X軸
	m.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	m.m[0][1] =        2.0f * q.x * q.y + 2.0f * q.w * q.z;
	m.m[0][2] =        2.0f * q.x * q.z - 2.0f * q.w * q.y;

	//Y軸
	m.m[1][0] =        2.0f * q.x * q.y - 2.0f * q.w * q.z;
	m.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	m.m[1][2] =        2.0f * q.y * q.z + 2.0f * q.w * q.x;

	//Z軸
	m.m[2][0] =        2.0f * q.x * q.z + 2.0f * q.w * q.y;
	m.m[2][1] =        2.0f * q.y * q.z - 2.0f * q.w * q.x;
	m.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

	// 転置し列優先に対応
	return MTranspose(m);
}

Quaternion math::ConvertRotMatrixToQuaternion(const MATRIX& rot_matrix)
{
    MATRIX m = MTranspose(MGetRotElem(rot_matrix));

    // 最大成分を検索
    std::array<float, 4> e
    {
         m.m[0][0] - m.m[1][1] - m.m[2][2] + 1.0f,    // x
        -m.m[0][0] + m.m[1][1] - m.m[2][2] + 1.0f,    // y
        -m.m[0][0] - m.m[1][1] + m.m[2][2] + 1.0f,    // z
         m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f     // w
    };

    unsigned int max_index = 0;
    for (size_t i = 1; i < e.size(); ++i)
    {
        if (e.at(i) > e.at(max_index))
        {
            max_index = static_cast<unsigned int>(i);
        }
    }

    // 最大要素の値を算出
    std::array<float, 4> q{ 0.0f, 0.0f, 0.0f, 1.0f };
    const float v       = sqrtf(e.at(max_index)) * 0.5f;
    const float mult    = 0.25f / v;
    q.at(max_index)     = v;

    switch (static_cast<quat::AxisKind>(max_index))
    {
    case quat::AxisKind::kX:
        q.at(1) = (m.m[0][1] + m.m[1][0]) * mult;
        q.at(2) = (m.m[2][0] + m.m[0][2]) * mult;
        q.at(3) = (m.m[1][2] - m.m[2][1]) * mult;
        break;

    case quat::AxisKind::kY:
        q.at(0) = (m.m[0][1] + m.m[1][0]) * mult;
        q.at(2) = (m.m[1][2] + m.m[2][1]) * mult;
        q.at(3) = (m.m[2][0] - m.m[0][2]) * mult;
        break;

    case quat::AxisKind::kZ:
        q.at(0) = (m.m[2][0] + m.m[0][2]) * mult;
        q.at(1) = (m.m[1][2] + m.m[2][1]) * mult;
        q.at(3) = (m.m[0][1] - m.m[1][0]) * mult;
        break;

    case quat::AxisKind::kW:
        q.at(0) = (m.m[1][2] - m.m[2][1]) * mult;
        q.at(1) = (m.m[2][0] - m.m[0][2]) * mult;
        q.at(2) = (m.m[0][1] - m.m[1][0]) * mult;
        break;
    }

    return Quaternion(q.at(0), q.at(1), q.at(2), q.at(3));
}

MATRIX math::ConvertAxisToRotMatrix(const Axis& axis)
{
    MATRIX mat = MGetIdent();

    // right
    mat.m[0][0] = axis.x_axis.x;
    mat.m[0][1] = axis.x_axis.y;
    mat.m[0][2] = axis.x_axis.z;
    mat.m[0][3] = 0.0f;

    // up
    mat.m[1][0] = axis.y_axis.x;
    mat.m[1][1] = axis.y_axis.y;
    mat.m[1][2] = axis.y_axis.z;
    mat.m[1][3] = 0.0f;

    // forward
    mat.m[2][0] = axis.z_axis.x;
    mat.m[2][1] = axis.z_axis.y;
    mat.m[2][2] = axis.z_axis.z;
    mat.m[2][3] = 0.0f;

    mat.m[3][0] = 0.0f;
    mat.m[3][1] = 0.0f;
    mat.m[3][2] = 0.0f;
    mat.m[3][3] = 1.0f;

    return mat;
}

VECTOR math::ConvertAxisToEulerAngles(const Axis& axis)
{
    const float angle_x = GetAngleBetweenTwoVector(axis::GetWorldXAxis(), axis.x_axis);
    const float angle_y = GetAngleBetweenTwoVector(axis::GetWorldYAxis(), axis.y_axis);
    const float angle_z = GetAngleBetweenTwoVector(axis::GetWorldZAxis(), axis.z_axis);

    return VECTOR(angle_x, angle_y, angle_z);
}

VECTOR math::ConvertXYZRotMatrixToEulerAngles(const MATRIX& rot_matrix)
{
    VECTOR angle = v3d::GetZeroV();
    const MATRIX m = MGetRotElem(rot_matrix);

    GetMatrixXYZRotation(&m, &angle.x, &angle.y, &angle.z);

    return angle;
}

VECTOR math::ConvertXYZRotMatrixToEulerAngles(const MATRIX& rot_matrix, bool& is_gimbal_lock)
{
    VECTOR angle = v3d::GetZeroV();
    const MATRIX m = MGetRotElem(rot_matrix);

    is_gimbal_lock = GetMatrixXYZRotation(&m, &angle.x, &angle.y, &angle.z);

    return angle;
}

VECTOR math::ConvertZXYRotMatrixToEulerAngles(const MATRIX& rot_matrix)
{
    VECTOR angle = v3d::GetZeroV();
    const MATRIX m = MGetRotElem(rot_matrix);

    GetMatrixZXYRotation(&m, &angle.x, &angle.y, &angle.z);

    return angle;
}

Axis math::ConvertRotMatrixToAxis(const MATRIX& rot_matrix)
{
    // 回転行列を取得
    MATRIX rot = MGetRotElem(rot_matrix);

    VECTOR x_axis = VGet(rot.m[0][0], rot.m[0][1], rot.m[0][2]);
    VECTOR y_axis = VGet(rot.m[1][0], rot.m[1][1], rot.m[1][2]);
    VECTOR z_axis = VGet(rot.m[2][0], rot.m[2][1], rot.m[2][2]);

    return { v3d::GetNormalizedV(x_axis), v3d::GetNormalizedV(y_axis), v3d::GetNormalizedV(z_axis) };
}

MATRIX math::ConvertEulerAnglesToXYZRotMatrix(const VECTOR& angle)
{
    MATRIX mat = MGetIdent();
    CreateRotationXYZMatrix(&mat, angle.x, angle.y, angle.z);
    return MGetRotElem(mat);
}

MATRIX math::ConvertEulerAnglesToZXYRotMatrix(const VECTOR& angle)
{
    MATRIX mat = MGetIdent();
    CreateRotationZXYMatrix(&mat, angle.x, angle.y, angle.z);
    return MGetRotElem(mat);
}

MATRIX math::ConvertForwardToRotMatrix(const VECTOR& forward)
{
    const auto forward_n    = v3d::GetNormalizedV(forward);
    const auto right        = -math::GetNormalVector(forward_n, axis::GetWorldYAxis());
    const auto up           = math::GetNormalVector(forward_n, right);

    return math::ConvertAxisToRotMatrix(Axis(right, up, forward_n));
}
#pragma endregion


#pragma region 補間
//VECTOR math::GetInterpolatedPos(const VECTOR& begin_pos, const VECTOR& end_pos, const VECTOR& current_pos, const float interpolate_time)
//{
//    VECTOR pos = current_pos;
//    const auto distance_v = end_pos - begin_pos;
//    const auto dir        = v3d::GetNormalizedV(distance_v);
//    const auto move_speed = (VSize(distance_v) / interpolate_time) * FPS::GetDeltaTime();
//
//    //DrawFormatString(500,   0, 0xffffff, "begin_pos  : %f, %f, %f", begin_pos.x,  begin_pos.y,  begin_pos.z);
//    //DrawFormatString(500,  20, 0xffffff, "end_pos     : %f, %f, %f", end_pos.x,     end_pos.y,     end_pos.z);
//    //DrawFormatString(500,  40, 0xffffff, "current_pos : %f, %f, %f", current_pos.x, current_pos.y, current_pos.z);
//    //DrawFormatString(500,  60, 0xffffff, "dir         : %f, %f, %f", dir.x, dir.y, dir.z);
//    //DrawFormatString(500,  80, 0xffffff, "distance    : %f", VSize(distance_v));
//    //DrawFormatString(500, 100, 0xffffff, "move_speed  : %f", move_speed);
//
//    // いてい距離に近づいた場合は完全に一致させる
//    pos += dir * move_speed;
//    if (VSize(pos - end_pos) < move_speed * kStopThreshold)
//    {
//        pos = end_pos;
//    }
//
//    return pos;
//}

VECTOR math::GetApproachedVector(const VECTOR& current_v, const VECTOR& end_v, const float speed)
{
    if (VSize(end_v - current_v) < kEpsilonLow) { return end_v; }

    const auto distance_end_to_current  = end_v - current_v;
    const auto dir                      = v3d::GetNormalizedV(distance_end_to_current);
    auto       result_v                 = current_v + dir * speed;

    // 終了判定
    if (VSize(end_v - result_v) < speed * kStopThreshold)
    {
        result_v = end_v;
    }

    // 終了判定を抜けた場合、強制的に終了させる
    const auto distance_result_to_current = result_v - current_v;
    if (VDot(distance_result_to_current, distance_end_to_current) > VSquareSize(distance_end_to_current))
    {
        result_v = end_v;
    }

    return result_v;
}

Quaternion math::GetSlerp(const Quaternion& begin_q, const Quaternion& end_q, const float t)
{
    // 安全性向上のため正規化・クランプ
    const auto normalized_begin_q   = quat::GetNormalizedQuaternion(begin_q);
    const auto normalized_end_q     = quat::GetNormalizedQuaternion(end_q);
    const auto clamped_t            = std::clamp(t, 0.0f, 1.0f);

    ////// 相対回転を取得
    ////auto relative_q       = quat::GetConjugateQuaternion(normalized_begin_q) * normalized_end_q;
    ////auto relative_angle   = quat::GetAngle(relative_q);
    ////
    ////// 180°を超える場合は逆方向を選択
    ////if (relative_angle > DX_PI_F)
    ////{
    ////    relative_q.x   = -relative_q.x;
    ////    relative_q.y   = -relative_q.y;
    ////    relative_q.z   = -relative_q.z;
    ////    relative_q.w   = -relative_q.w;
    ////    relative_angle = DX_TWO_PI_F - relative_angle;
    ////}
    ////
    ////// 相対回転をt倍
    ////Quaternion interpolated_relative = quat::GetIdentityQuaternion();
    ////if (relative_angle > math::kEpsilonLow)
    ////{
    ////    const auto half_angle   = relative_angle * t * 0.5f;
    ////    const auto sin_half     = sin(half_angle);
    ////    const auto cos_half     = cos(half_angle);
    ////    const auto axis_length  = sqrt(relative_q.x * relative_q.x + relative_q.y * relative_q.y + relative_q.z * relative_q.z);
    ////
    ////    if (axis_length > math::kEpsilonLow)
    ////    {
    ////        const auto inverse_axis_lenght = sin_half / axis_length;
    ////        interpolated_relative.x = relative_q.x * inverse_axis_lenght;
    ////        interpolated_relative.y = relative_q.y * inverse_axis_lenght;
    ////        interpolated_relative.z = relative_q.z * inverse_axis_lenght;
    ////        interpolated_relative.w = cos_half;
    ////    }
    ////}
    ////
    ////return normalized_begin_q * interpolated_relative;

    // 角度算出用の内積を計算
    const float begin_size  = quat::GetSize(normalized_begin_q);
    const float end_size    = quat::GetSize(normalized_end_q);

    // 不正なクォータニオンは処理を中断
    if (begin_size == 0.0f || end_size == 0.0f)
    {
        return normalized_begin_q;
    }

    //float dot = (normalized_begin_q.x * normalized_end_q.x
    //    + normalized_begin_q.y * normalized_end_q.y
    //    + normalized_begin_q.z * normalized_end_q.z
    //    + normalized_begin_q.w * normalized_end_q.w)
    //    / (begin_size * end_size);
    //
    //// 最短経路を選択するために、内積が負の場合は符号を反転
    //Quaternion target_q = normalized_end_q;
    //if (dot < 0.0f)
    //{
    //    target_q.x = -normalized_end_q.x;
    //    target_q.y = -normalized_end_q.y;
    //    target_q.z = -normalized_end_q.z;
    //    target_q.w = -normalized_end_q.w;
    //    dot = -dot;
    //}
    //
    //// 一致していた場合は補間完了とする
    //if (dot > kOneThreshold)
    //{
    //    return target_q;
    //}
    //
    //const float w = acos(fabs(dot));
    //const float sin_w = sin(w);
    //
    //// sin_wが0に近い場合の処理
    //if (sin_w < 1e-6f)
    //{
    //    return normalized_begin_q;
    //}
    //
    //const float sin_t_w     = sin(t * w);
    //const float sin_inv_t_w = sin((1.0f - t) * w);
    //const float mult_q1     = sin_inv_t_w / sin_w;
    //const float mult_q2     = sin_t_w / sin_w;
    //
    //return Quaternion
    //{
    //    mult_q1 * normalized_begin_q.x + mult_q2 * target_q.x,
    //    mult_q1 * normalized_begin_q.y + mult_q2 * target_q.y,
    //    mult_q1 * normalized_begin_q.z + mult_q2 * target_q.z,
    //    mult_q1 * normalized_begin_q.w + mult_q2 * target_q.w
    //};
    
    // 内積を計算
    float dot = 
        normalized_begin_q.x * normalized_end_q.x + 
        normalized_begin_q.y * normalized_end_q.y +
        normalized_begin_q.z * normalized_end_q.z + 
        normalized_begin_q.w * normalized_end_q.w;
    
    // 回転角度での最短経路を選択
    Quaternion target_q = normalized_end_q;
    if (dot < 0.0f)
    {
        target_q.x = -normalized_end_q.x;
        target_q.y = -normalized_end_q.y;
        target_q.z = -normalized_end_q.z;
        target_q.w = -normalized_end_q.w;
        dot = -dot;
    }
    
    // ほぼ同じクォータニオンの場合は線形補間を使用
    if (dot > math::kOneThreshold)
    {
        // 線形補間 + 正規化
        Quaternion result = {
            normalized_begin_q.x + clamped_t * (target_q.x - normalized_begin_q.x),
            normalized_begin_q.y + clamped_t * (target_q.y - normalized_begin_q.y),
            normalized_begin_q.z + clamped_t * (target_q.z - normalized_begin_q.z),
            normalized_begin_q.w + clamped_t * (target_q.w - normalized_begin_q.w)
        };
        return quat::GetNormalizedQuaternion(result);
    }
    
    // 球面線形補間
    const float theta       = acos(std::clamp(dot, 0.0f, 1.0f));
    const float sin_theta   = sin(theta);
    
    // 数値安定性チェック
    if (sin_theta < math::kEpsilonLow)
    {
        return normalized_begin_q;
    }
    
    const float sin_t_theta     = sin(clamped_t * theta);
    const float sin_inv_t_theta = sin((1.0f - clamped_t) * theta);
    const float mult_q1         = sin_inv_t_theta / sin_theta;
    const float mult_q2         = sin_t_theta / sin_theta;
    
    return Quaternion
    {
        mult_q1 * normalized_begin_q.x + mult_q2 * target_q.x,
        mult_q1 * normalized_begin_q.y + mult_q2 * target_q.y,
        mult_q1 * normalized_begin_q.z + mult_q2 * target_q.z,
        mult_q1 * normalized_begin_q.w + mult_q2 * target_q.w
    };
}

Transform& math::GetLerpTransform(
    Transform& begin_transform,
    Transform& end_transform,
    const float t, 
    const bool is_interpolate_pos, 
    const bool is_interpolate_scale, 
    const bool is_interpolate_rot)
{
    Transform result_transform = begin_transform;

    // 座標補間
    if (is_interpolate_pos)
    {
        const auto begin_pos    = begin_transform.GetPos(CoordinateKind::kWorld);
        const auto end_pos      = end_transform  .GetPos(CoordinateKind::kWorld);
        const auto result_pos   = GetLerp(begin_pos, end_pos, t);
        result_transform.SetPos(CoordinateKind::kWorld, result_pos);
    }

    // スケール補間
    if (is_interpolate_scale)
    {
        const auto begin_scale  = begin_transform.GetScale(CoordinateKind::kWorld);
        const auto end_scale    = end_transform  .GetScale(CoordinateKind::kWorld);
        const auto result_scale = GetLerp(begin_scale, end_scale, t);
        result_transform.SetScale(CoordinateKind::kWorld, result_scale);
    }

    // 回転補間
    if (is_interpolate_rot)
    {
        const auto begin_q  = begin_transform.GetQuaternion(CoordinateKind::kWorld);
        const auto end_q    = end_transform  .GetQuaternion(CoordinateKind::kWorld);
        const auto result_q = GetSlerp(begin_q, end_q, t);
        result_transform.SetRot(CoordinateKind::kWorld, result_q);
    }

    return result_transform;
}

float math::GetDampedValue(const float current_value, const float target_value, const float damping, const float delta_time)
{
    // damping値が0に近い場合は即座に目標値に設定
    if (damping <= kEpsilonLow)
    {
        return target_value;
    }

    // 減衰周波数を取得
    float omega = 1.0f / damping;

    // テイラー展開による近似式
    // 高速化のためstd::exp(-x)は避ける
    float x = omega * delta_time;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

    return target_value + (current_value - target_value) * exp;
}

VECTOR math::GetDampedValue(const VECTOR& current_value, const VECTOR& target_value, const VECTOR& damping, const float delta_time)
{
    VECTOR damped_value = current_value;
    damped_value.x = GetDampedValue(current_value.x, target_value.x, damping.x, delta_time);
    damped_value.y = GetDampedValue(current_value.y, target_value.y, damping.y, delta_time);
    damped_value.z = GetDampedValue(current_value.z, target_value.z, damping.z, delta_time);
    return damped_value;
}

VECTOR math::GetDampedValueOnAxis(const VECTOR& current_value, const VECTOR& target_value, const VECTOR& damping, const Axis& parent_axis, const float delta_time)
{
    VECTOR damped_value         = current_value;
    const auto distance         = target_value - current_value;

    // カメラの軸に分解（内積）
    // distanceが各軸にどれだけ含まれているかをを取得
    const auto forward_dot      = VDot(distance, parent_axis.z_axis);
    const auto right_dot        = VDot(distance, parent_axis.x_axis);
    const auto up_dot           = VDot(distance, parent_axis.y_axis);

    // 各軸の移動量に対して減衰を適用
    const auto damped_forward   = math::GetDampedValue(0.0f, forward_dot, damping.z, delta_time);
    const auto damped_right     = math::GetDampedValue(0.0f, right_dot,   damping.x, delta_time);
    const auto damped_up        = math::GetDampedValue(0.0f, up_dot,      damping.y, delta_time);

    // 減衰移動ベクトルを再合成
    const VECTOR damped_move =
        parent_axis.z_axis * damped_forward +
        parent_axis.x_axis * damped_right +
        parent_axis.y_axis * damped_up;

    damped_value += damped_move;

    return damped_value;
}
#pragma endregion


#pragma region 修正
//MATRIX math::GetCorrectedMatrix(const MATRIX& origin_m, const VECTOR& offset_pos, const VECTOR& offset_angle, const VECTOR& offset_scale)
//{
//    const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(offset_angle);
//    const auto result_mat       = offset_angle_mat * origin_m;
//
//    // 情報を適用
//    //m_transform->SetMatrix(CoordinateKind::kWorld, result_mat);
//    //m_transform->SetPos(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(offset_pos, result_mat));
//    //m_transform->SetScale(CoordinateKind::kWorld, offset_scale);
//}
#pragma endregion


#pragma region 三平方の定理
float math::GetAdjacentLengthIsoscelesRightTriangle(const float hypotenuse_length)
{
    return sqrt(hypotenuse_length * hypotenuse_length * 0.5f);
}

float math::GetHypotenuseLengthIsoscelesRightTriangle(const float adjacent_length)
{
    return sqrt(adjacent_length * adjacent_length + adjacent_length * adjacent_length);
}

float math::GetAdjacentLengthRightTriangle(const float opposite_length, const float hypotenuse_length)
{
    return sqrt(hypotenuse_length * hypotenuse_length - opposite_length * opposite_length);
}

float math::GetHypotenuseLengthRightTriangle(const float adjacent_length, const float opposite_length)
{
    return sqrt(adjacent_length * adjacent_length + opposite_length * opposite_length);
}

float math::GetHypotenuseLengthRightTriangleFromAdjacent(const float adjacent_length, const float theta)
{
    if (theta == 0.0f) { return 0.0f; }
    return adjacent_length / cos(theta);
}

float math::GetHypotenuseLengthRightTriangleFromOpposite(const float opposite_length, const float theta)
{
    if (theta == 0.0f) { return 0.0f; }
    return opposite_length / sin(theta);
}
#pragma endregion


#pragma region ベクトル
bool math::IsHorizontal(const VECTOR& v1, const VECTOR& v2)
{
    VECTOR cross = VCross(v1, v2);

    // 小数点第5位より後は誤差とみなして四捨五入
    cross = RoundOff(cross, -5);
    return cross == v3d::GetZeroV();
}

bool math::IsVertical(const VECTOR& v1, const VECTOR& v2)
{
    float dot = VDot(v1, v2);

    // 小数点第5位より後は誤差とみなして四捨五入
    dot = RoundOff(dot, -5);
    return dot == 0.0f;
}

VECTOR math::GetNormalVector(const VECTOR& v)
{
    VECTOR normal_v = v3d::GetZeroV();

    if (v != axis::GetWorldXAxis())
    {
        normal_v = math::GetNormalVector(v, axis::GetWorldXAxis());
    }
    else if (v != axis::GetWorldYAxis())
    {
        normal_v = math::GetNormalVector(v, axis::GetWorldYAxis());
    }
    else if (v != axis::GetWorldZAxis())
    {
        normal_v = math::GetNormalVector(v, axis::GetWorldZAxis());
    }
    return v3d::GetNormalizedV(normal_v);
}

VECTOR math::GetNormalVector(const VECTOR& v1, const VECTOR& v2)
{
    // [法線ベクトル = v1とv2の外積]から成分を取得
    return v3d::GetNormalizedV(VCross(v1, v2));
}

VECTOR math::GetProjectionVector(const VECTOR& projected_v, const VECTOR& base_v)
{
    // 投影先ベクトルが0の場合はゼロベクトルとする
    if (base_v == v3d::GetZeroV()) { return v3d::GetZeroV(); }

    const float dot         = VDot(base_v, projected_v);
    const float square_base = VSquareSize(base_v);
    return square_base != 0.0f ? (dot / square_base) * base_v : v3d::GetZeroV();
}
#pragma endregion


#pragma region 角度
bool math::IsAcuteAngle(const VECTOR& v1, const VECTOR& v2)
{
    const float radian = VDot(v1, v2) / (VSize(v1) * VSize(v2));
    return (radian < 90.0f * kDegToRad && radian > 0.0f);
}

bool math::IsAcuteAngle(const float radian)
{
    return (radian < 90.0f * kDegToRad && radian > 0.0f);
}

float math::GetAngleBetweenTwoVector(const VECTOR& v1, const VECTOR& v2)
{
    const auto length1 = VSize(v1);
    const auto length2 = VSize(v2);

    if (length1 == 0.0f || length2 == 0.0f) { return 0.0f; }

    auto dot = VDot(v1, v2) / (length1 * length2);
    dot = std::clamp(dot, -1.0f, 1.0f);

    return acos(dot);
}

double math::GetAngleBetweenTwoVectorD(const VECTOR_D& v1, const VECTOR_D& v2)
{
    const auto length1 = VSizeD(v1);
    const auto length2 = VSizeD(v2);

    if (length1 == 0.0f || length2 == 0.0f) { return 0.0f; }

    // 誤差を消し飛ばす
    auto dot = VDotD(v1, v2) / (length1 * length2);
    dot = std::clamp(dot, -1.0, 1.0);

    return acos(dot);
}

float math::GetYawBetweenTwoVector(const VECTOR& v1, const VECTOR& v2)
{
    const auto horizontal_v1 = v3d::GetNormalizedV(VGet(v1.x, 0.0f, v1.z));
    const auto horizontal_v2 = v3d::GetNormalizedV(VGet(v2.x, 0.0f, v2.z));

    return GetAngleBetweenTwoVector(horizontal_v1, horizontal_v2);
}
#pragma endregion


#pragma region 回転
VECTOR math::GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q)
{
	// 参考URL : [ https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7 ]

	const Quaternion q_pos(pos.x, pos.y, pos.z, 0.0f);
	const Quaternion n_rotate_q = quat::GetNormalizedQuaternion(rotate_q);
	const Quaternion totated_q  = n_rotate_q * q_pos * quat::GetInverseQuaternion(n_rotate_q);

	return VGet(totated_q.x, totated_q.y, totated_q.z);
}

float math::GetYaw(const VECTOR& v)
{
    return atan2f(v.x, v.z);
}

VECTOR math::GetYawRotVector(const VECTOR& v)
{
    return VGet(0.0f, GetYaw(v), 0.0f);
}

float math::GetPitch(const VECTOR& v)
{
    return atan2f(v.y, v.x);
    //return atan2f(v.y, sqrtf(v.x * v.x + v.z * v.z));
}

Axis math::GetAxis(const VECTOR& dir, const Axis& parent_axis)
{
    // directionを基準として各軸を取得
    const VECTOR local_dir_z = v3d::GetNormalizedV(dir);
    const VECTOR local_dir_x = math::GetNormalVector(parent_axis.y_axis, local_dir_z);
    const VECTOR local_dir_y = math::GetNormalVector(local_dir_z, local_dir_x);

    return Axis(local_dir_x, local_dir_y, local_dir_z);
}
#pragma endregion


#pragma region 重心
VECTOR math::GetCentroidOfATriangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3)
{
    VECTOR centroid = v3d::GetZeroV();
    centroid.x = (pos1.x + pos2.x + pos3.x) / 3.0f;
    centroid.y = (pos1.y + pos2.y + pos3.y) / 3.0f;
    centroid.z = (pos1.z + pos2.z + pos3.z) / 3.0f;
    return centroid;
}

VECTOR math::GetCentroidOfATriangle(const Triangle& triangle)
{
    const VECTOR pos1 = triangle.GetPos(0);
    const VECTOR pos2 = triangle.GetPos(1);
    const VECTOR pos3 = triangle.GetPos(2);
    return GetCentroidOfATriangle(pos1, pos2, pos3);
}

VECTOR math::GetCentroidOfAQuadrilateral(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4)
{
    const VECTOR centroid_triangle1 = GetCentroidOfATriangle(pos1, pos2, pos3);
    const VECTOR centroid_triangle2 = GetCentroidOfATriangle(pos3, pos4, pos1);
    const Line   line1(centroid_triangle1, v3d::GetNormalizedV(centroid_triangle2 - centroid_triangle1));

    const VECTOR centroid_triangle3 = GetCentroidOfATriangle(pos2, pos3, pos4);
    const VECTOR centroid_triangle4 = GetCentroidOfATriangle(pos4, pos1, pos2);
    const Line   line2(centroid_triangle3, v3d::GetNormalizedV(centroid_triangle4 - centroid_triangle3));

    // 四角形の時点でnulloptを返すことはないが、
    // 引数が正常でなかった場合nulloptである可能性があるため判定を行う
    const auto intersection = GetIntersectionLineAndLine(line1, line2);
    return intersection ? *intersection : centroid_triangle1;
}

VECTOR math::GetCentroidOfAQuadrilateral(const Square& square)
{
    const VECTOR pos1 = square.GetPos(0);
    const VECTOR pos2 = square.GetPos(1);
    const VECTOR pos3 = square.GetPos(2);
    const VECTOR pos4 = square.GetPos(3);
    return GetCentroidOfAQuadrilateral(pos1, pos2, pos3, pos4);
}
#pragma endregion


#pragma region 図形同士の関係
std::optional<VECTOR> math::GetIntersectionLineAndLine(const Line& line1, const Line& line2)
{
    //            pos1 + direction1 * t1 = pos2 + direction2 * t2
    // direction1 * t1 - direction2 * t2 = pos2 - pos1

    const VECTOR p1p2         = line2.GetPos() - line1.GetPos();
    const VECTOR cross_v      = VCross(line1.GetDir(), line2.GetDir());
    const float  cross_length = VDot(cross_v, cross_v);

    if (cross_length == 0.0f) { return std::nullopt; }

    const float t1 = VDot(VCross(p1p2, line1.GetDir()), cross_v) / cross_length;
    const float t2 = VDot(VCross(p1p2, line2.GetDir()), cross_v) / cross_length;

    const VECTOR p1 = line1.GetPoint(t1);
    const VECTOR p2 = line2.GetPoint(t2);

    if (VSize(p2 - p1) == 0.0f)
    {
        return p1;
    }
    return std::nullopt;
}

std::optional<VECTOR> math::GetIntersectionSegmentAndPlane(const Segment& segment, const Plane& plane)
{
    const VECTOR a = segment.GetBeginPos();
    const VECTOR b = segment.GetEndPos();
    const VECTOR ab = b - a;
    const VECTOR pa = a - plane.GetPos();
    const VECTOR pb = b - plane.GetPos();
    const VECTOR n = plane.GetNormalVector();

    if ((VDot(pa, n) >= 0 && VDot(pb, n) <= 0) || (VDot(pa, n) <= 0 && VDot(pb, n) >= 0))
    {
        const float dot_pa_n = std::abs(VDot(pa, n));
        const float dot_pb_n = std::abs(VDot(pb, n));

        // 同一平面上にある場合は不定値を返す
        return a + ab * (dot_pa_n / (dot_pa_n + dot_pb_n));
    }
    return std::nullopt;
}

//std::optional<VECTOR> math::GetIntersectionSegmentAndSphere(const Segment& segment, const Sphere& sphere)
//{
//    const auto diff     = segment.GetBeginPos() - sphere.GetPos();
//    const auto dot_proj = VDot(diff, segment.GetDir());
//
//    // 負なら球内部に含まれる
//    const auto diff_pos = VSquareSize(diff) - sphere.GetRadius() * sphere.GetRadius();
//
//    const auto distance = dot_proj * dot_proj - diff_pos;
//    if (distance < math::kEpsilonLow) { return std::nullopt; }
//
//    const auto sqrt_distance = sqrt(distance);
//    const auto t1 = -dot_proj - sqrt_distance;      // 入口
//    const auto t2 = -dot_proj + sqrt_distance;      // 出口
//
//    // 始点が球に含まれない場合、入口側の貫通点を採用
//    if (t1 >= 0.0f && t1 <= segment.GetLength())
//    {
//        return segment.GetBeginPos() + segment.GetDir() * t1;
//    }
//    // 始点が球に含まれる場合、出口側の貫通点を採用する
//    if (t2 >= 0.0f && t2 <= segment.GetLength())
//    {
//        return segment.GetBeginPos() + segment.GetDir() * t2;
//    }
//
//    return std::nullopt;
//}

//std::optional<VECTOR> math::GetIntersectionSegmentAndCapsule(const Segment& segment, const Capsule& capsule)
//{
//    const auto ab           = capsule.GetSegment().GetEndPos() - capsule.GetSegment().GetBeginPos();
//    const auto a0           = VSub(segment.GetBeginPos(), capsule.GetSegment().GetBeginPos());
//
//    const auto dot_ab_ab    = VDot(ab, ab);
//    const auto dot_ab_dir   = VDot(ab, segment.GetDir());
//    const auto dot_ab_a0    = VDot(ab, a0);
//    const auto dot_a0_dir   = VDot(a0, segment.GetDir());
//    const auto dot_a0_a0    = VDot(a0, a0);
//
//    const auto a            = dot_ab_ab * 1.0f       - dot_ab_dir * dot_ab_dir;
//    const auto b            = dot_ab_ab * dot_a0_dir - dot_ab_dir * dot_ab_a0;
//    const auto c            = dot_ab_ab * dot_a0_a0  - dot_ab_a0  * dot_ab_a0 - capsule.GetRadius() * capsule.GetRadius() * dot_ab_ab;
//
//    // 始点がカプセル内か判定
//    const auto is_inside = IsPointInsideCapsule(segment.GetBeginPos(), capsule);
//
//    // 交点候補
//    std::vector<std::pair<float, VECTOR>> intersection_candidates;
//
//    // --- 円柱部分 ---
//    const auto distance = b * b - a * c;
//    if (distance >= 0.0f && fabs(a) > 1e-6f)
//    {
//        const auto sqrtDisc = sqrtf(distance);
//        const auto t1 = (-b - sqrtDisc) / a;
//        const auto t2 = (-b + sqrtDisc) / a;
//
//        auto CheckAndAdd = [&](float t)
//        {
//            if (t < 0.0f || t > segment.GetLength()) { return; }
//
//            const auto pos = segment.GetBeginPos() + segment.GetDir() * t;
//            const auto proj = VDot(pos - capsule.GetSegment().GetBeginPos(), ab) / dot_ab_ab;
//            if (proj >= 0.0f && proj <= 1.0f)
//            {
//                intersection_candidates.emplace_back(t, pos);
//            }
//        };
//
//        CheckAndAdd(t1);
//        CheckAndAdd(t2);
//    }
//
//    // --- 球部分 ---
//    auto IntersectSphere = [&](const Sphere& sphere)
//    {
//        const auto diff     = segment.GetBeginPos() - sphere.GetPos();
//        const auto b        = VDot(diff, segment.GetDir());
//        const auto c        = VDot(diff, diff) - sphere.GetRadius() * sphere.GetRadius();
//        const auto distance = b * b - c;
//
//        if (distance < 0.0f) { return; }
//
//        const auto sqrtDisc = sqrtf(distance);
//        const auto t1       = -b - sqrtDisc;
//        const auto t2       = -b + sqrtDisc;
//
//        auto CheckAndAdd = [&](float t)
//        {
//            if (t >= 0.0f && t <= segment.GetLength())
//            {
//                const auto pos = segment.GetBeginPos() + segment.GetDir() * t;
//                intersection_candidates.emplace_back(t, pos);
//            }
//        };
//
//        CheckAndAdd(t1);
//        CheckAndAdd(t2);
//    };
//
//    IntersectSphere(Sphere(capsule.GetSegment().GetBeginPos(),  capsule.GetRadius()));
//    IntersectSphere(Sphere(capsule.GetSegment().GetEndPos(),    capsule.GetRadius()));
//
//    if (intersection_candidates.empty()) { return std::nullopt; }
//
//    // inside/outside に応じて選択
//    if (is_inside)
//    {
//        // 出口 → 最大の t
//        auto it = std::max_element(intersection_candidates.begin(), intersection_candidates.end(), [](auto& lhs, auto& rhs)
//        {
//            return lhs.first < rhs.first;
//        });
//
//        return it->second;
//    }
//    else
//    {
//        // 入口 → 最小の正の t
//        auto it = std::min_element(intersection_candidates.begin(), intersection_candidates.end(), [](auto& lhs, auto& rhs)
//        {
//            return lhs.first < rhs.first;
//        });
//
//        return it->second;
//    }
//
//    return std::nullopt;
//}

bool math::IsSameLine(const Line& line1, const Line& line2)
{
    if (!IsHorizontal(line1.GetDir(), line2.GetDir())) { return false; }

    const VECTOR direction = line2.GetDir() - line1.GetDir();
    return IsHorizontal(line1.GetDir(), direction) ? true : false;
}

VECTOR math::GetClosestPosOnPlaneFromSegment(const Plane& plane, const Segment& segment)
{
    const float distance1 = GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float distance2 = GetDistancePointToPlane(segment.GetEndPos(), plane);
    std::optional<VECTOR> closest_pos = GetIntersectionSegmentAndPlane(segment, plane);

    // 交点を持つ場合
    if (closest_pos)
    {
        return std::isfinite(closest_pos->x) ? *closest_pos : segment.GetBeginPos();
    }

    if (distance1 < distance2)
    {
        closest_pos = segment.GetBeginPos() + plane.GetNormalVector() * distance1;
        if (GetDistancePointToPlane(*closest_pos, plane) == 0.0f)
        {
            return *closest_pos;
        }
        return segment.GetBeginPos() - plane.GetNormalVector() * distance1;
    }

    closest_pos = segment.GetEndPos() + plane.GetNormalVector() * distance2;
    if (GetDistancePointToPlane(*closest_pos, plane) == 0.0f)
    {
        return *closest_pos;
    }
    return segment.GetEndPos() - plane.GetNormalVector() * distance2;
}

VECTOR math::GetClosestPosOnSegmentFromPlane(const Segment& segment, const Plane& plane)
{
    const VECTOR p = GetClosestPosOnPlaneFromSegment(plane, segment);
    const float distance = GetDistanceSegmentToPlane(segment, plane);

    const VECTOR closest_pos = p + plane.GetNormalVector() * distance;
    if (GetDistancePointToSegment(closest_pos, segment) == 0.0f)
    {
        return closest_pos;
    }
    return p - plane.GetNormalVector() * distance;
}

bool math::IsPointAheadOfPlane(const VECTOR& point, const Plane& plane)
{
    const VECTOR v = point - plane.GetPos();
    const float angle = GetAngleBetweenTwoVector(plane.GetNormalVector(), v);

    return angle < 90.0f * kDegToRad;
}

bool math::IsPointOnSphereSurface(const VECTOR& point, const Sphere& sphere)
{
    return VDot(point - sphere.GetPos(), point - sphere.GetPos()) == sqrt(sphere.GetRadius());
}

bool math::IsPointInsideCapsule(const VECTOR& point, const Capsule& capsule)
{
    const auto ab        = capsule.GetSegment().GetEndPos() - capsule.GetSegment().GetBeginPos();
    const auto dot_ab_ab = VDot(ab, ab);
    const auto t         = std::clamp(VDot(point - capsule.GetSegment().GetBeginPos(), ab) / dot_ab_ab, 0.0f, 1.0f);
    const auto closest   = capsule.GetSegment().GetBeginPos() + ab * t;
    const auto distance  = VDot(point - closest, point - closest);

    return distance <= capsule.GetRadius() * capsule.GetRadius();
}

bool math::IsProjectedPointInTriangle(const VECTOR& point, const Triangle& triangle)
{
    const auto n            = triangle.GetNormalVector();
    const auto square_size  = VSquareSize(n);

    // 退化
    if (square_size < math::kEpsilonLow) { return false; }

    // 投影
    const auto t            = VDot(point - triangle.GetPos(0), n) / square_size;
    const auto projected    = point - n * t;

    // 投影点が三角形の内側にあるかを判定するための符号付き面積
    const auto s1 = VDot(n, VCross(triangle.GetPos(1) - triangle.GetPos(0), projected - triangle.GetPos(0)));
    const auto s2 = VDot(n, VCross(triangle.GetPos(2) - triangle.GetPos(1), projected - triangle.GetPos(1)));
    const auto s3 = VDot(n, VCross(triangle.GetPos(0) - triangle.GetPos(2), projected - triangle.GetPos(2)));

    // 法線長に応じた許容値
    const auto tolerance = -square_size * math::kEpsilonLow;

    // すべて同じ符号なら三角形内に存在する
    return ((s1 >=  tolerance && s2 >=  tolerance && s3 >=  tolerance)
         || (s1 <= -tolerance && s2 <= -tolerance && s3 <= -tolerance));
}
#pragma endregion


#pragma region 図形
VECTOR math::GetRandomPointInCircle(const Circle& circle)
{
    // 半径ゼロなら乱数生成しない
    if (circle.GetRadius() <= 0.0f) { return circle.GetPos(); }

    const auto rand_gen = RandomGenerator::GetInstance();

    // 角度・距離を乱数で設定
    float angle     = DX_TWO_PI_F * (rand_gen->GetRandOpenClosed<float>(0.0f, DX_TWO_PI_F) / DX_TWO_PI_F);
    float distance  = circle.GetRadius() * (rand_gen->GetRandOpenClosed<float>(0.0f, circle.GetRadius()) / circle.GetRadius());

    // 座標を距離分移動させ、回転させる
    VECTOR pos = circle.GetPos() + math::GetNormalVector(-circle.GetNormalVector(), axis::GetWorldYAxis()) * distance;
    return math::GetRotatedPos(pos, quat::CreateQuaternion(circle.GetPos() + circle.GetNormalVector(), angle));
}
#pragma endregion


#pragma region 最短距離
float math::GetDistancePointToLine          (const VECTOR&      point,      const Line&     line)
{
    VECTOR h = v3d::GetZeroV();
    float  t = 0.0f;
    return GetDistancePointToLine(point, line, h, t);
}

float math::GetDistancePointToLine          (const VECTOR&      point,      const Line&     line,       VECTOR& h, float& t)
{
    const VECTOR dir = line.GetDir();
    t = 0.0f;

    if (VDot(dir, dir) > 0.0f)
    {
        t = VDot(dir, (point - line.GetPos())) / VDot(dir, dir);
    }

    h = line.GetPos() + dir * t;
    return VSize(h - point);
}

float math::GetDistancePointToSegment       (const VECTOR&      point,      const Segment&  segment)
{
    VECTOR h = v3d::GetZeroV();
    float  t = 0.0f;
    return GetDistancePointToSegment(point, segment, h, t);
}

float math::GetDistancePointToSegment       (const VECTOR&      point,      const Segment&  segment,    VECTOR& h, float& t)
{
    // MEMO : 点が線分の端点から伸びる垂線の内側にある ➡ 点と直線の最短距離を求める
    //                                   外側にある ➡ 端点までの距離を求める

    const Line   line       = Line(segment.GetBeginPos(), segment.GetDir());
    const float  distance   = GetDistancePointToLine(point, line, h, t);

    const VECTOR begin_to_point = segment.GetBeginPos() - point;
    const VECTOR begin_to_end   = segment.GetBeginPos() - segment.GetEndPos();
    const VECTOR end_to_point   = segment.GetEndPos()   - point;
    const VECTOR end_to_begin   = segment.GetEndPos()   - segment.GetBeginPos();

    // 始点側の外側であった場合
    if (!math::IsAcuteAngle(begin_to_point, begin_to_end))
    {
        return VSize(begin_to_point);
    }

    // 終点側の外側であった場合
    if (!math::IsAcuteAngle(end_to_point, end_to_begin))
    {
        return VSize(end_to_point);
    }

    // 内側であった場合
    return distance;
}

float math::GetDistancePointToPlane         (const VECTOR&      point,      const Plane&    plane)
{
    const VECTOR v1 = point - plane.GetPos();
    return std::abs(VDot(plane.GetNormalVector(), v1)) / VSize(plane.GetNormalVector());
}

float math::GetDistancePointToTriangle      (const VECTOR&      point,      const Triangle& triangle)
{
    return Triangle_Point_MinLength(triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2), point);
}

float math::GetDistancePointToSquare        (const VECTOR&      point,      const Square&   square)
{
    const float distance1 = GetDistancePointToTriangle(point, square.GetTriangle(0));
    const float distance2 = GetDistancePointToTriangle(point, square.GetTriangle(1));

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceLineToLine           (const Line&        line1,      const Line&     line2)
{
    VECTOR h1 = v3d::GetZeroV();
    VECTOR h2 = v3d::GetZeroV();
    float  t1 = 0.0f;
    float  t2 = 0.0f;
    return GetDistanceLineToLine(line1, line2, h1, h2, t1, t2);
}

float math::GetDistanceLineToLine           (const Line&        line1,      const Line&     line2,      VECTOR& h1, VECTOR& h2, float& t1, float& t2)
{
    // 平行であった場合
    if (math::IsHorizontal(line1.GetDir(), line2.GetDir()))
    {
        h1 = line1.GetPos();
        t1 = 0.0f;
        return GetDistancePointToLine(line1.GetPos(), line2, h2, t2);
    }

    // ねじれ関係であった場合
    const float  dot_v1v1 = VDot(line1.GetDir(), line1.GetDir());
    const float  dot_v1v2 = VDot(line1.GetDir(), line2.GetDir());
    const float  dot_v2v2 = VDot(line2.GetDir(), line2.GetDir());
    const VECTOR p21p11   = VSub(line1.GetPos(), line2.GetPos());

    t1 = (dot_v1v2 * VDot(line2.GetDir(), p21p11) - dot_v2v2 * VDot(line1.GetDir(), p21p11))
        / (dot_v1v1 * dot_v2v2 - dot_v1v2 * dot_v1v2);
    h1 = line1.GetPoint(t1);
    t2 = VDot(line2.GetDir(), h1 - line2.GetPos()) / dot_v2v2;
    h2 = line2.GetPoint(t2);

    return VSize(h2 - h1);
}

float math::GetDistanceSegmentToSegment     (const Segment&     segment1,   const Segment&  segment2)
{
    VECTOR h1 = v3d::GetZeroV();
    VECTOR h2 = v3d::GetZeroV();
    float  t1 = 0.0f;
    float  t2 = 0.0f;
    return GetDistanceSegmentToSegment(segment1, segment2, h1, h2, t1, t2);
}

float math::GetDistanceSegmentToSegment     (const Segment&     segment1,   const Segment&  segment2,   VECTOR& h1, VECTOR& h2, float& t1, float& t2)
{
    // どちらかの線分が縮退している場合
    if (segment1.GetLength() <= 0.0f || segment2.GetLength() <= 0.0f)
    {
        // どちらも縮退している場合
        if (segment1.GetLength() <= 0.0f && segment2.GetLength() <= 0.0f)
        {
            const float distance = VSize(segment2.GetBeginPos() - segment1.GetBeginPos());
            h1 = segment1.GetBeginPos();
            h2 = segment2.GetBeginPos();
            t1 = t2 = 0.0f;
            return distance;
        }

        // 線分1が縮退している場合
        if (segment1.GetLength() <= 0.0f)
        {
            const float distance = GetDistancePointToSegment(segment1.GetBeginPos(), segment2, h2, t2);
            h1 = segment1.GetBeginPos();
            t1 = 0.0f;
            t2 = std::clamp(t2, 0.0f, segment2.GetLength());
            return distance;
        }

        // 線分2が縮退している場合
        const float distance = GetDistancePointToSegment(segment2.GetBeginPos(), segment1, h1, t1);
        h2 = segment2.GetBeginPos();
        t2 = 0.0f;
        t1 = std::clamp(t1, 0.0f, segment1.GetLength());
        return distance;
    }

    // 平行であった場合
    if (math::IsHorizontal(segment1.GetDir(), segment2.GetDir()))
    {
        float distance = GetDistancePointToSegment(segment1.GetBeginPos(), segment2, h2, t2);
        h1 = segment1.GetBeginPos();
        t1 = 0.0f;
        if (t2 >= 0 && t2 <= segment2.GetLength())
        {
            return distance;
        }
    }
    // ねじれ関係であった場合
    else
    {
        Line line1 = Line(segment1.GetBeginPos(), segment1.GetDir());
        Line line2 = Line(segment2.GetBeginPos(), segment2.GetDir());
        float distance = GetDistanceLineToLine(line1, line2, h1, h2, t1, t2);
        if ((t1 >= 0.0f && t1 <= segment1.GetLength()) && (t2 >= 0.0f && t2 <= segment2.GetLength()))
        {
            return distance;
        }
    }

    // 垂線の足が外にある場合
    // 線分1をクランプ
    t1 = std::clamp(t1, 0.0f, segment1.GetLength());
    h1 = segment1.GetPoint(t1);
    float distance = GetDistancePointToSegment(h1, segment2, h2, t2);
    if (t2 >= 0.0f && t2 <= segment2.GetLength())
    {
        return distance;
    }

    // 線分2側が外であったため、線分2をクランプ
    t2 = std::clamp(t2, 0.0f, segment2.GetLength());
    h2 = segment2.GetPoint(t2);
    distance = GetDistancePointToSegment(h2, segment1, h1, t1);
    if (t1 >= 0.0f && t1 <= segment1.GetLength())
    {
        return distance;
    }

    // お互いの端点が最短であった場合
    t1 = std::clamp(t1, 0.0f, segment1.GetLength());
    h1 = segment1.GetPoint(t1);
    return VSize(h2 - h1);
}

float math::GetDistanceSegmentToPlane       (const Segment&     segment,    const Plane&    plane)
{
    const VECTOR v1 = plane.GetPos() - segment.GetBeginPos();
    const VECTOR v2 = plane.GetPos() - segment.GetEndPos();

    if (VDot(v1, plane.GetNormalVector()) * VDot(v2, plane.GetNormalVector()) <= 0)
    {
        return 0.0f;
    }

    const float distance1 = GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float distance2 = GetDistancePointToPlane(segment.GetEndPos(), plane);

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceSegmentToTriangle    (const Segment&     segment,    const Triangle& triangle)
{
    return Segment_Triangle_MinLength(segment.GetBeginPos(), segment.GetEndPos(),
        triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2));
}

float math::GetDistanceSegmentToSquare      (const Segment&     segment,    const Square&   square)
{
    const float distance1 = GetDistanceSegmentToTriangle(segment, square.GetTriangle(0));
    const float distance2 = GetDistanceSegmentToTriangle(segment, square.GetTriangle(1));

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistancePlaneToCapsule       (const Plane&       plane,      const Capsule&  capsule)
{
    const float distance = GetDistanceSegmentToPlane(capsule.GetSegment(), plane) - capsule.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceTriangleToTriangle   (const Triangle&    triangle1,  const Triangle& triangle2)
{
    return Triangle_Triangle_MinLength(
        triangle1.GetPos(0), triangle1.GetPos(1), triangle1.GetPos(2),
        triangle2.GetPos(0), triangle2.GetPos(1), triangle2.GetPos(2));
}

float math::GetDistanceTriangleToSquare     (const Triangle&    triangle,   const Square&   square)
{
    const float distance1 = GetDistanceTriangleToTriangle(triangle, square.GetTriangle(0));
    const float distance2 = GetDistanceTriangleToTriangle(triangle, square.GetTriangle(1));

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceTriangleToSphere(const Triangle& triangle, const Sphere& sphere)
{
    const float distance = GetDistancePointToTriangle(sphere.GetPos(), triangle) - sphere.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceTriangleToCapsule(const Triangle& triangle, const Capsule& capsule)
{
    const float distance = GetDistanceSegmentToTriangle(capsule.GetSegment(), triangle) - capsule.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceSquareToSquare       (const Square&      square1,    const Square&   square2)
{
    const float distance1 = GetDistanceTriangleToSquare(square1.GetTriangle(0), square2);
    const float distance2 = GetDistanceTriangleToSquare(square1.GetTriangle(1), square2);

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceSquareToCapsule      (const Square&      square,     const Capsule&  capsule)
{
    const float distance = GetDistanceSegmentToSquare(capsule.GetSegment(), square) - capsule.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceSphereToSphere(const Sphere& sphere1, const Sphere& sphere2)
{
    const float radius_sum  = sphere1.GetRadius() + sphere2.GetRadius();
    const float distance    = VSize(sphere1.GetPos() - sphere2.GetPos()) - radius_sum;
    return distance <= radius_sum ? 0.0f : distance;
}
#pragma endregion
