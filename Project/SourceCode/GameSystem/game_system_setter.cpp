#include "game_system_setter.hpp"

GameSystemSetter::GameSystemSetter()
{
	SetUpGameSystem();

	RandomGenerator	::Generate();
}

GameSystemSetter::~GameSystemSetter()
{
	RandomGenerator	::Delete();

	Effkseer_End();
	DxLib_End();
}

void GameSystemSetter::SetUpGameSystem()
{
	// 基本設定
	SetGraphMode(Window::kScreenSize.x, Window::kScreenSize.y, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) { exit(EXIT_FAILURE); }
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (Effekseer_Init(8000) == -1) { exit(EXIT_FAILURE); }
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	SetMainWindowText("3DGAME");

	// Zバッファ
	SetUseZBuffer3D  (TRUE);
	SetWriteZBuffer3D(TRUE);

	// フォント
	SetFontUseAdjustSizeFlag(FALSE);
}
