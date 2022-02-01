//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef ChannelShift_H
#define ChannelShift_H

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

#include "../FsLibrary/FsScreenShake.h"

#define	STR_RED_TOPIC		"Red"
#define	STR_RED_ROT			"Red_Rot"
#define	STR_RED_LENGTH		"Red_Length"

#define	STR_GREEN_TOPIC		"Green"
#define	STR_GREEN_ROT		"Green_Rot"
#define	STR_GREEN_LENGTH	"Green_Length"

#define	STR_BLUE_TOPIC		"Blue"
#define	STR_BLUE_ROT		"Blue_Rot"
#define	STR_BLUE_LENGTH		"Blue_Length"

#define	STR_ALPHA_TOPIC		"Alpha"
#define	STR_ALPHA_ROT		"Alpha_Rot"
#define	STR_ALPHA_LENGTH	"Alpha_Length"

#define	STR_EDGE_POP1		"Edge"
#define	STR_EDGE_POP2		"none|extend|tile|mirror"

#ifdef TEST_MODE
	#define	STR_TEST_TIME_CB1	"Time Disp"
	#define	STR_TEST_TIME_CB2	"ON"
#endif

enum {
	ID_INPUT = 0,	// default input layer 

	ID_RED_TOPIC,
	ID_RED_ROT,
	ID_RED_LENGTH,
	ID_RED_TOPIC_END,

	ID_GREEN_TOPIC,
	ID_GREEN_ROT,
	ID_GREEN_LENGTH,
	ID_GREEN_TOPIC_END,

	ID_BLUE_TOPIC,
	ID_BLUE_ROT,
	ID_BLUE_LENGTH,
	ID_BLUE_TOPIC_END,

	ID_ALPHA_TOPIC,
	ID_ALPHA_ROT,
	ID_ALPHA_LENGTH,
	ID_ALPHA_TOPIC_END,

	ID_EDGE_POP,

	ID_NUM_PARAMS
	};


enum {
	edge_none =1,
	edge_fill,
	edge_rep,
	edge_mirror
};

//プラグイン独自のパラメータを集めた構造体
typedef struct{
	PF_Fixed			r_angle;
	PF_Fixed			r_length;
	PF_Fixed			g_angle;
	PF_Fixed			g_length;
	PF_Fixed			b_angle;
	PF_Fixed			b_length;
	PF_Fixed			a_angle;
	PF_Fixed			a_length;
	A_long				edge_status;
	channelShiftPrm		cs;
	getPixelPrm			gp;
} ParamInfo;



//-------------------------------------------------------

extern "C" {

DllExport	PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP);

}
//-----------------------------------------------------------------------------------

#endif // ChannelShift_H
