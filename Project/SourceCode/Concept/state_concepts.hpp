#pragma once
#include "obj_concepts.hpp"

template<obj_concepts::CharacterT CharacterT>
class IState;

namespace state_concepts
{
    /// @brief ステート型である
    template<typename T, typename CharacterT>
    concept StateT = std::is_base_of_v<IState<CharacterT>, T> && obj_concepts::CharacterT<CharacterT>;
}
