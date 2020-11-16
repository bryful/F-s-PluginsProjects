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
#define NFS_ABOUT_DIALOG	"var  FsAbout = function()\
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
	PF_Cmd				m_cmd;
	A_long				m_paramsCount;
	A_long				m_infoSize;
	PF_Err				m_err;
	PF_Boolean			m_isGetEffectStream;

public:
	PF_PixelFormat		fromat() { return m_format; }
	A_long				frame() { return m_frame; }
	PF_Cmd				cmd() { return m_cmd; }
	A_long				paramsCount() { return m_paramsCount; }
	PF_Err				err() { return m_err; }
	PF_Boolean			isGetEffectStream() { return m_isGetEffectStream; }


	PF_InData*				in_data;
	PF_OutData*				out_data;
	PF_ParamDef*			params[AEInfo_ITEM_COUNT];

	PF_SmartRenderExtra*	SRextraP;
	PF_PreRenderExtra*		PRextraP;
	PF_WorldSuite2*			ws2P;

	PF_EffectWorld*			input;
	PF_EffectWorld*			output;

	PF_Handle				PreRenderH;
	AEGP_SuiteHandler*		suitesP;
	PF_Handle				ae_plugin_idH;
	ae_global_dataP			ae_plugin_idP;
	AEGP_StreamRefH			ae_item_streamH[AEInfo_ITEM_COUNT];
	AEGP_EffectRefH			ae_effect_refH;
	//*********************************************************************************
	void Init()
	{
		m_format = PF_PixelFormat_INVALID;
		m_frame = 0;
		m_cmd = PF_Cmd_ABOUT;
		m_paramsCount = 0;
		m_infoSize = 0;
		m_isGetEffectStream = FALSE;
		m_err = PF_Err_NONE;

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
		for (A_long i = 0; i < AEInfo_ITEM_COUNT; i++) ae_item_streamH[i] = NULL;

	}
	//******************************************************************************
	AEInfo()
	{
		Init();
	}

protected:
	//******************************************************************************
	PF_Err GetFrame(
		PF_InData* in_dataP 
	)
	{
		PF_Err	err = PF_Err_NONE;
		if (in_dataP == NULL)
		{
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		}
		in_data = in_dataP;
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}

		return err;
	}
	//******************************************************************************
	PF_Err GetSuites(
		PF_InData* in_dataP
	)
	{
		PF_Err	err = PF_Err_NONE;
		if (in_dataP == NULL)
		{
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		}
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
		if (in_dataP->global_data) {
			ae_plugin_idH = in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}
		return err;
	}
public:
	//******************************************************************************
	PF_Err AboutBox
	(
		A_char* Dispname,
		A_char* majorver,
		A_char* minorver,
		A_char* des
		)
	{
		PF_Err	err = PF_Err_NONE;
		if (ae_plugin_idP != NULL) {
			A_char scriptCode[2048] = { '\0' };

			PF_SPRINTF(scriptCode, FS_ABOUT_DIALOG,
				Dispname,
				majorver,
				minorver,
				des);

			ERR(suitesP->UtilitySuite5()->AEGP_ExecuteScript(ae_plugin_idP->my_id, scriptCode, TRUE, NULL, NULL));

		}
		else {
			PF_SPRINTF(out_data->return_msg,
				"%s, v%d.%d\r%s",
				Dispname,
				majorver,
				minorver,
				des);
		}

		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	About(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP)
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_ABOUT;

		in_data = in_dataP;
		GetFrame(in_dataP);
		GetSuites(in_dataP);

		return err;
	}
	//******************************************************************************
	virtual
		PF_Err	GlobalSetup(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP)
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		in_data = in_dataP;
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);


		return err;
	}
	//******************************************************************************
	virtual
		PF_Err	GlobalSetdown(
			PF_InData* in_dataP)
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		in_data = in_dataP;

		return PF_Err_NONE;
	}
	//******************************************************************************
	virtual
		PF_Err	ParamsSetup(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP)
	{
		PF_Err			err = PF_Err_NONE;
		Init();
		in_data = in_dataP;

		//PF_ParamDef		def;
		//out_data->num_params = ID_NUM_PARAMS;

		return err;
	}
	//******************************************************************************
	virtual
		PF_Err SequenceSetup(
			PF_InData * in_dataP,
			PF_OutData * out_data,
			PF_ParamDef * paramsP[],
			PF_LayerDef * outputP)
	{
		Init();
		in_data = in_dataP;

		return PF_Err_NONE;
	}	
	//******************************************************************************
	virtual
		PF_Err	SequenceSetdown(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP)
	{
		Init();
		in_data = in_dataP;
		return PF_Err_NONE;
	}

	//******************************************************************************
	virtual
	PF_Err SequenceResetup(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP)
	{
		in_data = in_dataP;
		Init();
		return PF_Err_NONE;
	}
	//******************************************************************************
	virtual
	PF_Err	HandleChangedParam(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			PF_UserChangedParamExtra* extraP)
	{
		PF_Err				err = PF_Err_NONE;
		Init();
		in_data = in_dataP;
		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	QueryDynamicFlags(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			void* extraP)
	{
		PF_Err 	err = PF_Err_NONE;
		Init();
		in_data = in_dataP;
		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	RespondtoAEGP(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			void* extraP)
	{
		PF_Err			err = PF_Err_NONE;
		Init();
		in_data = in_dataP;

		AEGP_SuiteHandler suites(in_dataP->pica_basicP);

		suites.ANSICallbacksSuite1()->sprintf(out_dataP->return_msg,
			"%s",
			reinterpret_cast<A_char*>(extraP));

		return err;
	}

	//******************************************************************************
	virtual
	PF_Err	Render(
			PF_InData*		in_dataP,
			PF_OutData*		out_dataP,
			PF_ParamDef*	paramsP[],
			PF_LayerDef*	outputP

	)
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		if ((in_dataP == NULL) || (out_dataP == NULL) || (paramsP == NULL) || (outputP == NULL)) {
			m_err = PF_Err_BAD_CALLBACK_PARAM;
			return m_err;
		}
		in_data = in_dataP;

		GetFrame(in_dataP, out_dataP);

		out_data = out_dataP;
		output = outputP;

		//Bit
		if (PF_WORLD_IS_DEEP(outputP) == TRUE) {
			m_format = PF_PixelFormat_ARGB64;
		}
		else {
			m_format = PF_PixelFormat_ARGB32;
		}

		if (paramsP != NULL) {
			input = &paramsP[0]->u.ld;
			if (m_paramsCount > 0) {
				for (A_long i = 0; i < m_paramsCount; i++) params[i] = paramsP[i];
			}
		}

		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	PreRender(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_PreRenderExtra* extraP)
	{
		PF_Err		err = PF_Err_NONE;
		in_data = in_dataP;

		ERR(GetFrame(in_dataP, out_dataP));
		if (err)
		{
			m_err = err;
			return m_err;
		}
		out_data = out_dataP;
		PRextraP = extraP;
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);

		if (in_dataP->global_data) {
			ae_plugin_idH = in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}

		if (m_infoSize > 0) {
			PreRenderH = suitesP->HandleSuite1()->host_new_handle(m_infoSize);
			if (PreRenderH == NULL) {
				m_err = PF_Err_OUT_OF_MEMORY;
				return m_err;
			}
		}
		suitesP->HandleSuite1()->host_lock_handle(PreRenderH);


		/*
		CFsAE ae(in_data, out_data, extraP, sizeof(ParamInfo), ID_NUM_PARAMS);
		err = ae.resultErr();
		if (!err) {

			ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
			if (infoP) {
				ae.SetHostPreRenderData();
				ERR(GetParams(&ae, infoP));
				ERR(ae.UnSetPreRenderData());
				ae.UnlockPreRenderData();
			}
			else {
				err = PF_Err_OUT_OF_MEMORY;
			}
		}
		*/
		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	SmartRender(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_SmartRenderExtra* extraP)
	{
		PF_Err	err = PF_Err_NONE;
		PF_Err	err2 = PF_Err_NONE;
		/*
		CFsAE ae(in_data, out_data, extraP, ID_NUM_PARAMS);
		err = ae.resultErr();
		if (!err) {
			ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
			if (infoP) {
				ERR(Exec(&ae, infoP));
				ERR2(ae.UnsetSmartRender());
				ae.UnlockPreRenderData();
			}
			else {
				err = PF_Err_OUT_OF_MEMORY;
			}
		}
		*/
		return err;
	}
	//******************************************************************************
	AEInfo(
		PF_Cmd			cmd,
		PF_InData*		in_dataP,
		PF_OutData*		out_dataP,
		PF_ParamDef*	paramsP[],
		PF_LayerDef*	outputP,
		void*			extraP,
		A_long			pc,
		A_long			infoSz
		)
	{
		PF_Err		err = PF_Err_NONE;
		Init();

		m_err = err;

		m_cmd = cmd;
		in_data = in_dataP;
		m_paramsCount = pc;
		m_infoSize = infoSz;

		try
		{
			switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_dataP);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceResetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_RENDER:
				err = Render(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_dataP, out_dataP, reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(in_dataP, out_data, reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
			case PF_Cmd_COMPLETELY_GENERAL:
				err = RespondtoAEGP(in_dataP, out_dataP, paramsP, outputP, extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;
			case PF_Cmd_USER_CHANGED_PARAM:
				err = HandleChangedParam(in_dataP,
					out_dataP,
					paramsP,
					outputP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_dataP,
					out_dataP,
					paramsP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			}
		}
		catch (PF_Err& thrown_err) {
			err = thrown_err;
		}
		m_err = err;
	}
	//******************************************************************************
};
//******************************************************************************

#endif