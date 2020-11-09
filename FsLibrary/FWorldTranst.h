#pragma once
#ifndef FWorldTranst_H
#define FWorldTranst_H

#include "Fs.h"
#include "FsAE.h"
#include "FWorld.h"


class FWorldTranst : public FWorld
{
public:

	// ***************************************************************
#pragma region  Constractor
	FWorldTranst(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	): FWorld(wld,ind,fmt)
	{
	}
	// ***************************************************************
public:
	virtual ~FWorldTranst()
	{
	}
#pragma endregion
	// ***************************************************************

};
#endif