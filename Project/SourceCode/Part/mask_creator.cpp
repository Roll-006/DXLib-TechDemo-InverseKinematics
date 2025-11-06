#include "mask_creator.hpp"

bool MaskCreator::m_is_invert_mask = false;

void MaskCreator::CreateMask()
{
	CreateMaskScreen();
}

void MaskCreator::DeleteMask()
{
	DeleteMaskScreen();
}

void MaskCreator::UseMask(const int graphic_handle, const bool is_invert_mask)
{
	if (m_is_invert_mask != is_invert_mask)
	{
		m_is_invert_mask = is_invert_mask;
		SetMaskReverseEffectFlag(m_is_invert_mask);
	}

	SetMaskScreenGraph(graphic_handle);
	SetUseMaskScreenFlag(TRUE);
}

void MaskCreator::UnuseMask()
{
	SetUseMaskScreenFlag(FALSE);	
}
