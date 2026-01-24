//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010 and VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef FS_TAGET_H
#define FS_TAGET_H

#define FS_NAME			"F's _sample_font"
#define FS_DESCRIPTION	"Fill transparent areas."

//-----------------------------------------------------------------------------------
//プラグインが表示されるメニュー名
#define FS_CATEGORY "NF's Plugins-Channel"
//#define FS_CATEGORY "NF's Plugins-Draw"
//#define FS_CATEGORY "NF's Plugins-Filter"
//#define FS_CATEGORY "NF's Plugins-FX"
//#define FS_CATEGORY "NF's Plugins-Cell"
//#define FS_CATEGORY "NF's Plugins-Script"
//#define FS_CATEGORY "NF's Plugins-Test"


//-----------------------------------------------------------
#ifndef SUPPORT_SMARTFX			//２重登録を防ぐ
#define SUPPORT_SMARTFX			//これを有効にするとSmartFX+Float_Colorに対応する
#endif
//#define NO_USE_FSGRAPHICS	//これを有効にするとFsGraphics関係がインクルードされない


//-----------------------------------------------------------------------------------
#include "../FsLibrary/FsVersion.h"
//-----------------------------------------------------------------------------------
//out_flags
/*
	out_data->out_flags	  =	
		PF_OutFlag_PIX_INDEPENDENT
		//| PF_OutFlag_NON_PARAM_VARY	
		| PF_OutFlag_DEEP_COLOR_AWARE
		| PF_OutFlag_USE_OUTPUT_EXTENT
		| PF_OutFlag_I_EXPAND_BUFFER;
		//33556032	
		//33556036 +PF_OutFlag_NON_PARAM_VARY
*/
#define FS_OUT_FLAGS 33556032
//#define Fs_OUTFLAGS 33556036

//-----------------------------------------------------------------------------------
//out_flags2
/*
	out_data->out_flags2  =	PF_OutFlag2_FLOAT_COLOR_AWARE
						  | PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG
						  | PF_OutFlag2_SUPPORTS_SMART_RENDER
						  | PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS
						  | PF_OutFlag2_DOESNT_NEED_EMPTY_PIXELS
						  | PF_OutFlag2_SUPPORTS_THREADED_RENDERING;		// PF_Cmd_GLOBAL_SETUP

						//5193
						//134222921
*/
#if defined(SUPPORT_SMARTFX)
//#define FS_OUT_FLAGS2 134222921
#define FS_OUT_FLAGS2	134222921
#else
#define FS_OUT_FLAGS2 134217801
#endif

#endif

