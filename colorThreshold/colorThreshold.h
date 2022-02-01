//-----------------------------------------------------------------------------------
/*
	colorThreshold for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef colorThreshold_H
#define colorThreshold_H

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
#include "../FsLibrary/FsHLS.h"
//#include "FsAE.h"

#include <stdio.h>
#include <Math.h>

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_ALPHA_LV,
	ID_HS_LV,
	ID_L_LV,
	ID_TOPIC_NAIN_COL,
	ID_MAIN_COL1_ON,
	ID_MAIN_COL1,
	ID_MAIN_COL1_HS,
	ID_MAIN_COL1_LV,
	ID_MAIN_COL2_ON,
	ID_MAIN_COL2,
	ID_MAIN_COL2_HS,
	ID_MAIN_COL2_LV,
	ID_MAIN_COL3_ON,
	ID_MAIN_COL3,
	ID_MAIN_COL3_HS,
	ID_MAIN_COL3_LV,
	ID_TOPIC_NAIN_COL_END,
	ID_TOPIC_SUB_COL,
	ID_SUB_COL_COUNT,
	ID_SUB_COL_START,

};
#define COUNT_OF_SUB	20
#define COUNT_OF_MAIN	3
#define ID_SUB_COL(idx)	( ID_SUB_COL_START + idx)
#define ID_TOPIC_SUB_COL_END	(ID_SUB_COL_START + COUNT_OF_SUB)
#define	ID_NUM_PARAMS	( ID_TOPIC_SUB_COL_END + 1 )
#define ID_MAIN_COL_ON(idx)	(ID_MAIN_COL1_ON + idx*4)
#define ID_MAIN_COL(idx)	(ID_MAIN_COL1 + idx*4)
#define ID_MAIN_COL_HS(idx)	(ID_MAIN_COL1_HS + idx*4)
#define ID_MAIN_COL_L(idx)	(ID_MAIN_COL1_LV + idx*4)

//UIの表示文字列
#define	STR_ALPHA_LV		"alpha_Level"
#define	STR_HS_LV			"colorMatch_level"
#define	STR_L_LV			"colorLightness_level"
#define	STR_MAIN_COL1_ON	"Enabled_1"
#define	STR_MAIN_COL2_ON	"Enabled_2"
#define	STR_MAIN_COL3_ON	"Enabled_3"
#define	STR_ON				"on"
#define	STR_MAIN_COL1		"color1"
#define	STR_MAIN_COL2		"color2"
#define	STR_MAIN_COL3		"color3"
#define	STR_MAIN_COL1_LV	"color1_level"
#define	STR_MAIN_COL2_LV	"color2_level"
#define	STR_MAIN_COL3_LV	"color3_level"
#define	STR_MAIN_COL1_HS	"color1_hs"
#define	STR_MAIN_COL2_HS	"color2_hs"
#define	STR_MAIN_COL3_HS	"color3_hs"

#define	STR_TOPIC_NAIN_COL	"LineColors"
#define	STR_TOPIC_SUB_COL	"UseColor"
#define	STR_SUB_COL_COUNT	"UseColor_count"

typedef struct hls {
	PF_FpLong	h;
	PF_FpLong	l;
	PF_FpLong	s;	
} hls;


/*
	0bit 1で対象ピクセル
	1bit 1で決定ピクセル
	2bit以降がテーブルインデックス。


*/
#define CT_NONE			0
#define CT_TARGET		1
#define CT_SEARCH		2
#define CT_DECISION		3
 

#define CT_FLOAT		1000

#define RETRY			10

typedef struct ParamInfo {
	
	PF_FpLong	alpha_level;
	A_u_char	alpha_border;

	PF_FpLong	hs_level;
	PF_FpLong	l_level;

	A_long		main_col_count;
	PF_Pixel	main_col[COUNT_OF_MAIN];
	PF_Pixel16	main_col16[COUNT_OF_MAIN];
	PF_Pixel32	main_col32[COUNT_OF_MAIN];
	PF_FpLong	main_l[COUNT_OF_MAIN];
	PF_FpLong	main_hs[COUNT_OF_MAIN];
	
	A_long		sub_col_count;
	PF_Pixel	sub_col[COUNT_OF_SUB];
	PF_Pixel16	sub_col16[COUNT_OF_SUB];
	PF_Pixel32	sub_col32[COUNT_OF_SUB];
	CFsGraph	*out;
	PF_Boolean	finFlag;

} ParamInfo, *ParamInfoP, **ParamInfoH;

hls rgbToHls (PF_Pixel p);
PF_FpLong colorDiatance(hls s,PF_Pixel c1);
PF_FpLong colorDiatance(hls s,PF_Pixel16 c1);
PF_FpLong colorDiatance(hls s,PF_PixelFloat c1);
PF_Boolean colorCompare(hls s,PF_Pixel c1,PF_FpLong hs,PF_FpLong l );
PF_Boolean colorCompare(hls s,PF_Pixel16 c1,PF_FpLong hs,PF_FpLong l );
PF_Boolean colorCompare(hls s,PF_PixelFloat c1,PF_FpLong hs,PF_FpLong l );

PF_Err AlphaThreshold8 (refconType	refcon, A_long xL, A_long yL, PF_Pixel8 *inP, PF_Pixel8	*outP);
PF_Err AlphaThreshold16 (refconType	refcon, A_long xL, A_long yL, PF_Pixel16 *inP, PF_Pixel16	*outP);
PF_Err AlphaThreshold32 (refconType	refcon, A_long xL, A_long yL, PF_PixelFloat *inP, PF_PixelFloat	*outP);

PF_Err AlphaFix8 (refconType refcon,A_long xL, A_long yL,PF_Pixel8 *inP, PF_Pixel8 *outP);
PF_Err AlphaFix16 (refconType refcon,A_long xL, A_long yL,PF_Pixel16 *inP, PF_Pixel16 *outP);
PF_Err AlphaFix32 (refconType refcon,A_long xL, A_long yL,PF_PixelFloat *inP, PF_PixelFloat *outP);

PF_Err hslMatch8 (refconType refcon, A_long xL, A_long yL, PF_Pixel8 *inP, PF_Pixel8 *outP);
PF_Err hslMatch16 (refconType refcon, A_long xL, A_long yL, PF_Pixel16 *inP, PF_Pixel16 *outP);
PF_Err hslMatch32 (refconType refcon, A_long xL, A_long yL, PF_PixelFloat *inP, PF_PixelFloat *outP);

PF_Err line_search8 (refconType	refcon, A_long xL, A_long yL, PF_Pixel8 *inP, PF_Pixel8 *outP);
PF_Err line_search16 (refconType	refcon, A_long xL, A_long yL, PF_Pixel16 *inP, PF_Pixel16 *outP);
PF_Err line_search32 (refconType	refcon, A_long xL, A_long yL, PF_PixelFloat *inP, PF_PixelFloat *outP);

PF_Err use_search8 (refconType refcon, A_long xL, A_long yL, PF_Pixel8 *inP, PF_Pixel8 *outP);
PF_Err use_search16 (refconType refcon, A_long xL, A_long yL, PF_Pixel16 *inP, PF_Pixel16 *outP);
PF_Err use_search32 (refconType refcon, A_long xL, A_long yL, PF_PixelFloat *inP, PF_PixelFloat *outP);

PF_Err use_searchSub8 (refconType refcon, A_long xL, A_long yL, PF_Pixel8 *inP, PF_Pixel8 *outP);
PF_Err use_searchSub16 (refconType refcon, A_long xL, A_long yL, PF_Pixel16 *inP, PF_Pixel16 *outP);
PF_Err use_searchSub32 (refconType refcon, A_long xL, A_long yL, PF_PixelFloat *inP, PF_PixelFloat *outP);
//-------------------------------------------------------

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
#endif // colorThreshold_H
