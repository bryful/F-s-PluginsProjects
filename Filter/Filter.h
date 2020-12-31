//-----------------------------------------------------------------------------------
/*
	Filter for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Filter_H
#define Filter_H

#include "../FsLibrary_next/FsAEHeader.h"
#include "../FsLibrary_next/FsVersion.h"
#include "../FsLibrary_next/FsUtils.h"
#include "../FsLibrary_next/PixelBase.h"
#include "../FsLibrary_next/FsG.h"
#include "../FsLibrary_next/FsBlend.h"

#include "Filter_Target.h"

//CAEクラスの読み込み
#include "../FsLibrary_next/CAE.h"

#ifdef AE_OS_WIN
#define SPRINTF(STR,IDX) sprintf_s(num, "%s%d", STR, IDX)
#else
#define SPRINTF(STR,IDX) sprintf_(num, "%s%d", STR, IDX)
#endif

enum
{
	ID_TOPIC =0,
	ID_ENABLED,
	ID_EXTRACT, // none Light Dark
	ID_BORDER_HI,
	ID_SOFTNESS_HI,
	ID_BORDER_LO,
	ID_SOFTNESS_LO,
	ID_BRIGHT,

	ID_MINMAX,
	ID_MAX,
	ID_BLUR,
	ID_OPACITY,
	ID_BLEND,
	ID_TOPIC_END,
	ID_COUNT
};
enum
{
	ID_INPUT=0,
	ID_BASE_ENABLED,
	ID_BASE_OPACITY,
	ID_FILTER_OPACITY,
	ID_BASE_END

};
#define PARAMSET_COUNT 4
#define ID_NUM(n,idx) (ID_BASE_END + idx + ID_COUNT * n)
#define ID_NOISE (ID_BASE_END + ID_COUNT *PARAMSET_COUNT)
#define ID_ALPHA_ON (ID_NOISE+1)

#define ID_NUM_PARAMS (ID_NOISE+2)

//ID_NUM_PARAMS

//UIの表示文字列
#define	STR_ON				"on"
#define	STR_BASE_ENABLED	"BaseEnabled"
#define	STR_BASE_OPACITY	"BaseOpacity"

#define	STR_TOPIC			"Filter_"
#define	STR_ENABLED			"Enabled_"
#define	STR_EXTRACT			"Extract_"
#define	STR_EXTRACT_ITEMS	"None|Hi|Low"
#define	STR_EXTRACT_COUNT	3
#define	STR_EXTRACT_DFLT	1
#define	STR_BORDER_HI		"border_hi_"
#define	STR_SOFTNESS_HI		"softness_hi_"
#define	STR_BORDER_LO		"border_lo_"
#define	STR_SOFTNESS_LO		"softness_lo_"
#define	STR_BRIGHT			"Brightness_"

#define	STR_MINMAX			"MinToMax_"
#define	STR_MAX				"Max_"
#define	STR_BLUR			"Blur_"
#define	STR_OPACITY			"Opacity_"
#define	STR_BLEND			"Blend_"
#define	STR_BLEND_ITEMS		"Normal|Add|Screen|Lighten|LighterColor|Multiply|Darken|DarkerColor|Overlay"
#define	STR_BLEND_COUNT		9
#define	STR_BLEND_DFLT		1
#define	STR_FILTER_OPACITY	"FilterOpacity"

#define	STR_NOISE			"Noise"
#define	STR_ALPHA_ON		"Alpha_ON"

enum EXTRACT_MODE
{
	NONE=1,
	HI,
	LO
};

//UIのパラメータ
typedef struct ParamSetInfo {
	PF_Boolean	enabled;
	A_long		extract_mode;
	PF_FpLong	border_hi;
	PF_FpLong	softness_hi;
	PF_FpLong	border_lo;
	PF_FpLong	softness_lo;
	PF_FpLong	brightness;
	A_long		minmax;
	A_long		max;
	A_long		blur;
	PF_FpLong	opacity;
	A_long		blend_mode;

}ParamSetInfo, *ParamSetInfoP, **ParamSetInfoH;

typedef struct ParamInfo {
	PF_Boolean		base_enabled;
	PF_FpLong		base_opacity;
	ParamSetInfo	paramset[PARAMSET_COUNT];
	PF_FpLong		noise;
	PF_Boolean		alpha_on;
	PF_FpLong		filter_opacity;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
PF_Err Exec(CAE *ae, ParamInfo *infoP);
PF_Err Exec08(CAE *ae, ParamInfo *infoP);
PF_Err Exec16(CAE *ae, ParamInfo *infoP);
PF_Err ToHarfSize16(PF_EffectWorldPtr world);
PF_Err ToDoubleSize16(PF_EffectWorldPtr world, PF_Handle bufH);

PF_Err ExtractHi16(PF_EffectWorldPtr world, PF_FpLong wp, PF_FpLong sf);
PF_Err ExtractLo16(PF_EffectWorldPtr world, PF_FpLong bp, PF_FpLong sf);

PF_Err Max16(PF_EffectWorldPtr world, A_long max, PF_Handle bufH);
PF_Err Rev16(PF_EffectWorldPtr world);
PF_Err Blur16(CAE *ae, PF_EffectWorldPtr world, A_long blur, PF_Handle bufH);
PF_Err Brightness16(PF_EffectWorldPtr world, PF_FpLong brigtness);

//-----------------------------------------------------------------------------------
extern "C" {

	DllExport
		PF_Err
		EffectMain(
			PF_Cmd			cmd,
			PF_InData		*in_data,
			PF_OutData		*out_data,
			PF_ParamDef		*params[],
			PF_LayerDef		*output,
			void			*extra);

}

#endif // Filter_H
