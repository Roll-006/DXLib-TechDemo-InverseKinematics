#pragma once
#include "../Base/singleton_base.hpp"

#include "../Object/main_camera.hpp"

#include "../VirtualCamera/control_virtual_cameras_controller.hpp"

#include "../Input/input_checker.hpp"
#include "../GameTime/game_time_manager.hpp"

class Player;

namespace camera_controllder_concepts
{
	/// @brief バーチャルカメラコントローラー型である
	template<typename T>
	concept VirtualCameraController = std::is_base_of_v<IVirtualCameraController, T>;
}

class CinemachineBrain final : public SingletonBase<CinemachineBrain>
{
public:
	void Init();
	void Update();
	void LateUpdate();

	/// @brief 優先順位をソートする
	void SortPriority(const std::shared_ptr<VirtualCamera>& virtual_camera);


	#pragma region 登録 / 解除
	/// @brief バーチャルカメラを登録する
	/// @param virtual_camera 登録するバーチャルカメラ
	/// @param is_active アクティブ化するかどうか
	void AddVirtualCamera(const std::shared_ptr<VirtualCamera>& virtual_camera, const bool is_active);
	/// @brief バーチャルカメラの登録を解除する
	void RemoveVirtualCamera(const int camera_handle);
	void RemoveAllVirtualCamera();

	/// @brief バーチャルカメラコントローラーを登録する
	template<camera_controllder_concepts::VirtualCameraController ControllerT>
	void AddVirtualCameraController(const std::shared_ptr<ControllerT>& virtual_camera_controller)
	{
		if (std::find(m_virtual_camera_controllers.begin(), m_virtual_camera_controllers.end(), virtual_camera_controller) == m_virtual_camera_controllers.end())
		{
			m_virtual_camera_controllers.emplace_back(virtual_camera_controller);
		}
	}

	/// @brief バーチャルカメラコントローラーの登録を解除する
	template<camera_controllder_concepts::VirtualCameraController ControllerT>
	void RemoveVirtualCameraController(const std::shared_ptr<ControllerT>& virtual_camera_controller)
	{
		if (std::find(m_virtual_camera_controllers.begin(), m_virtual_camera_controllers.end(), virtual_camera_controller) != m_virtual_camera_controllers.end())
		{
			erase(m_virtual_camera_controllers, virtual_camera_controller);
		}
	}
	void RemoveVirtualCameraController(const VirtualCameraControllerKind kind);
	void RemoveAllVirtualCameraController();
	#pragma endregion


	#pragma region Setter
	/// @brief メインカメラを設定する
	void SetMainCamera(const std::shared_ptr<MainCamera>& main_camera);

	/// @brief ブレンドにかける時間を設定する
	void SetBlendTime(const float blend_time);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<MainCamera> GetMainCamera() const { return m_main_camera; }

	/// @brief バーチャルカメラを取得
	/// @param camera_handle オブジェクトハンドル
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetVirtualCamera(const int camera_handle) const;
	/// @brief バーチャルカメラを取得
	/// @brief オブジェクト名でカメラを判別する(同じ名前が複数ある場合はオブジェクトハンドルでの取得を推奨)
	/// @param obj_name オブジェクト名
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetVirtualCamera(const std::string& obj_name) const;

	/// @brief バーチャルカメラコントローラーを取得
	/// @param obj_handle バーチャルカメラコントローラーハンドル
	[[nodiscard]] std::shared_ptr<IVirtualCameraController> GetVirtualCameraController(const int controller_handle) const;
	/// @brief バーチャルカメラコントローラーを取得
	/// @brief バーチャルカメラの種類でカメラを判別する(同じ種類が複数ある場合はハンドルでの取得を推奨)
	/// @param camra_kind バーチャルカメラの種類
	[[nodiscard]] std::shared_ptr<IVirtualCameraController> GetVirtualCameraController(const VirtualCameraControllerKind controller_kind) const;

	/// @brief バーチャルカメラをブレンド中であるかを判定
	[[nodiscard]] bool  IsBlending()	const { return m_is_blending; }
	[[nodiscard]] float GetBlendTime()	const { return m_blend_time; }

	[[nodiscard]] float GetNear()		const { return m_near;	}
	[[nodiscard]] float GetFar()		const { return m_far;	}
	[[nodiscard]] float GetFOV()		const { return m_fov;	}
	#pragma endregion

private:
	CinemachineBrain();
	~CinemachineBrain() override;
	

	#pragma region ブレンド関連処理
	/// @brief ターゲットバーチャルカメラによって自身以外のバーチャルカメラを非アクティブ化させる
	/// @brief 非アクティブ化条件はターゲットバーチャルカメラが保持
	void DeactivateVirtualCamera(const std::shared_ptr<VirtualCamera>& origin_camera, const std::shared_ptr<VirtualCamera>& target_camera);

	/// @brief バーチャルカメラ間のブレンドを行う
	void BlendVirtualCamera();

	/// @brief ターゲットとなるバーチャルカメラを変更する
	void ChangeTargetVirtualCamera(const int camera_handle);

	/// @brief ブレンド対象となるトランスフォームを設定
	void SetBlendTransform();

	/// @brief ブレンド結果行列トランスフォームを計算
	void CalcBlendResultTransform();

	void CalcBlendResultAimPos();
	#pragma endregion


private:
	float m_near;
	float m_far;
	float m_fov;

	std::shared_ptr<MainCamera>									m_main_camera;				// バーチャルカメラを適用させるメインカメラ
	std::shared_ptr<VirtualCamera>								m_origin_virtual_camera;	// 起点とするバーチャルカメラ
	std::shared_ptr<VirtualCamera>								m_target_virtual_camera;	// ターゲットとするバーチャルカメラ
	std::unordered_map<int, std::shared_ptr<VirtualCamera>>		m_virtual_cameras;			// 登録されているバーチャルカメラ
	std::vector<std::pair<int, int>>							m_priority;					// 優先順位<オブジェクトハンドル, 優先度>

	std::shared_ptr<Transform>			m_blend_origin_transform;							// ブレンドの起点とするトランスフォーム
	std::shared_ptr<Transform>			m_blend_target_transform;							// ブレンドのターゲットとするトランスフォーム
	std::shared_ptr<Transform>			m_blend_origin_result_transform;					// ブレンドの起点に利用するブレンド結果トランスフォーム
	std::shared_ptr<Transform>			m_blend_result_transform;							// ブレンド結果トランスフォーム

	VECTOR								m_blend_result_aim_pos;

	std::unordered_map<TimeKind, int>	m_origin_virtual_camera_handle;						// ブレンドの起点とするバーチャルカメラのハンドル
	std::unordered_map<TimeKind, int>	m_target_virtual_camera_handle;						// ブレンドのターゲットとするバーチャルカメラのハンドル

	float m_blend_time;
	float m_blend_timer;																	// ブレンドにかける時間を計測
	float m_blend_coefficient;																// ブレンド係数
	bool  m_is_blending;																	// ブレンド中かを判定
	bool  m_is_invert_horizontal;															// 操作時に左右反転を行うかを判定
	bool  m_is_invert_vertical;																// 操作時に上下反転を行うかを判定

	std::vector<std::shared_ptr<IVirtualCameraController>> m_virtual_camera_controllers;

	friend class SingletonBase<CinemachineBrain>;
};
