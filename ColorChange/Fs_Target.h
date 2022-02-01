//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef FS_TAGET_H
#define FS_TAGET_H

//-----------------------------------------------------------------------------------
//プラグインの識別に使われる名前
#define FS_NAME			"F's ColorChange"

//-----------------------------------------------------------------------------------
//プラグインの説明に使われる文字
#define FS_DESCRIPTION	"単色の色変えをします"

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

#if defined(SUPPORT_SMARTFX)
#define FS_OUT_FLAGS2	5193
#else
#define FS_OUT_FLAGS2	73
#endif


#endif
