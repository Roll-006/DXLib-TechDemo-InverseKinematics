#pragma once

/// @brief コライダーおよびトリガーの種類
enum class ColliderKind
{
	kNone = -1,

	kCollider,				// コライダー(押し戻し用)
	kMeshTrigger,			// モデルが持つメッシュをトリガーに利用
	kRay,					// 光線(レイキャスト)
	kProjectRay,			// 投影用光線

	kAttackTrigger,			// 攻撃判定用トリガー
	kLandingTrigger,		// 着地判定用トリガー
	kNearVisionTrigger,		// 視界判定用トリガー(近距離)
	kMiddleVisionTrigger,	// 視界判定用トリガー(中距離)
	kFarVisionTrigger,		// 視界判定用トリガー(遠距離)
	kVisibleTrigger,		// 視認判定を受けるトリガー

	kCollisionAreaTrigger,	// 衝突判定を許可するエリアに利用するトリガー
	kReactionTrigger,		// リアクショントリガー

	kHeadTrigger,			// 頭部トリガー
	kUpBodyTrigger,			// 胴体の上側トリガー
	kDownBodyTrigger,		// 胴体の下側トリガー
	kLeftUpperArmTrigger,	// 左上腕トリガー
	kRightUpperArmTrigger,	// 右上腕トリガー
	kLeftForearmTrigger,	// 左前腕トリガー
	kRightForearmTrigger,	// 右前腕トリガー
	kLeftHandTrigger,		// 左手トリガー
	kRightHandTrigger,		// 右手トリガー
	kLeftUpLegTrigger,		// 左脚の上側(太もも部分)トリガー
	kRightUpLegTrigger,		// 右脚の上側(太もも部分)トリガー
	kLeftDownLegTrigger,	// 左脚の下側(スネ・ふくらはぎ部分)トリガー
	kRightDownLegTrigger,	// 右脚の下側(スネ・ふくらはぎ部分)トリガー
};
