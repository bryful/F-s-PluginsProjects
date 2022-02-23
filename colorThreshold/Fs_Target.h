//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#pragma once
#ifndef Fs_TARGET_H
#define Fs_TARGET_H


//-----------------------------------------------------------------------------------
//プラグインの識別に使われる名前
#define FS_NAME			"F's colorThreshold"

//-----------------------------------------------------------------------------------
//プラグインの説明に使われる文字
#define FS_DESCRIPTION	"色を単色化します。"

//-----------------------------------------------------------------------------------
//プラグインが表示されるメニュー名
//#define FS_CATEGORY "F's Plugins-Channel"
//#define FS_CATEGORY "F's Plugins-Draw"
//#define FS_CATEGORY "F's Plugins-Filter"
#define FS_CATEGORY "NF's Plugins-Cell"
//#define FS_CATEGORY "F's Plugins-Colorize"
//#define FS_CATEGORY "F's Plugins-Script"
//#define FS_CATEGORY "F's Plugins-Test"


//-----------------------------------------------------------
#define SUPPORT_SMARTFX			//これを有効にするとSmartFX+Float_Colorに対応する
//#define NO_USE_FSGRAPHICS	//これを有効にするとFsGraphics関係がインクルードされない

//-----------------------------------------------------------------------------------
#include "../FsLibrary/FsVersion.h"


//-----------------------------------------------------------------------------------
//out_flags
/*
out_data->out_flags
	PF_OutFlag_PIX_INDEPENDENT		1024
	PF_OutFlag_NON_PARAM_VARY			4
	PF_OutFlag_DEEP_COLOR_AWARE		33554432
	PF_OutFlag_USE_OUTPUT_EXTENT	64
	PF_OutFlag_I_EXPAND_BUFFER		512
	PF_OutFlag_I_DO_DIALOG				32
*/

#define FS_OUT_FLAGS	33556032	//通常はこちら
//#define FS_OUT_FLAGS	33556036	//こっちを有効にすると毎フレームごとに描画する。NON_PARAM_VARYを動作中に切り替えるときもこちらに
//#define FS_OUT_FLAGS	1600		//8bitのみ

//-----------------------------------------------------------------------------------
//out_flags2
/*
out_data->out_flags2
	PF_OutFlag2_FLOAT_COLOR_AWARE
	PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG
	PF_OutFlag2_SUPPORTS_SMART_RENDER
	PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS
	PF_OutFlag2_DOESNT_NEED_EMPTY_PIXELS;
*/
#if defined(SUPPORT_SMARTFX)
#define FS_OUT_FLAGS2	134222921
#else
#define FS_OUT_FLAGS2 134217801
#endif


//-----------------------------------------------------------------------------------
#endif
