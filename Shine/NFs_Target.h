
#pragma once
#ifndef NFs_TARGET_H
#define NFs_TARGET_H

#include "../NFsLibrary/NFsVersion.h"


//-----------------------------------------------------------------------------------
//プラグインの識別に使われる名前
#define NFS_NAME			"F's Shine"
#define NFS_DISPNAME		"NF's Shine"


//-----------------------------------------------------------------------------------
//プラグインの説明に使われる文字
#define NFS_DESCRIPTION	"Fake shine"

	//-----------------------------------------------------------------------------------
	//プラグインが表示されるメニュー名
	//#define NFS_CATEGORY "NF's Plugins-Cell"
	//#define NFS_CATEGORY "NF's Plugins-Channel"
	//#define NFS_CATEGORY "NF's Plugins-Colorize"
	//#define NFS_CATEGORY "NF's Plugins-Draw"
	//#define NFS_CATEGORY "NF's Plugins-Filter"
	//#define NFS_CATEGORY "NF's Plugins-Noise"
	//#define NFS_CATEGORY "NF's Plugins-Script"
	//#define NFS_CATEGORY "NF's Plugins-{DEBUG}"
	//#define NFS_CATEGORY "NF's Plugins-{Legacy}"

//#define NFS_CATEGORY "NF's Plugins-{Legacy}"
	#define NFS_CATEGORY "NF's Plugins-Filter"

	//-----------------------------------------------------------
#define SUPPORT_SMARTFX			//これを有効にするとSmartFX+Float_Colorに対応する


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

#define NFS_OUT_FLAGS	33556032	//通常はこちら
//#define NFS_OUT_FLAGS	33556036	//こっちを有効にすると毎フレームごとに描画する。NON_PARAM_VARYを動作中に切り替えるときもこちらに
//#define NFS_OUT_FLAGS	1600		//8bitのみ

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
#define NFS_OUT_FLAGS2	134222921
#else
#define NFS_OUT_FLAGS2	134217801
#endif


//-----------------------------------------------------------------------------------
#endif
