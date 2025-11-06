#pragma once
#include <DxLib.h>

class MaskCreator final
{
public:
	/// @brief マスクスクリーンを生成する
	/// @brief UseMaskを使用する前にこの関数を使用する必要あり
	void CreateMask();

	/// @brief マスクスクリーンを削除する
	/// @brief 同スクリーンで複数回マスクを使用する場合、
	/// @brief 最後のマスクの使用が終了した後にこの関数を使用する
	void DeleteMask();

	/// @brief マスクを使用する
	/// @brief この関数を呼び出した後にスクリーンに反映する画像・図形を描画する
	/// @param is_invert_mask マスクをかける位置を反転させるかどうか
	void UseMask(const int graphic_handle, const bool is_invert_mask);

	/// @brief マスクの使用を終了する
	/// @brief スクリーンに反映する画像・図形を描画した後にこの関数を呼び出す
	void UnuseMask();

private:
	static bool m_is_invert_mask;
};
