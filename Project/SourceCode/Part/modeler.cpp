#include "modeler.hpp"

#pragma region コンストラクタ / デストラクタ
Modeler::Modeler(const std::shared_ptr<Transform>& transform, const std::string& file_path, const float basic_scale) :
	m_model_handle	(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kModel, file_path)),
	m_opacity		(1.0f),
	m_transform		(transform),
	m_basic_scale	(VGet(basic_scale, basic_scale, basic_scale))
{
	MV1SetupCollInfo(m_model_handle);
}

Modeler::Modeler(const int model_handle) :
	m_model_handle	(HandleKeeper::GetInstance()->ReloadHnadle(HandleKind::kModel, model_handle)),
	m_opacity		(1.0f),
	m_transform		(nullptr),
	m_basic_scale	(VGet(1.0f, 1.0f, 1.0f))
{
	MV1SetupCollInfo(m_model_handle);
}

Modeler::~Modeler()
{
	if (m_model_handle != -1)
	{
		MV1DeleteModel(m_model_handle);
		m_model_handle = -1;
	}
}
#pragma endregion

void Modeler::Draw() const
{
	ApplyOpacity();
	ApplyMatrix();

	MV1DrawModel(m_model_handle);
}

void Modeler::InitMaterial()
{
	MV1SetDifColorScale(m_model_handle, COLOR_F(1.0f, 1.0f, 1.0f, 1.0f));
	MV1SetSpcColorScale(m_model_handle, COLOR_F(1.0f, 1.0f, 1.0f, 1.0f));
	MV1SetEmiColorScale(m_model_handle, COLOR_F(1.0f, 1.0f, 1.0f, 1.0f));
	MV1SetAmbColorScale(m_model_handle, COLOR_F(1.0f, 1.0f, 1.0f, 1.0f));
}

void Modeler::ApplyOpacity() const
{
	// 適用済みの値と同じ場合は重複した適用を避ける
	if (MV1GetOpacityRate(m_model_handle) == m_opacity) { return; }

	if (m_opacity == 1.0f)
	{
		MV1SetUseZBuffer (m_model_handle, TRUE);
	}
	else
	{
		MV1SetUseZBuffer (m_model_handle, FALSE);
	}
	MV1SetOpacityRate(m_model_handle, m_opacity);
}

void Modeler::ApplyMatrix() const
{
	const auto scale_m		= MGetScale(m_basic_scale);
	const auto m			= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto result_m		= scale_m * m;
	
	// 適用済みの値と同じ場合は重複した適用を避ける
	if (MV1GetMatrix(m_model_handle) == result_m) { return; }

	// 行列情報の更新と同時に衝突情報も更新
	MV1SetMatrix(m_model_handle, result_m);
	MV1RefreshCollInfo(m_model_handle);
}
