#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Kind/coordinate_kind.hpp"
#include "../Calculation/math.hpp"

class Transform final
{
public:
	Transform();
	Transform(Transform& transform);
	Transform(Transform* transform);
	~Transform();

	void Move(const CoordinateKind coord_kind, const VECTOR& velocity);


	#pragma region Attach / Detach
	void AttachParent(const std::shared_ptr<Transform>& parent_transform);
	void AttachParent(const std::string& parent_obj_name);
	void DetachParent();
	#pragma endregion


	#pragma region Setter
	void SetMatrix	(const CoordinateKind coord_kind, const MATRIX& matrix);
	void SetPos		(const CoordinateKind coord_kind, const VECTOR& pos);
	void SetRot		(const CoordinateKind coord_kind, const MATRIX& rot_matrix);
	void SetRot		(const CoordinateKind coord_kind, const Quaternion& quaternion);

	/// @brief forwardÇ©ÇÁâÒì]Çê›íË
	/// @biref WARNING : forwardÇ™ÉèÅ[ÉãÉhYé≤Ç…ïΩçsÇ≈Ç†Ç¡ÇΩèÍçáÇÕâÒì]Ç™é∏ÇÌÇÍÇÈ
	void SetRot		(const CoordinateKind coord_kind, const VECTOR& forward);
	void SetScale	(const CoordinateKind coord_kind, const VECTOR& scale);
	void SetScale	(const CoordinateKind coord_kind, const float	scale);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] int GetTransformHandle() const { return m_transform_handle; }
	[[nodiscard]] std::shared_ptr<Transform> GetParentTransform() const { return m_parent_transform; }

	[[nodiscard]] MATRIX	 GetMatrix			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetPos				(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX	 GetRotMatrix		(const CoordinateKind coord_kind);
	[[nodiscard]] Quaternion GetQuaternion		(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetScale			(const CoordinateKind coord_kind);
	[[nodiscard]] MATRIX	 GetScaleMatrix		(const CoordinateKind coord_kind);
	[[nodiscard]] Axis		 GetAxis			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetRight			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetUp				(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetForward			(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetXYZEulerAngles	(const CoordinateKind coord_kind);
	[[nodiscard]] VECTOR	 GetZXYEulerAngles	(const CoordinateKind coord_kind);

	[[nodiscard]] bool		 IsDirtyWorldMatrix()	const { return m_is_dirty_world_matrix; }
	[[nodiscard]] bool		 HasParent()			const { return m_parent_transform != nullptr; }
	#pragma endregion

private:
	int  m_transform_handle;
	int  m_parent_transform_handle;
	bool m_is_dirty_world_matrix;

	MATRIX m_local_matrix;
	MATRIX m_world_matrix;

	std::shared_ptr<Transform> m_parent_transform;

	friend void from_json(const nlohmann::json& data, Transform& transform);
	friend void to_json(nlohmann::json& data, const Transform& transform);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Transform& transform)
{
	data.at("local_matrix").get_to(transform.m_local_matrix);
}

inline void to_json(nlohmann::json& data, const Transform& transform)
{
	data = nlohmann::json
	{
		{ "local_matrix", transform.m_local_matrix }
	};
}
#pragma endregion
