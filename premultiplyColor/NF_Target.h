

#pragma once
#ifndef NF_TARGET_H
#define NF_TARGET_H

#include "..\_NFLib\NF_CATEGORY.h"
#include "..\_NFLib\NF_Version.h"

//-----------------------------------------------------------------------------------
//プラグインの識別に使われる名前

#define NF_NAME			"NF-PremultiplyColor"
#define NF_MATCHNAME	"NF-PremultiplyColor"


//-----------------------------------------------------------------------------------
//プラグインの説明に使われる文字
#define NF_DESCRIPTION	"PremultiplyColor"

//-----------------------------------------------------------------------------------
//プラグインが表示されるメニュー名
//#define NF_CATEGORY NF_CATEGORY_CHANNEL
//#define NF_CATEGORY NF_CATEGORY_DRAW
//#define NF_CATEGORY NF_CATEGORY_FILTER
//#define NF_CATEGORY NF_CATEGORY_NOISE
//#define NF_CATEGORY NF_CATEGORY_CELL
//#define NF_CATEGORY NF_CATEGORY_COLORISE
//#define NF_CATEGORY NF_CATEGORY_SCRIPT
//#define NF_CATEGORY NF_CATEGORY_BETA
//#define NF_CATEGORY NF_CATEGORY_LEGACY

//#define NF_CATEGORY FS_CATEGORY_NONE
//#define NF_CATEGORY FS_CATEGORY_CHANNEL
//#define NF_CATEGORY FS_CATEGORY_DRAW
//#define NF_CATEGORY FS_CATEGORY_FILTER
//#define NF_CATEGORY FS_CATEGORY_NOISE
//#define NF_CATEGORY FS_CATEGORY_CELL
//#define NF_CATEGORY FS_CATEGORY_COLORISE
//#define NF_CATEGORY FS_CATEGORY_SCRIPT
//#define NF_CATEGORY FS_CATEGORY_BETA
//#define NF_CATEGORY FS_CATEGORY_LEGACY

#define NF_CATEGORY  NF_CATEGORY_NONE



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

#define NF_OUT_FLAGS	33556032	//通常はこちら
//#define NF_OUT_FLAGS	33556068	//こっちを有効にすると毎フレームごとに描画する。NON_PARAM_VARYを動作中に切り替えるときもこちらに
//#define NF_OUT_FLAGS	1600		//8bitのみ

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
#define NF_OUT_FLAGS2 134222921


//-----------------------------------------------------------------------------------
#endif

