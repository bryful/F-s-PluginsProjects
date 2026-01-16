#pragma once
#ifndef AEINFO_H
#define AEINFO_H

#include "AE_SDK.h"


typedef struct {
	PF_FpLong	x, y, z;
} Point3D;

/*
	プラグインID獲得のための構造体
*/
typedef struct {
	PF_Boolean		initializedB;
	AEGP_PluginID	my_id;
} ae_global_data, * ae_global_dataP, ** ae_global_dataH;

#pragma region About


//******************************************************************************
#define NFS_ABOUT_DIALOG	"var  FsAbout = function()\
{\
	var strName = \"%s\";\
	var strVersion = \"version %s.%s\";\
	var strDis = \"%s\";\
	var strMyName = \"https://github.com/bryful : bryful@gmail.com \";\
    var nanae = \"Nanae Furuhashi - My daughter, May her soul rest in peace．\";\
	var winObj = new Window(\"dialog\", \"NF's Plugins\", [ 0,  0,  480, 180] );\
\
	var edFsName = winObj.add(\"edittext\", [  30,   10,   30+ 440,   10+  20], strName, { readonly:true, borderless:true });\
	var edFsVersion = winObj.add(\"edittext\", [  30,   40,   30+ 440,   40+ 20], strVersion, { readonly:true, borderless:true });\
	var edFsDis = winObj.add(\"edittext\", [  30,   70,   30+ 440,   70+  20], strDis, { readonly:true, borderless:true });\
	var edMyName = winObj.add(\"edittext\", [  30,  100,   30+ 440,  100+  20], strMyName, { readonly:true, borderless:true });\
    var stNana = winObj.add(\"statictext\", [  30,  130,   30+ 440,  130+  20], nanae, { readonly:true, borderless:true });\
	var btnOK = winObj.add(\"button\", [ 360,  140,  360+ 100,  140+  24], \"OK\" , { name:\"ok\" });\
	this.show = function()\
	{\
		winObj.center();\
		return winObj.show();\
	}\
}\
var dlg = new FsAbout;\
dlg.show();"

#pragma endregion

//******************************************************************************
#define AEInfo_ITEM_COUNT	256
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
	//PF_Boolean			m_isGetEffectStream;

public:
	PF_PixelFormat		pixelFormat() { return m_format; }
	A_long				frame() { return m_frame; }
	PF_Cmd				cmd() { return m_cmd; }
	A_long				paramsCount() { return m_paramsCount; }
	PF_Err				err() { return m_err; }
	//PF_Boolean			isGetEffectStream() { return m_isGetEffectStream; }


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
		//m_isGetEffectStream = FALSE;
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
	~AEInfo()
	{
		PF_Err 	err = PF_Err_NONE;
		PF_Err 		err2 = PF_Err_NONE;
		if (ae_effect_refH != NULL) {
			suitesP->EffectSuite2()->AEGP_DisposeEffect(ae_effect_refH);
			ae_effect_refH = NULL;
		}
		/*
		if (m_paramsCount > 0) {
			if (m_isGetEffectStream == TRUE) {
				for (A_long i = 0; i < m_paramsCount; i++) {
					if (ae_item_streamH[i] != NULL) {
						ERR2(suitesP->StreamSuite2()->AEGP_DisposeStream(ae_item_streamH[i]));
						ae_item_streamH[i] = NULL;
					}
				}
			}
		}
		*/
		if (suitesP != NULL) {
			delete suitesP;
			suitesP = NULL;
		}

		if (ws2P != NULL) {
			ERR2(AEFX_ReleaseSuite(in_data,
				out_data,
				kPFWorldSuite,
				kPFWorldSuiteVersion2,
				"Couldn't release suite."));
		}
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
#pragma region Sub
	//--------------------------------------------------------------------
	void* LockPreRenderData()
	{
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER) {
			if (PreRenderH != NULL) {
				return suitesP->HandleSuite1()->host_lock_handle(PreRenderH);
			}
		}
		else if (m_cmd == PF_Cmd_SMART_RENDER) {
			if (SRextraP != NULL) {
				return suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
		return NULL;
	}	//******************************************************************************
	PF_Err SetHostPreRenderData()
	{
		PF_Err err = PF_Err_BAD_CALLBACK_PARAM;
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER) {
			if (PreRenderH != NULL) {
				PRextraP->output->pre_render_data = PreRenderH;
				err = PF_Err_NONE;
			}
		}
		return err;
	}
	// ******************************************************************************
	PF_Err UnSetPreRenderData(PF_RenderRequest* req, PF_CheckoutResult* in_result)
	{
		PF_Err err = PF_Err_NONE;
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER) {
			ERR(PRextraP->cb->checkout_layer(in_data->effect_ref,
				0,
				0,
				req,
				in_data->current_time,
				in_data->time_step,
				in_data->time_scale,
				in_result));

			UnionLRect(&in_result->result_rect, &PRextraP->output->result_rect);
			UnionLRect(&in_result->max_result_rect, &PRextraP->output->max_result_rect);
		}
		return err;
	}
	// ******************************************************************************
	PF_Err UnSetPreRenderData()
	{
		PF_RenderRequest req = PRextraP->input->output_request;
		PF_CheckoutResult in_result;
		return UnSetPreRenderData(&req, &in_result);
	}
	// ******************************************************************************
	PF_Err  UnlockPreRenderData()
	{
		PF_Err err = PF_Err_NONE;
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER) {
			if (PreRenderH != NULL) {
				suitesP->HandleSuite1()->host_unlock_handle(PreRenderH);
			}
		}
		else if (m_cmd == PF_Cmd_SMART_RENDER) {
			if (SRextraP != NULL) {
				suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
		return err;
	}
	// ******************************************************************************
	PF_Err UnsetSmartRender()
	{
		return SRextraP->cb->checkin_layer_pixels(in_data->effect_ref, 0);
	}
#pragma endregion
	// ******************************************************************************
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

			PF_SPRINTF(scriptCode, NFS_ABOUT_DIALOG,
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
#pragma region  Cmd


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
		out_data = out_dataP;
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
		m_cmd = PF_Cmd_GLOBAL_SETUP;
		in_data = in_dataP;
		out_data = out_dataP;
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
		m_cmd = PF_Cmd_GLOBAL_SETDOWN;
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
		in_data = in_dataP;

		//ae_plugin_idH
		if (in_dataP->global_data) {
			suitesP->HandleSuite1()->host_dispose_handle(in_dataP->global_data);
		}
		return err;
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
		m_cmd = PF_Cmd_PARAMS_SETUP;
		in_data = in_dataP;
		out_data = out_dataP;

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
		m_cmd = PF_Cmd_SEQUENCE_RESETUP;
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
		m_cmd = PF_Cmd_SEQUENCE_SETDOWN;
		in_data = in_dataP;
		out_data = out_dataP;

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
		Init();
		m_cmd = PF_Cmd_SEQUENCE_RESETUP;
		in_data = in_dataP;
		out_data = out_dataP;
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
		m_cmd = PF_Cmd_USER_CHANGED_PARAM;
		in_data = in_dataP;
		out_data = out_dataP;
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
		m_cmd = PF_Cmd_QUERY_DYNAMIC_FLAGS;
		in_data = in_dataP;
		out_data = out_dataP;
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
		out_data = out_dataP;
		m_cmd = PF_Cmd_COMPLETELY_GENERAL;

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
			PF_LayerDef*	outputP,
			A_long			pc

	)
	{
		PF_Err	err = PF_Err_NONE;

		Init();
		if ((in_dataP == NULL) || (out_dataP == NULL) || (paramsP == NULL) || (outputP == NULL) || (pc <= 0)) {
			m_err = PF_Err_BAD_CALLBACK_PARAM;
			return m_err;
		}
		m_cmd = PF_Cmd_RENDER;
		in_data = in_dataP;
		m_paramsCount = pc;
		out_data = out_dataP;
		output = outputP;
		GetFrame(in_dataP);

		//Bit
		if (PF_WORLD_IS_DEEP(outputP) == TRUE) {
			m_format = PF_PixelFormat_ARGB64;
		}
		else {
			m_format = PF_PixelFormat_ARGB32;
		}

		if (paramsP != NULL) {
			input = &paramsP[0]->u.ld;
			if (pc > 0) {
				for (A_long i = 0; i < pc; i++) params[i] = paramsP[i];
			}
		}
		output = outputP;
		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	PreRender(
			PF_InData*			in_dataP,
			PF_OutData*			out_dataP,
			PF_PreRenderExtra*	extraP,
			A_long				pc,
			A_long				infoSize
		)
	{
		PF_Err		err = PF_Err_NONE;
		Init();

		m_cmd = PF_Cmd_SMART_PRE_RENDER;
		in_data = in_dataP;
		out_data = out_dataP;
		PRextraP = extraP;
		m_paramsCount = pc;


		ERR(GetFrame(in_dataP));
		if (err)
		{
			m_err = err;
			return m_err;
		}
		GetSuites(in_dataP);

		if (infoSize > 0) {
			PreRenderH = suitesP->HandleSuite1()->host_new_handle(infoSize);
			if (PreRenderH == NULL) {
				m_err = PF_Err_OUT_OF_MEMORY;
				return m_err;
			}
		}
		return err;
	}
	//******************************************************************************
	virtual
	PF_Err	SmartRender(
			PF_InData*				in_dataP,
			PF_OutData*				out_dataP,
			PF_SmartRenderExtra*	extraP,
			A_long					pc
		)
	{
		PF_Err	err = PF_Err_NONE;
		PF_Err	err2 = PF_Err_NONE;
		if ((in_dataP == NULL) || (out_dataP == NULL) || (extraP == NULL)) {
			m_err = PF_Err_BAD_CALLBACK_PARAM;
			return m_err;
		}

		Init();
		m_cmd = PF_Cmd_SMART_RENDER;
		in_data = in_dataP;
		out_data = out_dataP;
		SRextraP = extraP;
		m_paramsCount = pc;
		GetFrame(in_dataP);
		GetSuites(in_dataP);
		ERR((extraP->cb->checkout_layer_pixels(in_dataP->effect_ref, 0, &(input))));
		ERR(extraP->cb->checkout_output(in_dataP->effect_ref, &(output)));

		ERR(AEFX_AcquireSuite(in_dataP,
			out_dataP,
			kPFWorldSuite,
			kPFWorldSuiteVersion2,
			"Couldn't load suite.",
			(void**)&(ws2P)));
		ERR(ws2P->PF_GetPixelFormat(input, &m_format));

		if (err) {
			m_err = PF_Err_BAD_CALLBACK_PARAM;
			return m_err;
		}

		return err;
	}
#pragma endregion

#pragma region Get Params


	//******************************************************************************
	PF_FpLong ParamDownScaleX()
	{
		return (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den;

	}
	PF_FpLong ParamDownScaleY()
	{
		return (PF_FpLong)in_data->downsample_y.num / (PF_FpLong)in_data->downsample_y.den;

	}
	//******************************************************************************
	PF_Err GetADD(A_long idx, A_long* a)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret = 0;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = param.u.sd.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.sd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*a = ret;
		return err;
	}
	//******************************************************************************
	PF_Err GetFIXED(A_long idx, PF_Fixed* f)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret = 0;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = param.u.sd.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.sd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*f = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetANGLE_FIXED(A_long idx, PF_Fixed* r)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret = 0;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = param.u.ad.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.ad.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*r = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetANGLE_DOUBLE(A_long idx, PF_FpLong* r)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed rr;
		err = GetANGLE_FIXED(idx, &rr);

		*r = (PF_FpLong)rr / 65536.0;

		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetCHECKBOX(A_long idx, PF_Boolean* b)
	{
		PF_Err err = PF_Err_NONE;
		PF_Boolean ret = FALSE;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = (PF_Boolean)param.u.bd.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = (PF_Boolean)params[idx]->u.bd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*b = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetCOLOR(A_long idx, PF_Pixel* c)
	{
		PF_Err err = PF_Err_NONE;
		PF_Pixel ret = { 0,0,0,0 };
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = param.u.cd.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.cd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*c = ret;
		return err;


	}
	//--------------------------------------------------------------------
	PF_Err GetFLOAT(A_long idx, PF_FpLong* f)
	{
		PF_Err err = PF_Err_NONE;
		PF_FpLong ret = 0;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = param.u.fs_d.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.fs_d.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*f = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetPOINT_FIXED(A_long idx, PF_FixedPoint* pos)
	{
		PF_Err err = PF_Err_NONE;
		PF_FixedPoint ret;
		ret.x = -1;
		ret.y = -1;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret.x = param.u.td.x_value;
					ret.y = param.u.td.y_value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret.x = params[idx]->u.td.x_value;
				ret.y = params[idx]->u.td.y_value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*pos = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetPOINT_DOUBLE(A_long idx, A_FloatPoint* pos)
	{
		PF_Err err = PF_Err_NONE;
		PF_FixedPoint r;
		r.x = 0; r.y = 0;
		err = GetPOINT_FIXED(idx, &r);
		if (!err) {

			pos->x = (PF_FpLong)r.x / 65536;
			pos->y = (PF_FpLong)r.y / 65536;
		}
		return err;

	}
	//--------------------------------------------------------------------
	PF_Err GetPOINT_INT(A_long idx, A_LPoint* pos)
	{
		PF_Err err = PF_Err_NONE;
		PF_FixedPoint r;
		r.x = 0; r.y = 0;
		err = GetPOINT_FIXED(idx, &r);
		if (!err) {

			pos->x = (A_long)((PF_FpLong)r.x / 65536 + 0.5);
			pos->y = (A_long)((PF_FpLong)r.y / 65536 + 0.5);
		}
		return err;

	}
	//--------------------------------------------------------------------
	PF_Err GetPOINT3D(A_long idx, Point3D* pos)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					pos->x = param.u.point3d_d.x_value;
					pos->y = param.u.point3d_d.y_value;
					pos->z = param.u.point3d_d.z_value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				pos->x = params[idx]->u.point3d_d.x_value;
				pos->y = params[idx]->u.point3d_d.y_value;
				pos->z = params[idx]->u.point3d_d.z_value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetPOPUP(A_long idx, A_long* pop)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret = 1;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
				//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM(
					in_data,
					idx,
					in_data->current_time, in_data->time_step, in_data->time_scale,
					&param));
				if (!err) {
					ret = param.u.pd.value;
				}
				else {
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.pd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		*pop = ret;
		return err;
	}
	//--------------------------------------------------------------------
#pragma endregion

#pragma region Iterate
	//******************************************************************************
	PF_Err iterate8(
		PF_EffectWorld *src,
		void* refcon,
		PF_Err(*pix_fn)(void* refcon, A_long x, A_long y, PF_Pixel* in, PF_Pixel* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->Iterate8Suite1()->iterate(in_data,
			0,				// progress base
			dst->height,	// progress final
			src,			// src 
			NULL,			// area - null for all pixels
			refcon,			// refcon - your custom data pointer
			pix_fn,			// pixel function pointer
			dst);			// dest

	}
	//*********************************************************************************
	PF_Err iterate16(
		PF_EffectWorld* src,
		void* refcon,
		PF_Err(*pix_fn)(void* refcon, A_long x, A_long y, PF_Pixel16* in, PF_Pixel16* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->Iterate16Suite1()->iterate(in_data,
			0,				// progress base
			dst->height,	// progress final
			src,			// src 
			NULL,			// area - null for all pixels
			refcon,			// refcon - your custom data pointer
			pix_fn,			// pixel function pointer
			dst);			// dest

	}
	//*********************************************************************************
	PF_Err iterate32(
		PF_EffectWorld* src,
		void* refcon,
		PF_Err(*pix_fn)(void* refcon, A_long x, A_long y, PF_PixelFloat* in, PF_PixelFloat* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->IterateFloatSuite1()->iterate(
			in_data,
			0,				// progress base
			dst->height,	// progress final
			src,			// src 
			NULL,			// area - null for all pixels
			refcon,			// refcon - your custom data pointer
			pix_fn,			// pixel function pointer
			dst);			// dest

	}
#pragma endregion

	//--------------------------------------------------------------------
	PF_Err Copy(PF_EffectWorld* src, PF_EffectWorld* dst)
	{
		PF_Err err = PF_Err_NONE;

		err = suitesP->WorldTransformSuite1()->copy_hq(in_data->effect_ref,	// This effect ref (unique id)
			src,						// Source
			dst,						// Dest
			NULL,						// Source rect - null for all pixels
			NULL);						// Dest rect - null for all pixels
		return err;
	}
	//--------------------------------------------------------------------
	PF_EffectWorld NewEffectWorld(A_long w, A_long h, PF_PixelFormat fmt)
	{
		PF_Err err = PF_Err_NONE;
		PF_EffectWorld ret;
		AEFX_CLR_STRUCT(ret);
		if (m_cmd == PF_Cmd_SMART_RENDER) {
			ERR(ws2P->PF_NewWorld(in_data->effect_ref, w, h, TRUE, fmt, &ret));
		}
		else 
		{
			PF_NewWorldFlags f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_NONE;
			if (fmt == PF_PixelFormat_ARGB64)
				f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_DEEP_PIXELS;

			ERR((*in_data->utils->new_world)(in_data->effect_ref, w, h, f, &ret));
		}
		
		return ret;
	}
};
//******************************************************************************

#endif
