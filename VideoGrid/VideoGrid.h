//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef VideoGrid_H
#define VideoGrid_H

#include "Fs_Target.h"

#include "AEConfig.h"
#include "entry.h"

//#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"

#if defined(PF_AE100_PLUG_IN_VERSION)
	#include "AEFX_SuiteHelper.h"
	#define refconType void*
#else
	#include "PF_Suite_Helper.h"
	#define refconType A_long
#endif
#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#include "../FsLibrary/FsAE.h"
//#include "FsAE.h"

enum {
	ID_INPUT = 0,		// default input layer 
	ID_GRIDSIZE,
	ID_MODE,
	ID_HILIGHT,
	ID_SHADOW,
	ID_HEIGHT,
	ID_NUM_PARAMS
};
#define STR_GRIDSIZE	"グリッドサイズ"
#define STR_MODE1		"モード"
#define STR_MODE2		"モザイク"
#define STR_HILIGHT		"ハイライト(%)"
#define STR_SHADOW		"シャドウ(%)"
#define STR_HEIGHT		"ライン幅"

typedef struct ParamInfo{
	A_long			gridSize;
	PF_Boolean		isMosaic;
	PF_FpShort		hilight;
	PF_FpShort		shadow;
	A_long			height;
} ParamInfo,*ParamInfoP,**ParamInfoH;

typedef struct gridParam{
	A_long		loopX;
	A_long		loopY;
	PF_InData	*in_data;
	ParamInfo	info;
	PF_PixelPtr	data;
	A_long		width;
	A_long		widthTrue;
	A_long		height;
	A_long		x;
	A_long		y;
	PF_EffectWorld	*out;
} gridParam,*gridParamP,**gridParamH;

//-------------------------------------------------------------------------------------------------
extern "C" {
DllExport 
PF_Err 
EntryPointFunc (	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}
//-----------------------------------------------------------------------------------
inline PF_Pixel lineBright8(PF_Pixel c, PF_FpShort v)
{
	PF_Pixel r;
	double vv;
	r.alpha		= c.alpha;
	if (v==1){
		r = c ;
	}else if ( v<=0){
		r.blue = r.green = r.red = 0;
	}else if ( v>=2){
		r.blue = r.green = r.red = PF_MAX_CHAN8;

	} else if (v<1){
		r.blue	= RoundByteFpLong(v * c.blue);
		r.green = RoundByteFpLong(v * c.green);
		r.red	= RoundByteFpLong(v * c.red);
	}else{
		vv = v - 1;
		r.blue	= RoundByteFpLong(c.blue  + vv * (PF_MAX_CHAN8 - c.blue));
		r.green = RoundByteFpLong(c.green + vv * (PF_MAX_CHAN8 - c.green));
		r.red	= RoundByteFpLong(c.red   + vv * (PF_MAX_CHAN8 - c.red));
	}
	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel16 lineBright16(PF_Pixel16 c, PF_FpShort v)
{
	PF_Pixel16 r;
	double vv;
	r.alpha		= c.alpha;
	if (v==1){
		r = c ;
	}else if ( v<=0){
		r.blue = r.green = r.red = 0;
	}else if ( v>=2){
		r.blue = r.green = r.red = PF_MAX_CHAN16;

	} else if (v<1){
		r.blue	= RoundShortFpLong(v * c.blue);
		r.green = RoundShortFpLong(v * c.green);
		r.red	= RoundShortFpLong(v * c.red);
	}else{
		vv = v - 1;
		r.blue	= RoundShortFpLong(c.blue  + vv * (PF_MAX_CHAN16 - c.blue));
		r.green = RoundShortFpLong(c.green + vv * (PF_MAX_CHAN16 - c.green));
		r.red	= RoundShortFpLong(c.red   + vv * (PF_MAX_CHAN16 - c.red));
	}
	return r;
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat lineBright32(PF_PixelFloat c, PF_FpShort v)
{
	PF_PixelFloat r;
	PF_FpShort vv;
	r.alpha		= c.alpha;
	if (v==1){
		r = c ;
	}else if ( v<=0){
		r.blue = r.green = r.red = 0;
	}else if ( v>=2){
		r.blue = r.green = r.red = 1;

	} else if (v<1){
		r.blue	= RoundFpShort(v * c.blue);
		r.green = RoundFpShort(v * c.green);
		r.red	= RoundFpShort(v * c.red);
	}else{
		vv = v - 1;
		r.blue	= RoundFpShort(c.blue  + vv * (1 - c.blue));
		r.green = RoundFpShort(c.green + vv * (1 - c.green));
		r.red	= RoundFpShort(c.red   + vv * (1 - c.red));
	}
	return r;
}
//-------------------------------------------------------------------------------------------------

void BlockBevel8(gridParam *prm);
void BlockBevel16(gridParam *prm);
void BlockBevel32(gridParam *prm);
void BlockMosaic8(gridParam *prm);
void BlockMosaic16(gridParam *prm);
void BlockMosaic32(gridParam *prm);

#endif // VideoGrid_H