#include "screen_creator.hpp"

ScreenCreator::ScreenCreator(const Vector2D<int>& screen_size, const bool is_ransparent) :
	m_screen_size			(screen_size),
	m_screen_graphic_handle	(MakeScreen(screen_size.x, screen_size.y, is_ransparent)),
	m_graphicer				(nullptr),
	m_prev_camera_pos		(v3d::GetZeroV()),
	m_prev_camera_target	(v3d::GetZeroV()),
	m_prev_camera_near		(0.0f),
	m_prev_camera_far		(0.0f),
	m_prev_camera_fov		(0.0f)
{
	m_graphicer = std::make_shared<Graphicer>(m_screen_graphic_handle);
}

ScreenCreator::ScreenCreator(const Vector2D<int>& screen_size, const Vector2D<int>& center_pos, const bool is_ransparent) :
	m_screen_size			(screen_size),
	m_screen_graphic_handle	(MakeScreen(screen_size.x, screen_size.y, is_ransparent)),
	m_graphicer				(nullptr),
	m_prev_camera_pos		(v3d::GetZeroV()),
	m_prev_camera_target	(v3d::GetZeroV()),
	m_prev_camera_near		(0.0f),
	m_prev_camera_far		(0.0f),
	m_prev_camera_fov		(0.0f)
{
	m_graphicer = std::make_shared<Graphicer>(m_screen_graphic_handle);
	m_graphicer->SetCenterPos(center_pos);
}

ScreenCreator::~ScreenCreator()
{
	DeleteGraph(m_screen_graphic_handle);
}

void ScreenCreator::UseScreen()
{
	SaveCamera();

	SetDrawScreen(m_screen_graphic_handle);
	ClearDrawScreen();

	RestoreCamera();
}

void ScreenCreator::UnuseScreen()
{
	SetDrawScreen(DX_SCREEN_BACK);
	RestoreCamera();
}

void ScreenCreator::Draw(const bool is_draw_graphic_frame) const
{
	m_graphicer->Draw(is_draw_graphic_frame);
}

void ScreenCreator::SaveCamera()
{
	// SetDrawScreenを使用した際に、カメラの設定が破棄されるため
	// 復元するためにカメラ情報を保存
	m_prev_camera_near		= GetCameraNear();
	m_prev_camera_far		= GetCameraFar();
	m_prev_camera_fov		= GetCameraFov();
	m_prev_camera_pos		= GetCameraPosition();
	m_prev_camera_target	= GetCameraTarget();
}

void ScreenCreator::RestoreCamera()
{
	// カメラ情報の復元
	SetCameraNearFar					(m_prev_camera_near, m_prev_camera_far);
	SetupCamera_Perspective				(m_prev_camera_fov);
	SetCameraPositionAndTarget_UpVecY	(m_prev_camera_pos, m_prev_camera_target);
}
