#pragma once

namespace box
{
	enum class VertexKind
	{
		kBottomRightBack,	// 右下後方
		kBottomRightFront,	// 右下前方
		kBottomLeftFront,	// 左下前方
		kBottomLeftBack,	// 左下後方
		kTopRightBack,		// 右上後方
		kTopRightFront,		// 右上前方
		kTopLeftFront,		// 左上前方
		kTopLeftBack,		// 左上後方
	};

	enum class EdgeKind
	{
		kBottomFront,		// 前方下
		kBottomBack,		// 後方下
		kBottomRight,		// 右下
		kBottomLeft,		// 左下
		kTopFront,			// 前方上
		kTopBack,			// 後方上
		kTopRight,			// 右上
		kTopLeft,			// 左上
		kCenterRightBack,	// 中央右後方
		kCenterRightFront,	// 中央右前方
		kCenterLeftFront,	// 中央左前方
		kCenterLeftBack,	// 中央左後方
	};

	enum class SquareKind
	{
		kFront,				// 前
		kBack,				// 後
		kRight,				// 右
		kLeft,				// 左
		kBottom,			// 下
		kTop,				// 上
	};
}