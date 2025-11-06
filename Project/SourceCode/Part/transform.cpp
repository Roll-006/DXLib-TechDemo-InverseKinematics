#include "transform.hpp"
#include "../Manager/obj_manager.hpp"
#include "../Handle/handle_creator.hpp"

Transform::Transform() :
	m_transform_handle			(HandleCreator::GetInstance()->CreateHandle()),
	m_parent_transform_handle	(-1),
	m_is_dirty_world_matrix		(true),
	m_local_matrix				(MGetIdent()),
	m_world_matrix				(MGetIdent()),
	m_parent_transform			(nullptr)
{
	// 処理なし
}

Transform::Transform(Transform& transform) :
	m_transform_handle			(transform.GetTransformHandle()),
	m_parent_transform_handle	(-1),
	m_is_dirty_world_matrix		(false),
	m_local_matrix				(transform.GetMatrix(CoordinateKind::kLocal)),
	m_world_matrix				(transform.GetMatrix(CoordinateKind::kWorld)),
	m_parent_transform			(transform.GetParentTransform())
{
	if (m_parent_transform)
	{
		m_parent_transform_handle = m_parent_transform->GetTransformHandle();
	}
}

Transform::Transform(Transform* transform) :
	m_transform_handle			(transform->GetTransformHandle()),
	m_parent_transform_handle	(-1),
	m_is_dirty_world_matrix		(false),
	m_local_matrix				(transform->GetMatrix(CoordinateKind::kLocal)),
	m_world_matrix				(transform->GetMatrix(CoordinateKind::kWorld)),
	m_parent_transform			(transform->GetParentTransform())
{
	if (m_parent_transform)
	{
		m_parent_transform_handle = m_parent_transform->GetTransformHandle();
	}
}

Transform::~Transform()
{
	// 処理なし
}

void Transform::Move(const CoordinateKind coord_kind, const VECTOR& velocity)
{
	SetPos(coord_kind, GetPos(coord_kind) + velocity);
}


#pragma region Attach / Detach
void Transform::AttachParent(const std::shared_ptr<Transform>& parent_transform)
{
	// 親がいない場合のみアタッチ
	if (m_parent_transform) { return; }

	m_parent_transform			= parent_transform;
	m_parent_transform_handle	= m_parent_transform->GetTransformHandle();

	// ワールド座標を保持したままローカル座標に変換
	const MATRIX world_mat = m_world_matrix;
	m_local_matrix = world_mat * MInverse(m_parent_transform->GetMatrix(CoordinateKind::kWorld));

	m_is_dirty_world_matrix = true;
}

void Transform::AttachParent(const std::string& parent_obj_name)
{
	// 親がいない場合のみアタッチ
	if (m_parent_transform) { return; }

	const auto parent_obj		= ObjManager::GetInstance()->GetObj<ObjBase>(parent_obj_name);
	const auto parent_transform = parent_obj->GetTransform();

	AttachParent(parent_transform);
}

void Transform::DetachParent()
{
	// 親がいる場合のみデタッチ
	if (!m_parent_transform) { return; }

	// ワールド座標を保持
	m_local_matrix		= m_world_matrix;
	m_parent_transform	= nullptr;
	m_parent_transform_handle = -1;

	// ワールド座標は変わらないのでdirtyフラグは立てない
}
#pragma endregion


#pragma region Setter
void Transform::SetMatrix(const CoordinateKind coord_kind, const MATRIX& matrix)
{
	switch (coord_kind)
	{
	case CoordinateKind::kLocal:
		m_local_matrix			= matrix;
		m_is_dirty_world_matrix = true;
		break;

	case CoordinateKind::kWorld:
		// ワールド行列を計算
		m_world_matrix = matrix;

		// ローカル行列を計算
		if (m_parent_transform)
		{
			m_local_matrix = m_world_matrix * MInverse(m_parent_transform->GetMatrix(CoordinateKind::kWorld));
		}
		else
		{
			m_local_matrix = matrix;
		}
		break;
	}
}

void Transform::SetPos(const CoordinateKind coord_kind, const VECTOR& pos)
{
	switch (coord_kind)
	{
	case CoordinateKind::kLocal:
		matrix::SetPos(m_local_matrix, pos);
		m_is_dirty_world_matrix = true;
		break;

	case CoordinateKind::kWorld:
		// ワールド行列を計算
		matrix::SetPos(m_world_matrix, pos);

		// ローカル行列を計算
		if (m_parent_transform)
		{
			const VECTOR parent_pos = m_parent_transform->GetPos(CoordinateKind::kWorld);
			matrix::SetPos(m_local_matrix, pos - parent_pos);
		}
		else
		{
			matrix::SetPos(m_local_matrix, pos);
		}
		break;
	}
}

void Transform::SetRot(const CoordinateKind coord_kind, const MATRIX& rot_matrix)
{
	switch (coord_kind)
	{
	case CoordinateKind::kLocal:
		m_local_matrix			= rot_matrix * MInverse(GetRotMatrix(CoordinateKind::kLocal)) * m_local_matrix;
		m_is_dirty_world_matrix = true;
		break;

	case CoordinateKind::kWorld:
		// ワールド行列を計算
		m_world_matrix = rot_matrix * MInverse(GetRotMatrix(CoordinateKind::kWorld)) * m_world_matrix;

		// ローカル行列を計算
		if (m_parent_transform)
		{
			m_local_matrix = m_world_matrix * MInverse(m_parent_transform->GetMatrix(CoordinateKind::kWorld));
		}
		else
		{
			m_local_matrix = m_world_matrix;
		}
		break;
	}
}

void Transform::SetRot(const CoordinateKind coord_kind, const Quaternion& quaternion)
{
	SetRot(coord_kind, math::ConvertQuaternionToRotMatrix(quaternion));
}

void Transform::SetRot(const CoordinateKind coord_kind, const VECTOR& forward)
{
	SetRot(coord_kind, math::ConvertForwardToRotMatrix(forward));
}

void Transform::SetScale(const CoordinateKind coord_kind, const VECTOR& scale)
{
	const MATRIX scale_m = MGetScale(scale);
	const MATRIX rot_m = GetRotMatrix(coord_kind);
	const MATRIX pos_m = MGetTranslate(GetPos(coord_kind));
	const MATRIX result_m = scale_m * rot_m * pos_m;

	switch (coord_kind)
	{
	case CoordinateKind::kLocal:
		m_local_matrix			= result_m;
		m_is_dirty_world_matrix = true;
		break;

	case CoordinateKind::kWorld:
		// ワールド行列を計算
		m_world_matrix = result_m;

		// ローカル行列を計算
		if (m_parent_transform)
		{
			m_local_matrix = m_world_matrix * MInverse(m_parent_transform->GetMatrix(CoordinateKind::kWorld));
		}
		else
		{
			m_local_matrix = result_m;
		}
		break;
	}
}

void Transform::SetScale(const CoordinateKind coord_kind, const float scale)
{
	SetScale(coord_kind, VGet(scale, scale, scale));
}
#pragma endregion


#pragma region Getter
MATRIX Transform::GetMatrix(const CoordinateKind coord_kind)
{
	switch (coord_kind)
	{
	case CoordinateKind::kLocal:
		return m_local_matrix;

	case CoordinateKind::kWorld:
		// キャッシュが古い場合は再計算
		if (m_is_dirty_world_matrix)
		{
			if (m_parent_transform)
			{
				m_world_matrix = m_local_matrix * m_parent_transform->GetMatrix(CoordinateKind::kWorld);
			}
			else
			{
				m_world_matrix = m_local_matrix;
			}
			m_is_dirty_world_matrix = false;
		}

		return m_world_matrix;
	}
}

VECTOR Transform::GetPos(const CoordinateKind coord_kind)
{
	const MATRIX mat = GetMatrix(coord_kind);
	return VGet(mat.m[3][0], mat.m[3][1], mat.m[3][2]);
}

MATRIX Transform::GetRotMatrix(const CoordinateKind coord_kind)
{
	return MGetRotElem(GetMatrix(coord_kind));
}

Quaternion Transform::GetQuaternion(const CoordinateKind coord_kind)
{
	return math::ConvertRotMatrixToQuaternion(GetRotMatrix(coord_kind));
}

VECTOR Transform::GetScale(const CoordinateKind coord_kind)
{
	const MATRIX mat = GetMatrix(coord_kind);

	const float scale_x = VSize(VGet(mat.m[0][0], mat.m[0][1], mat.m[0][2]));
	const float scale_y = VSize(VGet(mat.m[1][0], mat.m[1][1], mat.m[1][2]));
	const float scale_z = VSize(VGet(mat.m[2][0], mat.m[2][1], mat.m[2][2]));

	return VGet(scale_x, scale_y, scale_z);
}

MATRIX Transform::GetScaleMatrix(const CoordinateKind coord_kind)
{
	return MGetScale(GetScale(coord_kind));
}

VECTOR Transform::GetRight(const CoordinateKind coord_kind)
{
	return GetAxes(coord_kind).x_axis;
}

VECTOR Transform::GetUp(const CoordinateKind coord_kind)
{
	return GetAxes(coord_kind).y_axis;
}

VECTOR Transform::GetForward(const CoordinateKind coord_kind)
{
	return GetAxes(coord_kind).z_axis;
}

Axes Transform::GetAxes(const CoordinateKind coord_kind)
{
	return math::ConvertRotMatrixToAxes(GetRotMatrix(coord_kind));
}

VECTOR Transform::GetXYZEulerAngles(const CoordinateKind coord_kind)
{
	return math::ConvertXYZRotMatrixToEulerAngles(GetRotMatrix(coord_kind));
}

VECTOR Transform::GetZXYEulerAngles(const CoordinateKind coord_kind)
{
	return math::ConvertZXYRotMatrixToEulerAngles(GetRotMatrix(coord_kind));
}
#pragma endregion