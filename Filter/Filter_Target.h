/*
	各種定数を設定

	PiPLリソースに使う為にマクロ展開は最低限

*/
#pragma once
#ifndef Filter_Target_H
#define Filter_Target_H

#include "../FsLibrary_next/FsVersion.h"



#define FS_DESCRIPTION		"Filter Effect Filter"
#define	FS_NAME				"F's Filter"
#define FS_MATCH_NAME		FS_NAME
#define FS_CREATER			"Copyright (c) 2019 bryful ( bryful@gmail.com/twitter:bryful )"
//-----------------------------------------------------------------------------------
//プラグインが表示されるメニュー名
	//#define FS_CATEGORY "F's Plugins-Channel"
	//#define FS_CATEGORY "F's Plugins-Draw"
	#define FS_CATEGORY "NF's Plugins-Filter"
	//#define FS_CATEGORY "F's Plugins-Cell"
	//#define FS_CATEGORY "F's Plugins-Colorize"
	//#define FS_CATEGORY "F's Plugins-Script"
	//#define FS_CATEGORY "F's Plugins-{Legacy}"


//#define SUPPORT_SMARTFX			//これを有効にするとSmartFX+Float_Colorに対応する



//value:4 [PF_OutFlag_NON_PARAM_VARY] 全フレームで描画する

//value:64 [PF_OutFlag_USE_OUTPUT_EXTENT] 表示画面全部
//value:16777216 [PF_OutFlag_I_HAVE_EXTERNAL_DEPENDENCIES]

//value:33554432[PF_OutFlag_DEEP_COLOR_AWARE] 16bit

//#define FS_OUT_FLAGS	50332164	//こっちにすると全フレーム描画する
#define FS_OUT_FLAGS	50332160	//通常はこちら



#if defined(SUPPORT_SMARTFX)
//value:8 [PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG] グループ
//value:1024 [PF_OutFlag2_SUPPORTS_SMART_RENDER] スマートレンダー
//value:4096 [PF_OutFlag2_FLOAT_COLOR_AWARE] 32bit
#define FS_OUT_FLAGS2	5128

#else
//value : 8[PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG]
#define FS_OUT_FLAGS2	8
#endif

#define FS_ABOUT_STR "function FsAbout()\
{\
	this.pluginName = \"%s\";\
	this.pluginVersion = \"Version %d.%d\";\
	this.pluginDis = \"%s\";\
	this.creater = \"%s\";\
	var winObj = new Window(\"dialog\", \"%s\", [ 0,  0,  500,  200] );\
	this.edFsName = winObj.add(\"edittext\", [  30,   12,   30+ 440,   12+  40], this.pluginName, { readonly:true, borderless:true });\
	this.edFsName.graphics.font = ScriptUI.newFont(\"Tahoma\",ScriptUI.FontStyle.BOLD, 32);\
	\
	this.edFsVersion = winObj.add(\"edittext\", [  70,   60,   70+ 430,   60+  20], this.pluginVersion, { readonly:true, borderless:true });\
	this.edFsVersion.graphics.font = ScriptUI.newFont(\"Tahoma\",ScriptUI.FontStyle.BOLD, 16);\
	\
	this.edFsDis = winObj.add(\"edittext\", [  30,   90,   30 + 470,   90 +  20], this.pluginDis, { readonly:true, borderless:true });\
	this.edFsDis.graphics.font = ScriptUI.newFont(\"Tahoma\",ScriptUI.FontStyle.REGULAR, 16);\
	\
	this.edMyName = winObj.add(\"edittext\", [  30,  120,   30+ 470,  120+  20], this.creater, { readonly:true, borderless:true });\
	this.edMyName.graphics.font = ScriptUI.newFont(\"Tahoma\",ScriptUI.FontStyle.REGULAR, 16);\
	var btnOK = winObj.add(\"button\", [ 360,  150,  360+  100,  150+  40], \"OK\" , { name:\"ok\" });\
	btnOK.graphics.font = ScriptUI.newFont(\"Tahoma\",ScriptUI.FontStyle.REGULAR, 11);\
	this.show = function()\
	{\
		winObj.center(); \
		return winObj.show();\
	}\
}\
var dlg = new FsAbout;\
\
dlg.show();\
"




#endif // Filter_Target_H

