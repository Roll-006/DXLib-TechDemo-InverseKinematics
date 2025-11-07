#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <Matrix/matrix.hpp>
#include <Axis/axis.hpp>
#include <Vector/vector_2d.hpp>

#include "../Algorithm/algorithm.hpp"
#include "../Random/random_generator.hpp"

#include "../Concept/common_concepts.hpp"

#include "quaternion.hpp"

#include "../Kind/time_kind.hpp"
#include "../GameTime/fps.hpp"

class Point;
class Line;
class Segment;
class Plane;
class Triangle;
class Square;
class Circle;
class OBB;
class Sphere;
class Capsule;
class Cone;

class Transform;

namespace math
{
	static constexpr float kDegToRad		= DX_PI_F / 180.0f;		// ディグリーをラジアンに変換(変換対象と掛け算を行う)
	static constexpr float kRadToDeg		= 180.0f / DX_PI_F;		// ラジアンをディグリーに変換(変換対象と掛け算を行う)
	static constexpr float kEpsilonLow		= 1e-4f;				// 0とみなす(低精度)
	static constexpr float kEpsilonHigh		= 1e-6f;				// 0とみなす(高精度)
	static constexpr float kOneThreshold	= 0.9999f;				// 1とみなす閾値
	static constexpr float kStopThreshold	= 2.0f;


	#pragma region 変換
	/// @brief 四捨五入
	/// @param value 対象数字
	/// @param digit 概数を取る桁数。負の値は小数点以下を示すものとする。
	/// 
	/// 例1) value : 2.45, digit : -1　➡　return 2.5
	/// 
	/// 例2) value : 1.73, digit : 0   ➡　return 1.73(そのまま返す)
	template<typename T>
	[[nodiscard]] T RoundOff(const T& value, int digit)
	{
		// 0桁目は存在しないためそのまま返す
		if (digit == 0) { return value; }

		if (digit > 0) { --digit; }

		const float adjust = static_cast<float>(std::pow(10, digit));
		return std::round(value / adjust) * adjust;
	}

	template<>
	inline [[nodiscard]] VECTOR RoundOff(const VECTOR& value, int digit)
	{
		return VGet(RoundOff(value.x, digit), RoundOff(value.y, digit), RoundOff(value.z, digit));
	}

	/// @brief クォータニオンから回転行列(回転順は問わない)へ変換
	[[nodiscard]] MATRIX ConvertQuaternionToRotMatrix(const Quaternion& q);

	/// @brief 回転行列(回転順は問わない)からクォータニオンへ変換
	[[nodiscard]] Quaternion ConvertRotMatrixToQuaternion(const MATRIX& rot_matrix);

	/// @brief XYZ軸から回転行列(回転順は問わない)へ変換
	[[nodiscard]] MATRIX ConvertAxisToRotMatrix(const Axis& axis);

	/// @brief XYZ軸からオイラー角へ変換
	[[nodiscard]] VECTOR ConvertAxisToEulerAngles(const Axis& axis);

	/// @brief 回転行列からオイラー角へ変換
	/// @brief FIXME : 不具合ありな可能性あり。要検証
	[[nodiscard]] VECTOR ConvertXYZRotMatrixToEulerAngles(const MATRIX& rot_matrix);
	[[nodiscard]] VECTOR ConvertXYZRotMatrixToEulerAngles(const MATRIX& rot_matrix, bool& is_gimbal_lock);
	[[nodiscard]] VECTOR ConvertZXYRotMatrixToEulerAngles(const MATRIX& rot_matrix);

	/// @brief 回転行列(回転順は問わない)からXYZ軸へ変換
	[[nodiscard]] Axis ConvertRotMatrixToAxis(const MATRIX& rot_matrix);

	/// @brief オイラー角から回転行列へ変換
	[[nodiscard]] MATRIX ConvertEulerAnglesToXYZRotMatrix(const VECTOR& angle);
	[[nodiscard]] MATRIX ConvertEulerAnglesToZXYRotMatrix(const VECTOR& angle);

	/// @brief forwardから回転ベクトルを取得(Z軸回転なし)
	[[nodiscard]] MATRIX ConvertForwardToRotMatrix(const VECTOR& forward);
	#pragma endregion


	#pragma region 平均値
	/// @brief 値の平均を取得
	/// @brief 呼び出す際は「GetAverageValue<戻り値型>(値1, 値2...);」とする
	template<common_concepts::FloatingPointT ReturnT, common_concepts::ArithmeticT T, common_concepts::ArithmeticT... Args>
	inline [[nodiscard]] ReturnT GetAverageValue(T first, Args... args)
	{
		const int length = 1 + sizeof...(args);
		const ReturnT sum = static_cast<ReturnT>(first) + (static_cast<ReturnT>(args) + ...);

		return sum / length;
	}
	#pragma endregion


	#pragma region インクリメント / デクリメント
	/// @brief 値を増加させる
	/// @param value 増加させる値
	/// @param increase_value 増加量
	/// @param max_value 最大値
	/// @param is_loop ループ判定
	template<common_concepts::ArithmeticT T>
	void Increase(T& value, const T& increase_value, const T& max_value, const bool is_loop, const T& loop_value = 0)
	{
		value += increase_value;
		if (value > max_value)
		{
			value = is_loop ? loop_value : max_value;
		}
	}

	/// @brief 値を減少させる
	/// @param value 減少させる値
	/// @param decrease_value 減少量
	/// @param min_value 最小値
	template<common_concepts::ArithmeticT T>
	void Decrease(T& value, const T& decrease_value, const T& min_value)
	{
		if (value == min_value) { return; }

		value -= decrease_value;
		if (value < min_value)
		{
			value = min_value;
		}
	}
	#pragma endregion


	#pragma region 補間
	/// @brief 目的地に向かって移動したベクトルを取得する
	/// @param current_v 現在地点
	/// @param end_v 終了地点
	/// @param speed 移動速度(デルタタイム適用後)
	/// @return 移動後ベクトル
	[[nodiscard]] VECTOR GetApproachedVector(const VECTOR& current_v, const VECTOR& end_v, const float speed);
	
	/// @brief 2つの値を線形補間で補間
	/// @param begin_v 開始地点
	/// @param end_v 終了地点
	/// @param t 補間係数(0.0～1.0)
	/// @return 補間結果
	template<typename T>
	[[nodiscard]] T GetLerp(const T& begin_num, const T& end_num, const float t)
	{
		return static_cast<T>((1 - t) * begin_num + t * end_num);
	}

	template<>
	inline [[nodiscard]] VECTOR GetLerp(const VECTOR& begin_v, const VECTOR& end_v, const float t)
	{
		return begin_v + (end_v - begin_v) * t;
	}

	/// @brief 2つのクォータニオンを球面線形補間で補間
	/// @param begin_q 開始地点となるクォータニオン
	/// @param end_q 終了地点となるクォータニオン
	/// @param t 補間係数(0.0～1.0)
	/// @return 補間結果のクォータニオン
	[[nodiscard]] Quaternion GetSlerp(const Quaternion& begin_q, const Quaternion& end_q, const float t);
	
	/// @brief 2つのトランスフォームを線形補間で補間
	/// @brief 座標 : 線形補間, 回転 : 球面線形補間, スケール : 線形補間
	/// @param begin_scale 開始地点となるトランスフォーム
	/// @param end_scale 終了地点となるトランスフォーム
	/// @param t 補間係数(0.0～1.0)
	/// @param is_interpolate_pos 座標の補間を行うか
	/// @param is_interpolate_scale スケールの補間を行うか
	/// @param is_interpolate_rot 回転の補間を行うか
	/// @return 補間結果のトランスフォーム
	[[nodiscard]] Transform& GetLerpTransform(
		Transform& begin_transform,
		Transform& end_transform,
		const float t,
		const bool is_interpolate_pos,
		const bool is_interpolate_scale,
		const bool is_interpolate_rot);

	/// @brief 減衰後の値を取得
	/// @param current_value 現在の値
	/// @param target_value 目的の値
	/// @param damping 減衰値(時定数)
	/// @return 減衰後の値
	[[nodiscard]] float  GetDampedValue(const float   current_value, const float   target_value, const float   damping, const float delta_time);
	[[nodiscard]] VECTOR GetDampedValue(const VECTOR& current_value, const VECTOR& target_value, const VECTOR& damping, const float delta_time);

	/// @brief 特定の軸(forward, right, up)を基準にした減衰後の値を取得
	/// @param current_value 現在の値
	/// @param target_value 目的の値
	/// @param damping 減衰値(時定数)
	/// @param parent_axis 基準とする軸
	/// @return 減衰後の値
	[[nodiscard]] VECTOR GetDampedValueOnAxis(const VECTOR& current_value, const VECTOR& target_value, const VECTOR& damping, const Axis& parent_axis, const float delta_time);
	#pragma endregion


	#pragma region 値の修正
	/// @brief 値が指定の[-value～value]をループするように繋ぎ合わせる
	template<typename T>
	T ConnectMinusValueToValue(const T current_value, const T connect_value)
	{
		T connected_value = current_value;
		if (connected_value <= -connect_value) { connected_value += connect_value * 2; }
		if (connected_value >=  connect_value) { connected_value -= connect_value * 2; }
		return connected_value;
	}

	/// @brief 0～1の間に変換した値を取得
	/// @tparam TargetT 変換対象の型
	/// @tparam ReturnValue 戻り値の型
	/// @param min 値の最小値(この値を0とする)
	/// @param max 値の最大値(この値を1とする)
	/// @param value 変換対象の値
	template<common_concepts::ArithmeticT TargetT, common_concepts::FloatingPointT ReturnT>
	[[nodiscard]] ReturnT GetUnitValue(const TargetT min, const TargetT max, const TargetT value)
	{
		return static_cast<ReturnT>(value - min) / (max - min);
	}

	/// @brief 特定の範囲にあった値を、別の範囲に置き換えた場合の値を取得
	/// @param old_min 元の範囲の最小値
	/// @param old_max 元の範囲の最大値
	/// @param new_min 新たな範囲の最小値
	/// @param new_max 新たな範囲の最大値
	/// @param value 変換対象の値
	template<common_concepts::ArithmeticT TargetT, common_concepts::FloatingPointT ReturnT>
	[[nodiscard]] ReturnT ConvertValueNewRange(const TargetT old_min, const TargetT old_max, const TargetT new_min, const TargetT new_max, const TargetT value)
	{
		// 元の値を0～1の範囲に収める
		ReturnT normalized = GetUnitValue<TargetT, ReturnT>(old_min, old_max, value);

		// 新しい範囲にスケール
		return static_cast<ReturnT>(new_min + (new_max - new_min) * normalized);
	}

	#pragma endregion


	#pragma region 比率
	/// @brief 比率計算を行う
	/// @brief base_numerator : base_denominator = return_num : pair_denominator
	template<common_concepts::ArithmeticT ArithmeticT, common_concepts::FloatingPointT ReturnT>
	[[nodiscard]] ReturnT GetRatio(const ArithmeticT base_numerator, const ArithmeticT base_denominator, const ArithmeticT pair_denominator)
	{
		return (base_numerator / base_denominator) * pair_denominator;
	}
	#pragma endregion


	#pragma region 三平方の定理
	/// @brief 直角二等辺三角形の底辺(又は高さ)の長さを取得
	/// @param hypotenuse_length 斜辺の長さ
	[[nodiscard]] float GetAdjacentLengthIsoscelesRightTriangle(const float hypotenuse_length);

	/// @brief 直角二等辺三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺(又は高さ)の長さ
	[[nodiscard]] float GetHypotenuseLengthIsoscelesRightTriangle(const float adjacent_length);

	/// @brief 直角三角形の底辺(又は高さ)の長さを取得
	/// @param opposite_length 高さ(又は底辺の長さ)
	/// @param hypotenuse_length 斜辺の長さ
	[[nodiscard]] float GetAdjacentLengthRightTriangle(const float opposite_length, const float hypotenuse_length);

	/// @brief 直角三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺の長さ
	/// @param opposite_length 高さ
	[[nodiscard]] float GetHypotenuseLengthRightTriangle(const float adjacent_length, const float opposite_length);

	/// @brief 直角三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺の長さ
	/// @param theta θ(なす角)
	[[nodiscard]] float GetHypotenuseLengthRightTriangleFromAdjacent(const float adjacent_length, const float theta);

	/// @brief 直角三角形の斜辺の長さを取得
	/// @param opposite_length 高さ
	/// @param theta θ(なす角)
	[[nodiscard]] float GetHypotenuseLengthRightTriangleFromOpposite(const float opposite_length, const float theta);
	#pragma endregion


	#pragma region ベクトル
	/// @brief ベクトル同士が平行かを判定
	[[nodiscard]] bool IsHorizontal(const VECTOR& v1, const VECTOR& v2);

	/// @brief ベクトル同士が垂直かを判定
	[[nodiscard]] bool IsVertical(const VECTOR& v1, const VECTOR& v2);

	/// @brief 基準とするベクトルから法線ベクトルを求め、その内の一つを取得
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& v);
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& v1, const VECTOR& v2);

	/// @brief 正射影ベクトル
	/// @param projected_v 投影元ベクトル
	/// @param base_v 投影先ベクトル
	/// @return 投影されたベクトル
	[[nodiscard]] VECTOR GetProjectionVector(const VECTOR& projected_v, const VECTOR& base_v);
	#pragma endregion


	#pragma region 角度
	/// @brief 鋭角かを判定
	[[nodiscard]] bool IsAcuteAngle(const VECTOR& v1, const VECTOR& v2);
	[[nodiscard]] bool IsAcuteAngle(const float radian);

	/// @brief 二つのベクトルのなす角を取得
	/// @return 0°～180°の範囲をラジアンで返す
	[[nodiscard]] float  GetAngleBetweenTwoVector (const VECTOR&   v1, const VECTOR&   v2);
	[[nodiscard]] double GetAngleBetweenTwoVectorD(const VECTOR_D& v1, const VECTOR_D& v2);

	/// @brief 二つのベクトルのヨー角(Y軸)回転のなす角を取得
	[[nodiscard]] float GetYawBetweenTwoVector(const VECTOR& v1, const VECTOR& v2);
	#pragma endregion


	#pragma region 回転
	/// @brief 回転後の座標を取得
	[[nodiscard]] VECTOR GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q);

	/// @brief ヨー角(Y軸回転)を取得
	[[nodiscard]] float GetYaw(const VECTOR& v);

	/// @brief ヨー角(Y軸)回転ベクトルを取得
	[[nodiscard]] VECTOR GetYawRotVector(const VECTOR& v);

	/// @brief XYZ軸を取得
	/// @param dir 向きベクトル(この値をZ軸とする)
	/// @param parent_axis 親とするXYZ軸
	[[nodiscard]] Axis GetAxis(const VECTOR& dir, const Axis& parent_axis);
	#pragma endregion


	#pragma region 重心
	/// @brief 三角形の物理的、および幾何学的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfATriangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3);
	[[nodiscard]] VECTOR GetCentroidOfATriangle(const Triangle& triangle);

	/// @brief 四角形の物理的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfAQuadrilateral(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4);
	[[nodiscard]] VECTOR GetCentroidOfAQuadrilateral(const Square& square);
	#pragma endregion


	#pragma region 図形同士の関係
	/// @brief 直線と直線の交点を取得
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合、もしくは二直線が同一直線上にある場合はnullopt(if(戻り値)で判定する必要あり)
	[[nodiscard]] std::optional<VECTOR> GetIntersectionLineAndLine(const Line& line1, const Line& line2);

	/// @brief 線分と平面(無限に広がる面)の交点を取得
	/// @brief 受け取り側で、戻り値がnulloptである場合と不定値である場合を考慮する必要あり
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合はnullopt(if(戻り値){}で判定する必要あり)
	/// @return 線分と平面が同一平面上にある場合は不定値(受け取り側でstd::isfinite()を使用する必要あり)
	[[nodiscard]] std::optional<VECTOR> GetIntersectionSegmentAndPlane(const Segment& segment, const Plane& plane);

	/// @brief 線分と球の交点を取得
	/// @return 交点が2つある場合は始点に近い交点
	/// @return 線分が球に含まれる場合は交点なし
	//[[nodiscard]] std::optional<VECTOR> GetIntersectionSegmentAndSphere(const Segment& segment, const Sphere& sphere);

	/// @brief 線分とカプセルの交点を取得
	/// @brief 受け取り側で、戻り値がnulloptである場合と不定値である場合を考慮する必要あり
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合はnullopt(if(戻り値){}で判定する必要あり)
	/// @return 線分と平面が同一平面上にある場合は不定値(受け取り側でstd::isfinite()を使用する必要あり)
	//[[nodiscard]] std::optional<VECTOR> GetIntersectionSegmentAndCapsule(const Segment& segment, const Capsule& capsule);

	/// @brief 二直線が同一直線上にあるかを判定
	[[nodiscard]] bool IsSameLine(const Line& line1, const Line& line2);

	/// @brief 線分に最も近い平面上の座標を取得
	[[nodiscard]] VECTOR GetClosestPosOnPlaneFromSegment(const Plane& plane, const Segment& segment);

	/// @brief 平面に最も近い線分上の座標を取得
	[[nodiscard]] VECTOR GetClosestPosOnSegmentFromPlane(const Segment& segment, const Plane& plane);

	/// @brief 線分に最も近い線分上の座標を取得
	//[[nodiscard]] VECTOR GetClosestPosOnSegmentFromSegment(const Segment& segment1, const Segment& target_segmen);

	/// @brief 点が平面の前方にあるかを判定
	/// @return true : 前方, false : 後方もしくは平面に含まれる
	[[nodiscard]] bool IsPointAheadOfPlane(const VECTOR& point, const Plane& plane);

	/// @brief 点が球の表面にあるかを判定
	/// TODO : 正しく機能するか試していないため検証が必要
	[[nodiscard]] bool IsPointOnSphereSurface(const VECTOR& point, const Sphere& sphere);

	/// @brief 点がカプセル内にあるかを判定
	[[nodiscard]] bool IsPointInsideCapsule(const VECTOR& point, const Capsule& capsule);

	/// @brief 点を三角形が存在する平面に投影した際、三角形の内部に存在するかを判定
	[[nodiscard]] bool IsProjectedPointInTriangle(const VECTOR& point, const Triangle& triangle);
	#pragma endregion


	#pragma region 図形
	/// @brief 円内のランダムな点を取得する
	/// @param center_pos 円の中心座標
	/// @param radius 円の半径
	/// @return 円内のランダム座標
	[[nodiscard]] VECTOR GetRandomPointInCircle(const Circle& circle);
	#pragma endregion


	#pragma region 最短距離
	/// @brief 点と直線の最短距離を取得
	[[nodiscard]] float GetDistancePointToLine			(const VECTOR&		point,		const Line&		line);
	[[nodiscard]] float GetDistancePointToLine			(const VECTOR&		point,		const Line&		line,		VECTOR& h, float& t);

	/// @brief 点と線分の最短距離を取得
	[[nodiscard]] float GetDistancePointToSegment		(const VECTOR&		point,		const Segment&	segment);
	[[nodiscard]] float GetDistancePointToSegment		(const VECTOR&		point,		const Segment&	segment,	VECTOR& h, float& t);

	/// @brief 点と平面(無限に広がる面)の最短距離を取得
	[[nodiscard]] float GetDistancePointToPlane			(const VECTOR&		point,		const Plane&	plane);

	/// @brief 点と三角形の最短距離を取得
	[[nodiscard]] float GetDistancePointToTriangle		(const VECTOR&		point,		const Triangle& triangle);

	/// @brief 点と四角形の最短距離を取得
	[[nodiscard]] float GetDistancePointToSquare		(const VECTOR&		point,		const Square&	square);

	/// @brief 直線と直線の最短距離を取得
	[[nodiscard]] float GetDistanceLineToLine			(const Line&		line1,		const Line&		line2);
	[[nodiscard]] float GetDistanceLineToLine			(const Line&		line1,		const Line&		line2,		VECTOR& h1, VECTOR& h2, float& t1, float& t2);

	/// @brief 線分と線分の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToSegment		(const Segment&		segment1,	const Segment&	segment2);
	[[nodiscard]] float GetDistanceSegmentToSegment		(const Segment&		segment1,	const Segment&	segment2,	VECTOR& h1, VECTOR& h2, float& t1, float& t2);

	/// @brief 線分と平面(無限に広がる面)の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToPlane		(const Segment&		segment,	const Plane&	plane);

	/// @brief 線分と三角形の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToTriangle	(const Segment&		segment,	const Triangle& triangle);

	/// @brief 線分と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToSquare		(const Segment&		segment,	const Square&	square);

	/// @brief 平面とカプセルの最短距離を取得
	[[nodiscard]] float GetDistancePlaneToCapsule		(const Plane&		plane,		const Capsule&	capsule);

	/// @brief 三角形と三角形の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToTriangle	(const Triangle&	triangle1,	const Triangle& triangle2);

	/// @brief 三角形と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToSquare		(const Triangle&	triangle,	const Square&	square);

	/// @brief 三角形と球の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToSphere     (const Triangle&    triangle,   const Sphere&   sphere);

	/// @brief 三角形とカプセルの最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToCapsule	(const Triangle&	triangle,	const Capsule&	capsule);

	/// @brief 四角形と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceSquareToSquare		(const Square&		square1,	const Square&	square2);

	/// @brief 四角形とカプセルの最短距離を取得
	[[nodiscard]] float GetDistanceSquareToCapsule		(const Square&		square,		const Capsule&	capsule);

	/// @brief 球と球の最短距離を取得
	[[nodiscard]] float GetDistanceSphereToSphere		(const Sphere&		sphere1,	const Sphere&	sphere2);
	#pragma endregion
}
