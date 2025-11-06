#pragma once
#include <string>

struct AnimTimeKindData
{
	int   kind				= -1;
	int   attach_index		= -1;
	float play_timer		= 0.0f;
	float total_time		= 0.0f;
};

/// @brief アニメーションの個々のデータ
/// @brief is_self_blend 自分自身でブレンドを行うかを判定(最初と最後がループしていないアニメーションで使用)
struct AnimKindData
{
	int			anim_handle		= -1;
	int			index			= 0;
	std::string tag				= "";
	float		play_speed		= 0.0f;
	bool		is_loop			= true;
	bool		is_self_blend	= false;
};
