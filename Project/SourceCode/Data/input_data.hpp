#pragma once
#include <functional>
#include <unordered_map>

#include "../IncludeList/vector.hpp"
#include "../Kind/input_kind.hpp"

struct InputCode
{
	InputKind	kind = InputKind::kKey;
	int			code = 0;
};

struct InputData
{
	bool  is_input		= false;
	float input_time	= 0.0f;
};

struct MouseData
{
	Vector2D<int>   pos				= v2d::GetZeroV<Vector2D<int>>();
	Vector2D<float> dir				= v2d::GetZeroV<Vector2D<float>>();
	Vector2D<float> velocity		= v2d::GetZeroV<Vector2D<float>>();
	int				wheel_rotation	= 0;
};


inline bool operator==(const InputCode& code1, const InputCode& code2)
{
    return code1.kind == code2.kind && code1.code == code2.code;
}

inline bool operator==(const InputData& data1, const InputData& data2)
{
    return data1.is_input == data2.is_input && data1.input_time == data2.input_time;
}

namespace std
{
    template <>
    struct hash<InputCode>
    {
        std::size_t operator()(const InputCode& code) const noexcept
        {
            // 適当なハッシュ結合
            std::size_t h1 = std::hash<InputKind>{}(code.kind);
            std::size_t h2 = std::hash<int>{}(code.code);

            // 結合方法：XORとシフト
            return h1 ^ (h2 << 1);
        }
    };
}

namespace std
{
    template <>
    struct hash<InputData>
    {
        std::size_t operator()(const InputData& data) const noexcept
        {
            // 適当なハッシュ結合
            std::size_t h1 = std::hash<bool>{}(data.is_input);
            std::size_t h2 = std::hash<float>{}(data.input_time);

            // 結合方法：XORとシフト
            return h1 ^ (h2 << 1);
        }
    };
}
