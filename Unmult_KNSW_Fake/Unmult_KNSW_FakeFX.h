#pragma once
#ifndef Unmult_KNSW_FakeFX_H
#define Unmult_KNSW_FakeFX_H

#include "../NFsLibrary/AE_SDK.h"
#include "../NFsLibrary/AEInfo.h"
#include "../NFsLibrary/NFsWorld.h"

#include "Unmult_KNSW_Fake.h"



class Unmult_KNSW_FakeFX :public NFsWorld
{
private:

public:
	Unmult_KNSW_FakeFX(
		PF_EffectWorld* wld = NULL,
		PF_InData* ind = NULL,
		PF_PixelFormat	fmt = PF_PixelFormat_ARGB32
	) : NFsWorld(wld, ind, fmt)
	{

	}
#pragma region Noise

	// **************************************************************
	PF_Err Noise(ParamInfo* infoP)
	{
		PF_Err			err = PF_Err_NONE;
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			ERR(Iterate32((void*)infoP, Noise32));
			break;
		case PF_PixelFormat_ARGB64:
			ERR(Iterate16((void*)infoP, Noise16));
			break;
		case PF_PixelFormat_ARGB32:
			ERR(Iterate8((void*)infoP, Noise8));
			break;
		}
		return err;
	}
#pragma endregion
};

#endif