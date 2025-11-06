#pragma once
#include <string>
#include <functional>

#include "../Kind/collider_kind.hpp"

struct ColliderData
{
	std::string  owner_tag	= "";
	ColliderKind kind		= ColliderKind::kNone;
};

inline bool operator==(const ColliderData& data1, const ColliderData& data2)
{
	return data1.owner_tag == data2.owner_tag && data1.kind == data2.kind;
}

inline bool operator!=(const ColliderData& data1, const ColliderData& data2)
{
	return !(data1 == data2);
}

namespace std
{
    template<>
    struct hash<ColliderData>
    {
        std::size_t operator()(const ColliderData& data) const noexcept
        {
            std::size_t h1 = std::hash<std::string>()(data.owner_tag);
            std::size_t h2 = std::hash<int>()(static_cast<int>(data.kind));
            return h1 ^ (h2 << 1);
        }
    };
}
