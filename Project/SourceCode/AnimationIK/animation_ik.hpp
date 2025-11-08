#pragma once
#include "../Calculation/math.hpp"
#include "../Path/bone_path.hpp"

class AnimationIK final
{
public:
	AnimationIK();
	~AnimationIK();

	void Test(const int model_handle, const VECTOR& world_destination);
	void LegIK(const int model_handle);

private:
	void CalcOneBoneIK(const VECTOR& world_destination, MATRIX& frame_local_matrix);
};
