#pragma once
#include <nlohmann/json.hpp>
#include <DxLib.h>

inline MATRIX operator+ (const MATRIX& mat1, const MATRIX& mat2)	{ return MAdd (mat1, mat2); }
inline MATRIX operator* (const MATRIX& mat1, const MATRIX& mat2)	{ return MMult(mat1, mat2); }

inline MATRIX operator* (const MATRIX& mat, const float scale)		{ return MScale(mat, scale); }
inline MATRIX operator* (const float scale, const MATRIX& mat)		{ return MScale(mat, scale); }

inline MATRIX operator+=(MATRIX& mat1, const MATRIX& mat2)			{ mat1 = mat1 + mat2; return mat1; }
inline MATRIX operator*=(MATRIX& mat1, const MATRIX& mat2)			{ mat1 = mat1 * mat2; return mat1; }

template<typename ScaleT>
inline MATRIX operator*=(MATRIX& mat, const ScaleT scale)			{ mat = mat * scale; return mat; }

inline bool operator==(const MATRIX& mat1, const MATRIX& mat2)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (mat1.m[i][j] != mat2.m[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
inline bool operator!=(const MATRIX& mat1, const MATRIX& mat2) { return !(mat1 == mat2); }

namespace matrix
{
	/// @brief XŽ²‰ñ“](ƒsƒbƒ`Ž²‰ñ“])‚ðcosƒÆAsinƒÆ‚©‚ç¶¬
	[[nodiscard]] inline MATRIX CreateXMatrix(const float cos_theta, const float sin_theta)
	{
		auto mat = MGetIdent();
		mat.m[0][0] = 1.0f;			mat.m[0][1] = 0.0f;			mat.m[0][2] = 0.0f;			mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;			mat.m[1][1] =  cos_theta;	mat.m[1][2] = sin_theta;	mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;			mat.m[2][1] = -sin_theta;	mat.m[2][2] = cos_theta;	mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;			mat.m[3][1] = 0.0f;			mat.m[3][2] = 0.0f;			mat.m[3][3] = 1.0f;
		return mat;
	}

	/// @brief YŽ²‰ñ“](ƒˆ[Ž²‰ñ“])‚ðcosƒÆAsinƒÆ‚©‚ç¶¬
	[[nodiscard]] inline MATRIX CreateYMatrix(const float cos_theta, const float sin_theta)
	{
		auto mat = MGetIdent();
		mat.m[0][0] = cos_theta;	mat.m[0][1] = 0.0f;			mat.m[0][2] = -sin_theta;	mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;			mat.m[1][1] = 1.0f;			mat.m[1][2] = 0.0f;			mat.m[1][3] = 0.0f;
		mat.m[2][0] = sin_theta;	mat.m[2][1] = 0.0f;			mat.m[2][2] =  cos_theta;	mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;			mat.m[3][1] = 0.0f;			mat.m[3][2] = 0.0f;			mat.m[3][3] = 1.0f;
		return mat;
	}

	/// @brief ZŽ²‰ñ“](ƒ[ƒ‹Ž²‰ñ“])‚ðcosƒÆAsinƒÆ‚©‚ç¶¬
	[[nodiscard]] inline MATRIX CreateZMatrix(const float cos_theta, const float sin_theta)
	{
		auto mat = MGetIdent();
		mat.m[0][0] =  cos_theta;	mat.m[0][1] = sin_theta;	mat.m[0][2] = 0.0f;			mat.m[0][3] = 0.0f;
		mat.m[1][0] = -sin_theta;	mat.m[1][1] = cos_theta;	mat.m[1][2] = 0.0f;			mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;			mat.m[2][1] = 0.0f;			mat.m[2][2] = 1.0f;			mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;			mat.m[3][1] = 0.0f;			mat.m[3][2] = 0.0f;			mat.m[3][3] = 1.0f;
		return mat;
	}

	inline void SetRot(MATRIX& mat, const MATRIX& rot_mat)
	{
		mat.m[0][0] = rot_mat.m[0][0];
		mat.m[1][0] = rot_mat.m[1][0];
		mat.m[2][0] = rot_mat.m[2][0];

		mat.m[0][1] = rot_mat.m[0][1];
		mat.m[1][1] = rot_mat.m[1][1];
		mat.m[2][1] = rot_mat.m[2][1];

		mat.m[0][2] = rot_mat.m[0][2];
		mat.m[1][2] = rot_mat.m[1][2];
		mat.m[2][2] = rot_mat.m[2][2];
	}

	inline void SetPos(MATRIX& mat, const VECTOR& pos)
	{
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
	}

	/// @brief s—ñ‚ÌƒXƒP[ƒ‹‚ðŽæ“¾
	[[nodiscard]] inline VECTOR GetScale(const MATRIX& mat)
	{
		const auto scale_x = VGet(mat.m[0][0], mat.m[0][1], mat.m[0][2]);
		const auto scale_y = VGet(mat.m[1][0], mat.m[1][1], mat.m[1][2]);
		const auto scale_z = VGet(mat.m[2][0], mat.m[2][1], mat.m[2][2]);

		return { VSize(scale_x), VSize(scale_y), VSize(scale_z) };
	}
	
	[[nodiscard]] inline MATRIX GetRotMatrix(const MATRIX& mat)
	{
		const auto scale		= GetScale(mat);
		const auto scale_rot_m	= MGetRotElem(mat);

		auto rot_m = scale_rot_m;
		rot_m.m[0][0] /= scale.x; rot_m.m[0][1] /= scale.x; rot_m.m[0][2] /= scale.x;
		rot_m.m[1][0] /= scale.y; rot_m.m[1][1] /= scale.y; rot_m.m[1][2] /= scale.y;
		rot_m.m[2][0] /= scale.z; rot_m.m[2][1] /= scale.z; rot_m.m[2][2] /= scale.z;
		return rot_m;
	}

	inline void Draw(const int x, const int y, const MATRIX& mat)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				int pos_x = x + j * 200;
				int pos_y = y + i * 20;
				DrawFormatString(pos_x, pos_y, 0xffffff, "%f", mat.m[i][j]);
			}
		}
	}
}


#pragma region from / to JSON
namespace DxLib
{
	inline void from_json(const nlohmann::json& data, MATRIX& mat)
	{
		auto m = data.get<std::array<std::array<float, 4>, 4>>();
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat.m[i][j] = m[i][j];
			}
		}
	}

	inline void to_json(nlohmann::json& data, const MATRIX& mat)
	{
		std::array<std::array<float, 4>, 4> m{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = mat.m[i][j];
			}
		}

		data = mat;
	}
}
#pragma endregion
