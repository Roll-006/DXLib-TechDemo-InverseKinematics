#pragma once
#include <nlohmann/json.hpp>
#include "../Concept/vector_concepts.hpp"

inline auto operator+ (const v3d_concepts::ValidT auto& v)									{ return v; }
template<v3d_concepts::ValidT VecT>
inline auto operator- (const VecT& v)														{ return VecT(-v.x, -v.y, -v.z); }

template<v3d_concepts::ValidT VecT>
inline auto operator+ (const VecT& v1, const VecT& v2)										{ return VecT(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
template<v3d_concepts::ValidT VecT>
inline auto operator- (const VecT& v1, const VecT& v2)										{ return VecT(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
template<v3d_concepts::ValidT VecT>
inline auto operator* (const VecT& v1, const VecT& v2)										{ return VecT(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

template<v3d_concepts::ValidT VecT, typename ScaleT>
inline auto operator* (const VecT& v, const ScaleT scale)									{ return VecT(v.x * scale, v.y * scale, v.z * scale); }
template<typename ScaleT>
inline auto operator* (const ScaleT scale, const v3d_concepts::ValidT auto& v)				{ return v * scale; }

inline auto operator+=(v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2)	{ v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }
inline auto operator-=(v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2)	{ v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }
inline auto operator*=(v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2)	{ v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }

template<typename ScaleT>
inline auto operator*=(v3d_concepts::ValidT auto& v, const ScaleT scale)						{ v.x *= scale; v.y *= scale; v.z *= scale; return v; }

inline bool operator==(const v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2){ return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
inline bool operator!=(const v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2){ return !(v1 == v2); }


namespace v3d
{
    [[nodiscard]] inline VECTOR GetZeroV()                      { return VGet(0.0f, 0.0f, 0.0f); }
    [[nodiscard]] inline VECTOR GetNormalizedV(const VECTOR& v) { return VSize(v) != 0.0f ? VNorm(v) : v; }
}


#pragma region from / to JSON
namespace DxLib
{
	inline void from_json(const nlohmann::json& data, VECTOR& vector)
	{
		data.at("x").get_to(vector.x);
		data.at("y").get_to(vector.y);
		data.at("z").get_to(vector.z);
	}

	inline void from_json(const nlohmann::json& data, VECTOR_D& vector)
	{
		data.at("x").get_to(vector.x);
		data.at("y").get_to(vector.y);
		data.at("z").get_to(vector.z);
	}

	inline void to_json(nlohmann::json& data, const VECTOR& vector)
	{
		data = nlohmann::json
		{
			{ "x",	vector.x },
			{ "y",	vector.y },
			{ "z",	vector.z }
		};
	}

	inline void to_json(nlohmann::json& data, const VECTOR_D& vector)
	{
		data = nlohmann::json
		{
			{ "x",	vector.x },
			{ "y",	vector.y },
			{ "z",	vector.z }
		};
	}
}
#pragma endregion
