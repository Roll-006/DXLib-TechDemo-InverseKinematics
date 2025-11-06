#pragma once
#include <concepts>

class ObjBase;
class PhysicalObjBase;
class CharacterBase;
class IItem;
class WeaponBase;
class KnifeBase;
class GunBase;

namespace obj_concepts
{
	/// @brief オブジェクト型である
	template<typename T>
	concept ObjT			= std::is_base_of_v<ObjBase, T>;

	/// @brief 物理オブジェクト型である
	template<typename T>
	concept PhysicalObjT	= std::is_base_of_v<PhysicalObjBase, T>;

	/// @brief キャラオブジェクト型である
	template<typename T>
	concept CharacterT		= std::is_base_of_v<CharacterBase, T>;
}
