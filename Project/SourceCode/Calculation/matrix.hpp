#pragma once
#include <nlohmann/json.hpp>
#include <DxLib.h>

inline MATRIX operator+ (const MATRIX& m1, const MATRIX& m2)	{ return MAdd(m1, m2); }
inline MATRIX operator* (const MATRIX& m1, const MATRIX& m2)	{ return MMult(m1, m2); }

inline MATRIX operator* (const MATRIX& m, const float scale)	{ return MScale(m, scale); }
inline MATRIX operator* (const float scale, const MATRIX& m)	{ return MScale(m, scale); }

inline MATRIX operator+=(MATRIX& m1, const MATRIX& m2)			{ m1 = m1 + m2; return m1; }
inline MATRIX operator*=(MATRIX& m1, const MATRIX& m2)			{ m1 = m1 * m2; return m1; }

template<typename ScaleT>
inline MATRIX operator*=(MATRIX& m, const ScaleT scale)			{ m = m * scale; return m; }

inline bool operator==(const MATRIX& m1, const MATRIX& m2)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (m1.m[i][j] != m2.m[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
inline bool operator!=(const MATRIX& m1, const MATRIX& m2) { return !(m1 == m2); }

namespace matrix
{
	inline void SetPos(MATRIX& mat, const VECTOR& pos)
	{
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
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
	inline void from_json(const nlohmann::json& data, MATRIX& matrix)
	{
		auto m = data.get<std::array<std::array<float, 4>, 4>>();
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				matrix.m[i][j] = m[i][j];
			}
		}
	}

	inline void to_json(nlohmann::json& data, const MATRIX& matrix)
	{
		std::array<std::array<float, 4>, 4> m{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = matrix.m[i][j];
			}
		}

		data = m;
	}
}
#pragma endregion
