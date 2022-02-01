//-----------------------------------------------------------------------------------
/*
	Flare for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef Flare_H
#define Flare_H

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



#ifdef AE_OS_WIN
#define SPRINTF(STR,IDX) sprintf_s(num, "%s%d", STR, IDX)
#else
#define SPRINTF(STR,IDX) sprintf_(num, "%s%d", STR, IDX)
#endif

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_MODE,		//モード  WhiteBaseMask or BlackBaseMask or Image(lumnan) 
	ID_BASE_ON,
	ID_BASE_COLOR,
	ID_BASE_OPACITY,
	ID_OFFSET
};
enum
{
	ID_TOPIC = 0,
	ID_ENABLED,		// これが有効
	ID_REV,			// rev none rev or revandMask
	ID_BORDER_TOPIC,
	ID_BORDER,		//境界線 Image
	ID_INSIDE,		// border時の内側
	ID_OUTSIDE,		// border時の外側
	ID_BORDER_TOPIC_END,
	ID_MAX,			// 膨張サイズ
	ID_BLUR,		// ぼかしサイズ
	ID_COLOR,		// color and borderの時に使う
	ID_BLEND,		// normal or screen or Add
	ID_OPACITY,
	ID_TOPIC_END,
	ID_FLARE_COUNT
};
constexpr auto PCOUNT = 6;

#define PIDX(x) ( ID_OFFSET + (x)*ID_FLARE_COUNT)
#define ID_NUM_PARAMS (ID_OFFSET + ID_FLARE_COUNT* PCOUNT) //15


enum MODE
{
	WhitebasedMask=1,
	BlackbasedMask,
	Alpha,
	Image
};

enum REVMODE
{
	None=1,
	Reverse,
	ReverseAndOriginalAlpha
};
enum BLEND
{
	Screen=1,
	Add,
	Normal,
};

//UIの表示文字列
#define	STR_ON				"on"
#define	STR_BASE_ON			"baseOn"
#define	STR_BASE_COLOR		"baseColor"
#define	STR_BASE_OPACITY	"baseOpacity"

#define	STR_MODE1			"mode"
#define	STR_MODE1_ITEMS		"WhitebackMask|BlackbackMask|Alpha|Image"
#define	STR_MODE1_COUNT		4
#define	STR_MODE1_DEFL		3

#define	STR_TOPIC			"flare_"
#define	STR_ENABLED			"enabled_"

#define	STR_BORDER_TOPIC	"border(except_image)_"
#define	STR_BORDER			"border_"
#define	STR_INSIDE			"inside_"
#define	STR_OUTSIDE			"outside_"

#define	STR_REV				"reverse(except_image)_"
#define	STR_REV_ITEMS		"none|reverse|reverseAndOriginalAlpha"
#define	STR_REV_COUNT		3
#define	STR_REV_DEFL		1
#define	STR_MAX				"max_"
#define	STR_BLUR			"blur_"
#define	STR_COLOR			"color(except_image)_"
#define	STR_BLEND			"blend_"
#define	STR_BLEND_ITEMS		"screen|add|normal"
#define	STR_BLEND_COUNT		3
#define	STR_BLEND_DFLT		1
#define	STR_OPACITY			"opacity_"



//UIのパラメータ
typedef struct FlareInfo {
	PF_Boolean enabled;
	PF_Boolean	border;
	A_long		rev;
	A_long		max;
	A_long		inside;
	A_long		outside;
	A_long		blur;
	PF_Pixel	color;
	A_long		blend;
	double		opacitry;
	PF_Handle	bufH;
}FlareInfo,*FlareInfoP, **FlareInfoH;

typedef struct FlareInfo16 {
	PF_Boolean enabled;
	PF_Boolean	border;
	A_long		rev;
	A_long		max;
	A_long		inside;
	A_long		outside;
	A_long		blur;
	PF_Pixel16	color;
	A_long		blend;
	double		opacitry;
	PF_Handle	bufH;
}FlareInfo16, *FlareInfo16P, **FlareInfo16H;


typedef struct ParamInfo {

	A_long		mode;
	PF_Boolean	BaseOn;
	PF_FpLong	BaseOpacity;
	PF_Pixel	BaseColor;
	FlareInfo	flareInfo[PCOUNT];

} ParamInfo, *ParamInfoP, **ParamInfoH;

typedef struct ParamInfo16 {

	A_long		mode;
	PF_Boolean	BaseOn;
	PF_FpLong	BaseOpacity;
	PF_Pixel16	BaseColor;
	FlareInfo16	flareInfo[PCOUNT];

} ParamInfo16, *ParamInfo16P, **ParamInfo16H;


//-------------------------------------------------------
PF_Err Exec08(CFsAE *ae, ParamInfo *infoP);
PF_Err MaxAll08(CFsAE *ae, FlareInfo *infoP);
PF_Err MaxRed08(CFsAE *ae, FlareInfo *infoP);
PF_Err RedToGreen08(CFsGraph *g);
PF_Err RevRedChannel08(CFsGraph *g);
PF_Err Border08(CFsAE *ae, FlareInfo *infoP);
PF_Err BlurRed08(CFsAE *ae, FlareInfo *infoP);
PF_Err BlurAll08(CFsAE *ae, FlareInfo *infoP);

PF_Err Exec16(CFsAE *ae, ParamInfo16 *infoP);
PF_Err MaxAll16(CFsAE *ae, FlareInfo16 *infoP);
PF_Err MaxRed16(CFsAE *ae, FlareInfo16 *infoP);
PF_Err RedToGreen16(CFsGraph *g);
PF_Err RevRedChannel16(CFsGraph *g);
PF_Err Border16(CFsAE *ae, FlareInfo16 *infoP);
PF_Err BlurRed16(CFsAE *ae, FlareInfo16 *infoP);
PF_Err BlurAll16(CFsAE *ae, FlareInfo16 *infoP);
//-----------------------------------------------------------------------------------
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
#endif // Flare_H
