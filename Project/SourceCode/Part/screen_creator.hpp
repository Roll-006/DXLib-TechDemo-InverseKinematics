#pragma once
#include "graphicer.hpp"

class ScreenCreator final
{
public:
	ScreenCreator(const Vector2D<int>& screen_size, const bool is_ransparent = true);
	ScreenCreator(const Vector2D<int>& screen_size, const Vector2D<int>& center_pos, const bool is_ransparent = true);
	~ScreenCreator();

	/// @brief 生成したスクリーンを使用する
	/// @brief この関数を呼び出した後にスクリーンに反映する画像・図形を描画する
	void UseScreen();

	/// @brief 生成したスクリーンの使用を終了する
	/// @brief スクリーンに反映する画像・図形を描画した後にこの関数を呼び出す
	void UnuseScreen();

	/// @brief 描画(GetGraphicer()->Draw()と同等の効果)
	/// @brief UnuseScreenを呼び出した後に呼び出す
	void Draw(const bool is_draw_graphic_frame = false) const;

	[[nodiscard]] std::shared_ptr<Graphicer> GetGraphicer()			const { return m_graphicer; }
	[[nodiscard]] Vector2D<int>				 GetScreenSize()		const { return m_screen_size; }
	[[nodiscard]] Vector2D<int>				 GetHalfScreenSize()	const { return Vector2D<int>(static_cast<int>(m_screen_size.x * 0.5f), static_cast<int>(m_screen_size.y * 0.5f)); }
	[[nodiscard]] int						 GetScreenHandle()		const { return m_screen_graphic_handle; }

private:
	void SaveCamera();
	void RestoreCamera();

private:
	std::shared_ptr<Graphicer>	m_graphicer;
	Vector2D<int>				m_screen_size;
	int							m_screen_graphic_handle;

	VECTOR						m_prev_camera_pos;
	VECTOR						m_prev_camera_target;
	float						m_prev_camera_near;
	float						m_prev_camera_far;
	float						m_prev_camera_fov;

	bool a;
};
