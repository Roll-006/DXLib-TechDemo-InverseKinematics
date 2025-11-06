#pragma once

enum class ShapeKind
{
	kNone,
	kPoint,			// 点
	kLine,			// 直線(無限に広がる線)
	kSegment,		// 線分
	kPlane,			// 平面(無限に広がる面)
	kTriangle,		// 三角形
	kSquare,		// 四角形
	kCircle,		// 円
	kCircumference,	// 円周
	kAABB,			// 軸平行境界ボックス
	kOBB,			// 有向境界ボックス
	kSphere,		// 球
	kCapsule,		// カプセル
	kRayCapsule,	// 光線カプセル
	kCone,			// 円錐
};
