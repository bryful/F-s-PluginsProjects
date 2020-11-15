#pragma once
#ifndef AEINFO_H
#define AEINFO_H

#include "AE_SDK.h"


/*
	プラグインID獲得のための構造体
*/
typedef struct {
	PF_Boolean		initializedB;
	AEGP_PluginID	my_id;
} ae_global_data, * ae_global_dataP, ** ae_global_dataH;

//******************************************************************************
#define FS_ABOUT_DIALOG	"var  FsAbout = function()\
{\
	var strName = \"%s\";\
	var strVersion = \"version %d.%d\";\
	var strDis = \"%s\";\
	var strMyName = \"https://github.com/bryful : bryful@gmail.com \";\
    var nanae = \"Nanae Furuhashi - My daughter, May her soul rest in peace．\";\
	var winObj = new Window(\"dialog\", \"NF's Plugins\", [ 0,  0,  480, 180] );\
\
	var edFsName = winObj.add(\"edittext\", [  30,   10,   30+ 440,   10+  20], strName, { readonly:true, borderless:true });\
	var edFsVersion = winObj.add(\"edittext\", [  70,   40,   70+ 400,   40+ 20], strVersion, { readonly:true, borderless:true });\
	var edFsDis = winObj.add(\"edittext\", [  30,   70,   30+ 440,   70+  20], strDis, { readonly:true, borderless:true });\
	var edMyName = winObj.add(\"edittext\", [  30,  100,   30+ 440,  100+  20], strMyName, { readonly:true, borderless:true });\
    var stNana = winObj.add(\"statictext\", [  30,  120,   30+ 440,  120+  20], nanae, { readonly:true, borderless:true });\
	var btnOK = winObj.add(\"button\", [ 360,  140,  360+ 100,  140+  24], \"OK\" , { name:\"ok\" });\
	this.show = function()\
	{\
		winObj.center();\
		return winObj.show();\
	}\
}\
var dlg = new FsAbout;\
dlg.show();"

//******************************************************************************
#define AEInfo_ITEM_COUNT	256
//******************************************************************************
//******************************************************************************
class AEInfo {
private:
protected:
	PF_PixelFormat		m_format;
	A_long				m_frame;
	PF_Cmd				m_mode;
	A_long				m_paramsCount;
	PF_Err				m_resultErr;
	PF_Boolean			m_isGetEffectStream;

public:
	PF_InData* in_data;
	PF_OutData* out_data;
	PF_ParamDef* params[AEInfo_ITEM_COUNT];

	PF_SmartRenderExtra* SRextraP;
	PF_PreRenderExtra* PRextraP;
	PF_WorldSuite2* ws2P;

	PF_EffectWorld* input;
	PF_EffectWorld* output;
	PF_Handle			PreRenderH;
	AEGP_SuiteHandler* suitesP;
	PF_Handle			ae_plugin_idH;
	ae_global_dataP		ae_plugin_idP;
	AEGP_StreamRefH		ae_item_streamH[AEInfo_ITEM_COUNT];
	AEGP_EffectRefH		ae_effect_refH;
	//*********************************************************************************
	void Init()
	{
		m_format = PF_PixelFormat_INVALID;
		m_frame = 0;
		m_mode = PF_Cmd_ABOUT;
		m_paramsCount = 0;
		m_isGetEffectStream = FALSE;
		m_resultErr = PF_Err_NONE;

		in_data = NULL;
		out_data = NULL;
		input = NULL;
		output = NULL;

		PreRenderH = NULL;
		PRextraP = NULL;
		SRextraP = NULL;
		ws2P = NULL;
		suitesP = NULL;
		ae_plugin_idH = NULL;
		ae_plugin_idP = NULL;

		ae_effect_refH = NULL;
		for (A_long i = 0; i < AEInfo_ITEM_COUNT; i++) CFsAE::ae_item_streamH[i] = NULL;

	}
	//******************************************************************************
	AEInfo()
	{
		Init();
	}
	//******************************************************************************
};
//******************************************************************************

#endif