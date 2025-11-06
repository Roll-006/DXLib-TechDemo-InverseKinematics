#pragma once
#include <unordered_map>
#include "../Base/singleton_base.hpp"

class HandleCreator final : public SingletonBase<HandleCreator>
{
public:
	/// @brief ステートハンドルを生成
	[[nodiscard]] int CreateHandle() { return ++m_handle_create_count; }

private:
	HandleCreator() {}
	~HandleCreator() override {}

private:
	int m_handle_create_count;	// ハンドル生成用カウント

	friend SingletonBase<HandleCreator>;
};
