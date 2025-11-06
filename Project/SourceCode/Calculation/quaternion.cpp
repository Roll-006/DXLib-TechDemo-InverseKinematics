#include "quaternion.hpp"

Quaternion quat::ConvertFloat4ToQuaternion(const FLOAT4& f)
{
	return Quaternion(f.x, f.y, f.z, f.w);
}

FLOAT4 quat::ConvertQuaternionToFloat4(const Quaternion& q)
{
	return FLOAT4(q.x, q.y, q.z, q.w);
}

Quaternion quat::GetZeroQuaternion()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

Quaternion quat::GetIdentityQuaternion()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion quat::CreateQuaternion(const VECTOR& axis, const float angle)
{
	return ConvertFloat4ToQuaternion(QTRot(v3d::GetNormalizedV(axis), angle));
}

float quat::GetSize(const Quaternion& q)
{
	return sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

float quat::GetSquareSize(const Quaternion& q)
{
	return GetSize(q) * GetSize(q);
}

Quaternion quat::GetNormalizedQuaternion(const Quaternion& q)
{
	const float size = GetSize(q);
	return size != 0 ? Quaternion(q.x / size, q.y / size, q.z / size, q.w / size) : q;
}

float quat::GetDot(const Quaternion& q1, const Quaternion& q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

float quat::GetAngle(const Quaternion& q)
{
	return 2.0f * acos(std::clamp(std::abs(q.w), 0.0f, 1.0f));
}

Quaternion quat::GetConjugateQuaternion(const Quaternion& q)
{
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

Quaternion quat::GetInverseQuaternion(const Quaternion& q)
{
	return GetConjugateQuaternion(q) * (1.0f / GetSquareSize(q));
}
