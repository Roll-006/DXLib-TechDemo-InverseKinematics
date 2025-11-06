#pragma once

/// @brief Blenderを通したMixamoモデルのボーンパス
static struct MixamoBonePath
{
	const TCHAR* ARMATURE				= "Armature";


	const TCHAR* HIPS					= "mixamorig:Hips";
	const TCHAR* SPINE					= "mixamorig:Spine";
	const TCHAR* SPINE_1				= "mixamorig:Spine1";
	const TCHAR* SPINE_2				= "mixamorig:Spine2";
	const TCHAR* NECK					= "mixamorig:Neck";
	const TCHAR* HEAD					= "mixamorig:Head";
	const TCHAR* HEAD_TOP_END			= "mixamorig:HeadTop_End";
	const TCHAR* HEAD_TOP_END_END		= "mixamorig:HeadTop_End_end";


	const TCHAR* LEFT_SHOULDER			= "mixamorig:LeftShoulder";
	const TCHAR* LEFT_ARM				= "mixamorig:LeftArm";
	const TCHAR* LEFT_FORE_ARM			= "mixamorig:LeftForeArm";
	const TCHAR* LEFT_HAND				= "mixamorig:LeftHand";
	const TCHAR* LEFT_HAND_THUMB_1		= "mixamorig:LeftHandThumb1";
	const TCHAR* LEFT_HAND_THUMB_2		= "mixamorig:LeftHandThumb2";
	const TCHAR* LEFT_HAND_THUMB_3		= "mixamorig:LeftHandThumb3";
	const TCHAR* LEFT_HAND_THUMB_4		= "mixamorig:LeftHandThumb4";
	const TCHAR* LEFT_HAND_THUMB_4_END	= "mixamorig:LeftHandThumb4_end";

	const TCHAR* LEFT_HAND_INDEX_1		= "mixamorig:LeftHandIndex1";
	const TCHAR* LEFT_HAND_INDEX_2		= "mixamorig:LeftHandIndex2";
	const TCHAR* LEFT_HAND_INDEX_3		= "mixamorig:LeftHandIndex3";
	const TCHAR* LEFT_HAND_INDEX_4		= "mixamorig:LeftHandIndex4";
	const TCHAR* LEFT_HAND_INDEX_4_END	= "mixamorig:LeftHandIndex4_end";

	const TCHAR* LEFT_HAND_MIDDLE_1		= "mixamorig:LeftHandMiddle1";
	const TCHAR* LEFT_HAND_MIDDLE_2		= "mixamorig:LeftHandMiddle2";
	const TCHAR* LEFT_HAND_MIDDLE_3		= "mixamorig:LeftHandMiddle3";
	const TCHAR* LEFT_HAND_MIDDLE_4		= "mixamorig:LeftHandMiddle4";
	const TCHAR* LEFT_HAND_MIDDLE_4_END	= "mixamorig:LeftHandMiddle4_end";

	const TCHAR* LEFT_HAND_RING_1		= "mixamorig:LeftHandRing1";
	const TCHAR* LEFT_HAND_RING_2		= "mixamorig:LeftHandRing2";
	const TCHAR* LEFT_HAND_RING_3		= "mixamorig:LeftHandRing3";
	const TCHAR* LEFT_HAND_RING_4		= "mixamorig:LeftHandRing4";
	const TCHAR* LEFT_HAND_RING_4_END	= "mixamorig:LeftHandRing4_end";
	
	const TCHAR* LEFT_HAND_PINKY_1		= "mixamorig:LeftHandPinky1";
	const TCHAR* LEFT_HAND_PINKY_2		= "mixamorig:LeftHandPinky2";
	const TCHAR* LEFT_HAND_PINKY_3		= "mixamorig:LeftHandPinky3";
	const TCHAR* LEFT_HAND_PINKY_4		= "mixamorig:LeftHandPinky4";
	const TCHAR* LEFT_HAND_PINKY_4_END	= "mixamorig:LeftHandPinky4_end";


	const TCHAR* RIGHT_SHOULDER			= "mixamorig:RightShoulder";
	const TCHAR* RIGHT_ARM				= "mixamorig:RightArm";
	const TCHAR* RIGHT_FORE_ARM			= "mixamorig:RightForeArm";
	const TCHAR* RIGHT_HAND				= "mixamorig:RightHand";
	const TCHAR* RIGHT_HAND_THUMB_1		= "mixamorig:RightHandThumb1";
	const TCHAR* RIGHT_HAND_THUMB_2		= "mixamorig:RightHandThumb2";
	const TCHAR* RIGHT_HAND_THUMB_3		= "mixamorig:RightHandThumb3";
	const TCHAR* RIGHT_HAND_THUMB_4		= "mixamorig:RightHandThumb4";
	const TCHAR* RIGHT_HAND_THUMB_4_END	= "mixamorig:RightHandThumb4_end";

	const TCHAR* RIGHT_HAND_INDEX_1		= "mixamorig:RightHandIndex1";
	const TCHAR* RIGHT_HAND_INDEX_2		= "mixamorig:RightHandIndex2";
	const TCHAR* RIGHT_HAND_INDEX_3		= "mixamorig:RightHandIndex3";
	const TCHAR* RIGHT_HAND_INDEX_4		= "mixamorig:RightHandIndex4";
	const TCHAR* RIGHT_HAND_INDEX_4_END	= "mixamorig:RightHandIndex4_end";

	const TCHAR* RIGHT_HAND_MIDDLE_1	= "mixamorig:RightHandMiddle1";
	const TCHAR* RIGHT_HAND_MIDDLE_2	= "mixamorig:RightHandMiddle2";
	const TCHAR* RIGHT_HAND_MIDDLE_3	= "mixamorig:RightHandMiddle3";
	const TCHAR* RIGHT_HAND_MIDDLE_4	= "mixamorig:RightHandMiddle4";
	const TCHAR* RIGHT_HAND_MIDDLE_4_END= "mixamorig:RightHandMiddle4_end";

	const TCHAR* RIGHT_HAND_RING_1		= "mixamorig:RightHandRing1";
	const TCHAR* RIGHT_HAND_RING_2		= "mixamorig:RightHandRing2";
	const TCHAR* RIGHT_HAND_RING_3		= "mixamorig:RightHandRing3";
	const TCHAR* RIGHT_HAND_RING_4		= "mixamorig:RightHandRing4";
	const TCHAR* RIGHT_HAND_RING_4_END	= "mixamorig:RightHandRing4_end";

	const TCHAR* RIGHT_HAND_PINKY_1		= "mixamorig:RightHandPinky1";
	const TCHAR* RIGHT_HAND_PINKY_2		= "mixamorig:RightHandPinky2";
	const TCHAR* RIGHT_HAND_PINKY_3		= "mixamorig:RightHandPinky3";
	const TCHAR* RIGHT_HAND_PINKY_4		= "mixamorig:RightHandPinky4";
	const TCHAR* RIGHT_HAND_PINKY_4_END	= "mixamorig:RightHandPinky4_end";


	const TCHAR* LEFT_UP_LEG			= "mixamorig:LeftUpLeg";
	const TCHAR* LEFT_LEG				= "mixamorig:LeftLeg";
	const TCHAR* LEFT_FOOT				= "mixamorig:LeftFoot";
	const TCHAR* LEFT_TOE_BASE			= "mixamorig:LeftToeBase";
	const TCHAR* LEFT_TOE_END			= "mixamorig:LeftToe_End";
	const TCHAR* LEFT_TOE_END_END		= "mixamorig:LeftToe_End_end";

	const TCHAR* RIGHT_UP_LEG			= "mixamorig:RightUpLeg";
	const TCHAR* RIGHT_LEG				= "mixamorig:RightLeg";
	const TCHAR* RIGHT_FOOT				= "mixamorig:RightFoot";
	const TCHAR* RIGHT_TOE_BASE			= "mixamorig:RightToeBase";
	const TCHAR* RIGHT_TOE_END			= "mixamorig:RightToe_End";
	const TCHAR* RIGHT_TOE_END_END		= "mixamorig:RightToe_End_end";

}BonePath;
