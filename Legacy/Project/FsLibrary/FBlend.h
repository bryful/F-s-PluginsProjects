#pragma once
#ifndef FBlend_H
#define FBlend_H

#include "Fs.h"
#include "FsUtils.h"

#include "FWorld.h"


class FBlend
{
private:
	PF_Boolean m_Enabled = FALSE;

public:
	PF_InData* in_data = NULL;
	FWorld src = NULL;
	FWorld dst = NULL;

	PF_Boolean Enabled() { return m_Enabled; };

	// ***************************************************************
	FBlend(
		PF_InData* ind,
		FWorld s,
		FWorld d

		)
	{
		in_data = ind;
		src = s;
		dst = d;
		m_Enabled = IsChk();
	}
	// ***************************************************************
	virtual ~FBlend()
	{
	}
private:
	// ***************************************************************
	PF_Boolean IsChk()
	{
		PF_Boolean ret = FALSE;
		if ((src.world == NULL) || (dst.world == NULL)) return ret;
		if (src.pixelFormat() != dst.pixelFormat()) return ret;
		if (src.width() != src.width()) return ret;
		if (src.height() != src.height()) return ret;
		ret = TRUE;
		return ret;
	}
	// ***************************************************************
	void blend8()
	{
		if (m_Enabled == FALSE) return;
		for (A_long y = 0; y < dst.height(); y++)
		{
			for (A_long x = 0; x < dst.width(); x++)
			{
			}
		}
	}

};
#endif
