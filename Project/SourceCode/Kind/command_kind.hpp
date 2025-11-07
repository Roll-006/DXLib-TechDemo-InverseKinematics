#pragma once

enum class CommandKind
{
	#pragma region キー・パッド共通
	kDecide,							// 決定ボタン
	kBack,								// 戻るボタン
	kSelectUp,							// 選択(上)
	kSelectDown,						// 選択(下)
	kSelectLeft,						// 選択(左)
	kSelectRight,						// 選択(右)
	kPause,								// ポーズ画面切り替え

	kAimGun,							// 銃を構える
	kAttack,							// 攻撃
	kPullTrigger,						// 引き金を引く
	kAimKnife,							// ナイフを構える / (パリィ)
	kInvestigate,						// 調べる / (拾う)
	kMelee,								// メレー
	kEscape,							// 脱出
	kReload,							// リロード
	kScope,								// スコープ倍率変更
	kMoveUpPlayer,						// プレイヤー上移動
	kMoveDownPlayer,					// プレイヤー下移動
	kMoveLeftPlayer,					// プレイヤー左移動
	kMoveRightPlayer,					// プレイヤー右移動
	kRun,								// ダッシュ
	kCrouch,							// しゃがむ
	kQuickTurn,							// クイックターン

	kMoveUpCamera,						// カメラ上移動
	kMoveDownCamera,					// カメラ下移動
	kMoveLeftCamera,					// カメラ左移動
	kMoveRightCamera,					// カメラ右移動
	kInitAim,							// 視点リセット
	#pragma endregion


	#pragma region キー固有
	kSelectWeaponInsideUp,				// 内側上の武器を選択する
	kSelectWeaponInsideLeft,			// 内側左の武器を選択する
	kSelectWeaponInsideDown,			// 内側下の武器を選択する
	kSelectWeaponInsideRight,			// 内側右の武器を選択する
	kSelectWeaponOutsideUp,				// 外側上の武器を選択する
	kSelectWeaponOutsideLeft,			// 外側左の武器を選択する
	kSelectWeaponOutsideDown,			// 外側下の武器を選択する
	kSelectWeaponOutsideRight,			// 外側右の武器を選択する
	kSelectWeaponRotateLeft,			// 左回転で武器を選択する
	kSelectWeaponRotateRight,			// 右回転で武器を選択する
	kSideChangeWeapon,					// 武器ショートカットの内側 / 外側切り替え
	#pragma endregion


	#pragma region パッド固有
	kSelectWeaponUp,					// 上の武器を選択する
	kSelectWeaponLeft,					// 左の武器を選択する
	kSelectWeaponDown,					// 下の武器を選択する
	kSelectWeaponRight,					// 右の武器を選択する
	#pragma endregion
};

enum class CommandSlotKind
{
	kMain,		// メインスロット
	kSub,		// サブスロット
	kStatic1,	// 固定スロット1(ユーザーからの変更は受け付けない)
	kStatic2,	// 固定スロット2(ユーザーからの変更は受け付けない)
	kStatic3,	// 固定スロット2(ユーザーからの変更は受け付けない)
};
