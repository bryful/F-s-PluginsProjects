//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2026
*/
//-----------------------------------------------------------------------------------

//FsAE
#pragma once
#ifndef NF_AE_H
#define NF_AE_H

//******************************************************************************

#include "AE_SDK.h"

#include "NF_Utils.h"
#include "NF_Version.h"


#include <vector>
enum
{
	FsAE_NONE = 0,
	FsAE_ABOUT,
	FsAE_GLOBAL_SETUP,
	FsAE_GLOBAL_SETDOWN,
	FsAE_PARAMS_SETUP,
	FsAE_SEQUENCE_SETUP,
	FsAE_SEQUENCE_SETDOWN,
	FsAE_SEQUENCE_RESETUP,
	FsAE_RENDER,
	FsAE_SMART_PR_ERENDER,
	FsAE_SMART_RENDER,
	FsAE_DO_DIALOG,
	FsAE_USER_CHANGED_PARAM,
	FsAE_QUERY_DYNAMIC_FLAGS

};
#define FsAE_ERR	PF_Err_BAD_CALLBACK_PARAM
//-----------------------------------------------------------------------------------
/*
	プラグインID獲得のための構造体
*/
typedef struct {
	PF_Boolean		initializedB;
	AEGP_PluginID	my_id;
} ae_global_data, *ae_global_dataP, **ae_global_dataH;
//-----------------------------------------------------------------------------------


//******************************************************************************
#define FS_SCRIPT_ALERT	"alert(%s);\r\n"

#define FS_ABOUT_DIALOG	"var  FsAbout = function()\r\n\
{\r\n\
	var strName = \"%s\";\r\n\
	var strVersion = \"ver %d.%d [%s]\";\r\n\
	var strDis = \"%s\";\r\n\
	var strMyName = \"https://github.com/bryful : bryful@gmail.com \";\r\n\
    var nanae = \"Nanae Furuhashi - My beloved daughter. May she rest in peace.\";\r\n\
	var winObj = new Window(\"dialog\", \"NF's Plugins\", [ 0,  0,  480, 180] );\r\n\
\
	var edFsName = winObj.add(\"edittext\", [  30,   10,   30+ 440,   10+  20], strName, { readonly:true, borderless:true });\r\n\
	var edFsVersion = winObj.add(\"edittext\", [  30,   40,   30+ 440,   40+ 20], strVersion, { readonly:true, borderless:true });\r\n\
	var edFsDis = winObj.add(\"edittext\", [  30,   70,   30+ 440,   70+  20], strDis, { readonly:true, borderless:true });\r\n\
	var edMyName = winObj.add(\"edittext\", [  30,  100,   30+ 440,  100+  20], strMyName, { readonly:true, borderless:true });\r\n\
    var stNana = winObj.add(\"statictext\", [  30,  130,   30+ 440,  130+  20], nanae, { readonly:true, borderless:true });\r\n\
	var btnOK = winObj.add(\"button\", [ 360,  140,  360+ 100,  140+  24], \"OK\" , { name:\"ok\" });\r\n\
	this.show = function()\r\n\
	{\r\n\
		winObj.center();\r\n\
		return winObj.show();\r\n\
	}\r\n\
}\r\n\
var dlg = new FsAbout;\r\n\
dlg.show();\r\n"

#define FsAE_ITEM_COUNT	256

//******************************************************************************
//-----------------------------------------------------------------------------

typedef struct {
	A_long			width;
	A_long			height;
	A_long			widthTrue;
	A_long			offsetWidth;
}WORLDInfo;
//******************************************************************************
class CFsAE{
private:
	
protected:
	PF_PixelFormat		m_format;
	A_long				m_frame;
	A_long				m_mode;
	A_long				m_paramsCount;
	PF_Err				m_resultErr;
	PF_Boolean			m_isGetEffectStream;

public:
	//--------------------------------------------------------------------
	PF_InData			*in_data;
	PF_OutData			*out_data;
	PF_ParamDef			*params[FsAE_ITEM_COUNT];

	PF_SmartRenderExtra	*SRextraP;
	PF_PreRenderExtra	*PRextraP;
	PF_WorldSuite2		*ws2P;

	PF_EffectWorld		*input;
	PF_EffectWorld		*output;
	
	WORLDInfo			inputInfo;
	WORLDInfo			outputInfo;

	PF_Handle			PreRenderH;
	AEGP_SuiteHandler	*suitesP;
	PF_Iterate8Suite1*	iterS;
	PF_Handle			ae_plugin_idH;
	ae_global_dataP		ae_plugin_idP;
	AEGP_StreamRefH		ae_item_streamH[FsAE_ITEM_COUNT];
	AEGP_EffectRefH		ae_effect_refH;

	//*********************************************************************************
	//*********************************************************************************
	void Init()
	{
		m_format			= PF_PixelFormat_INVALID;
		m_frame				= 0;
		m_mode				= FsAE_NONE;
		m_paramsCount		= 0;
		m_isGetEffectStream	= FALSE;
		m_resultErr			= PF_Err_NONE;

		AEFX_CLR_STRUCT(inputInfo);
		AEFX_CLR_STRUCT(outputInfo);


		CFsAE::in_data		= NULL;
		CFsAE::out_data		= NULL;
		CFsAE::input		= NULL;
		CFsAE::output		= NULL;

		CFsAE::PreRenderH	= NULL;
		CFsAE::PRextraP		= NULL;
		CFsAE::SRextraP		= NULL;
		CFsAE::ws2P			= NULL;
		CFsAE::suitesP		= NULL;
		CFsAE::ae_plugin_idH	= NULL;
		CFsAE::ae_plugin_idP	= NULL;

		CFsAE::ae_effect_refH	= NULL;
		for (A_long i=0; i<FsAE_ITEM_COUNT; i++) CFsAE::ae_item_streamH[i]=NULL;

	}
	//*********************************************************************************
	CFsAE()
	{
		Init();
	}
	//*********************************************************************************
	PF_Err GetFrame()
	{
		PF_Err err = PF_Err_NONE;
		m_frame = 0;
		//カレントフレームを求める画頭は０
		if ((in_data->current_time >= 0) && (in_data->time_step > 0)) {
			m_frame = (in_data->current_time / in_data->time_step);
		}
		else {
			err = PF_Err_BAD_CALLBACK_PARAM;
		}
		return err;
	}
	//*********************************************************************************
	PF_Err GetSuite(A_long infoSize=0)
	{
		PF_Err err = PF_Err_NONE;
		if (CFsAE::in_data != NULL) {
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		};
		CFsAE::suitesP = new AEGP_SuiteHandler(in_data->pica_basicP);
		
		if (in_data->global_data) {
			ae_plugin_idH = in_data->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_data->global_data));
		}
		if (out_data != NULL) {
			ERR(AEFX_AcquireSuite(in_data,
				out_data,
				kPFWorldSuite,
				kPFWorldSuiteVersion2,
				"Couldn't load suite.",
				(void**)&(CFsAE::ws2P)));
		}
		return err;
	}
	PF_Err GetPixelFormat()
	{
		PF_Err err = PF_Err_NONE;
		if (CFsAE::input == NULL) {
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		}
		if (CFsAE::ws2P != NULL) {
			err = CFsAE::ws2P->PF_GetPixelFormat(CFsAE::input, &m_format);
		}
		else {
			if(PF_WORLD_IS_DEEP(CFsAE::input) == TRUE) {
				m_format = PF_PixelFormat_ARGB64;
			}
			else {
				m_format = PF_PixelFormat_ARGB32;
			}
		}
		return err;
	}
	PF_Err GetWorldSize()
	{
		PF_Err err = PF_Err_NONE;

		inputInfo.width = CFsAE::input->width;
		inputInfo.height = CFsAE::input->height;
		outputInfo.width = CFsAE::output->width;
		outputInfo.height = CFsAE::output->height;
		switch (m_format)
		{
		case PF_PixelFormat_ARGB128:
			inputInfo.widthTrue = CFsAE::input->rowbytes / sizeof(PF_PixelFloat);
			outputInfo.widthTrue = CFsAE::output->rowbytes / sizeof(PF_PixelFloat);

			break;
		case PF_PixelFormat_ARGB64:
			inputInfo.widthTrue = CFsAE::input->rowbytes / sizeof(PF_Pixel16);
			outputInfo.widthTrue = CFsAE::output->rowbytes / sizeof(PF_Pixel16);
			break;
		case PF_PixelFormat_ARGB32:
			inputInfo.widthTrue = CFsAE::input->rowbytes / sizeof(PF_Pixel);
			outputInfo.widthTrue = CFsAE::output->rowbytes / sizeof(PF_Pixel);
			break;
		default:
			err = PF_Err_BAD_CALLBACK_PARAM;
			break;
		}
		inputInfo.offsetWidth = inputInfo.widthTrue - inputInfo.width;
		outputInfo.offsetWidth = outputInfo.widthTrue - outputInfo.width;
		return err;
	}
	//*********************************************************************************
	//Render
	//*********************************************************************************
	//--------------------------------
	CFsAE(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP,
		A_long			paramsCount
		)
	{
		m_resultErr = Render(in_dataP,out_dataP,paramsP,outputP,paramsCount);
	}
	//--------------------------------
	PF_Err Render(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP,
		A_long			paramsCount
		)
	{
		Init();
		PF_Err err = PF_Err_NONE;
		m_mode = FsAE_RENDER;
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)|| (paramsP ==NULL) || (outputP ==NULL)||(paramsCount<=0)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		CFsAE::in_data = in_dataP;
		CFsAE::out_data = out_dataP;
		CFsAE::output = outputP;
		if (paramsP != NULL) {
			CFsAE::input = &paramsP[0]->u.ld;
			for (A_long i = 0; i < paramsCount; i++) CFsAE::params[i] = paramsP[i];
			m_paramsCount = paramsCount;
		}

		ERR(GetFrame());
		ERR(GetSuite());
		ERR(GetPixelFormat());
		ERR(GetWorldSize());	

		m_resultErr		= err;

		return err;
	}
	//*********************************************************************************
	//PreRender
	//*********************************************************************************
	//--------------------------------
	CFsAE(
		PF_InData			*in_dataP,
		PF_OutData			*out_dataP,
		PF_PreRenderExtra	*extraP,
		A_long				infoSize,
		A_long				paramsCount
		)
	{
		m_resultErr = PreRender(in_dataP,out_dataP,extraP,infoSize,paramsCount);
	}
	//--------------------------------
	PF_Err PreRender(
		PF_InData			*in_dataP,
		PF_OutData			*out_dataP,
		PF_PreRenderExtra	*extraP,
		A_long				infoSize,
		A_long				paramsCount
		)
	{
		Init();
		PF_Err err = PF_Err_NONE;
		m_mode = FsAE_SMART_PR_ERENDER;
		if ((in_dataP ==NULL) || ( out_dataP ==NULL)|| (extraP ==NULL)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		CFsAE::in_data		= in_dataP;
		CFsAE::out_data		= out_dataP;
		CFsAE::PRextraP		= extraP;
		m_paramsCount		= paramsCount;
		
		err = GetFrame();
		err = GetSuite();

		if (infoSize > 0) {
			NewPreRenderData(infoSize);
			if (PreRenderH == NULL) {
				m_resultErr = PF_Err_OUT_OF_MEMORY;
				m_mode = FsAE_NONE;
				return m_resultErr;
			}
		}

		m_resultErr		= err;
		return err;
	}
	//*********************************************************************************
	//SmartRender
	//*********************************************************************************
	//--------------------------------
	CFsAE(
		PF_InData				*in_dataP,
		PF_OutData				*out_dataP,
		PF_SmartRenderExtra		*extraP,
		A_long					paramsCount
		)
	{
		m_resultErr = SmartRender(in_dataP, out_dataP, extraP, paramsCount);
	}
	//--------------------------------
	PF_Err SmartRender(
		PF_InData				*in_dataP,
		PF_OutData				*out_dataP,
		PF_SmartRenderExtra		*extraP,
		A_long					paramsCount
		)
	{
		Init();
		PF_Err	err		= PF_Err_NONE;
		m_mode = FsAE_SMART_RENDER;
		if ((in_dataP ==NULL) || ( out_dataP ==NULL)|| (extraP ==NULL)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		CFsAE::in_data = in_dataP;
		CFsAE::out_data = out_dataP;
		CFsAE::SRextraP = extraP;
		m_paramsCount = paramsCount;
		ERR((extraP->cb->checkout_layer_pixels(in_dataP->effect_ref, 0, &(CFsAE::input))));
		ERR(extraP->cb->checkout_output(in_dataP->effect_ref, &(CFsAE::output)));

		ERR(GetFrame());
		ERR(GetSuite());
		ERR(GetPixelFormat());
		ERR(GetWorldSize());

		m_resultErr = err;
		return err;
	}
	//*********************************************************************************
	//*********************************************************************************
	//その他の処理
	//*********************************************************************************
	PF_Err About
	(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP)
	{
		(void)paramsP;  // 未使用パラメータの警告を抑制
		(void)outputP;  // 未使用パラメータの警告を抑制
		PF_Err	err				= PF_Err_NONE;

		m_mode = FsAE_ABOUT;
		in_data = in_dataP;
		out_data = out_dataP;
		output = outputP;
		ERR(GetFrame());
		ERR(GetSuite());

		if (ae_plugin_idP!=NULL){
			
			A_char scriptCode[2048] = {'\0'}; 
			PF_SPRINTF(	scriptCode,FS_ABOUT_DIALOG,
				NF_NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				__DATE__,
				NF_DESCRIPTION);
			
			ERR(suitesP->UtilitySuite5()->AEGP_ExecuteScript(ae_plugin_idP->my_id, scriptCode, TRUE, NULL, NULL));

		}else{
			PF_SPRINTF(	out_dataP->return_msg, 
				"%s, v%d.%d (%s)\r%s",
				NF_NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				__DATE__,
				NF_DESCRIPTION);
		}
		m_resultErr = err;
		m_mode		= FsAE_ABOUT;
		return err;
	}
	//*********************************************************************************
	PF_Err GlobalSetup
	(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP)
	{
		PF_Err	err				= PF_Err_NONE;
		m_mode = FsAE_GLOBAL_SETUP;
		in_data = in_dataP;
		out_data = out_dataP;
		output = outputP;
		ERR(GetSuite());

		//NF_Target.hで定義
		out_data->my_version	=	NF_VERSION;
		out_data->out_flags		=	NF_OUT_FLAGS;
		out_data->out_flags2	=	NF_OUT_FLAGS2;

		/**********************************************************
		プラグインIDを獲得して、グローバルにアクセスできるように保存
		*/
		ae_plugin_idH	=	suitesP->HandleSuite1()->host_new_handle(sizeof(ae_global_data));
	
		if (ae_plugin_idH) {
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(suitesP->HandleSuite1()->host_lock_handle(ae_plugin_idH));
			if (ae_plugin_idP) {
				ae_plugin_idP->initializedB 	= TRUE;

				if (in_data->appl_id != 'PrMr') {
					ERR(suitesP->UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, NF_NAME, &ae_plugin_idP->my_id));
				}
				if (!err){
					out_data->global_data 	= ae_plugin_idH;
				}
			}
			suitesP->HandleSuite1()->host_unlock_handle(ae_plugin_idH);
		} else	{
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		}
		//**********************************************************

		return err;

	}
	//*********************************************************************************
	PF_Err GlobalSetdown(PF_InData	*in_dataP)
	{
		PF_Err	err				= PF_Err_NONE;
		m_mode = FsAE_GLOBAL_SETDOWN;
		in_data = in_dataP;
		ERR(GetSuite());
	
		//ae_plugin_idH
		if (in_data->global_data) {
			suitesP->HandleSuite1()->host_dispose_handle(in_data->global_data);
		}

		return err;
	}
	//*********************************************************************************
	PF_Err HandleChangedParam(
		PF_InData					*in_dataP,
		PF_OutData					*out_dataP,
		PF_ParamDef					*paramsP[],
		PF_LayerDef					*outputP,
		PF_UserChangedParamExtra	*extraP,
		A_long						paramsCount
		)
	{
		PF_Err	err				= PF_Err_NONE;
		m_mode = FsAE_USER_CHANGED_PARAM;

		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)||(paramsCount<=0)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		CFsAE::in_data		= in_dataP;
		CFsAE::out_data		= out_dataP;
		
		ERR(GetFrame());
		ERR(GetSuite());
		ERR(GetPixelFormat());
		
		if ( paramsP !=NULL){
			CFsAE::input		= &paramsP[0]->u.ld;
			for ( A_long i=0; i<paramsCount; i++) CFsAE::params[i] = paramsP[i];
			m_paramsCount		= paramsCount;
		}

		if ( ae_plugin_idP){
			err = suitesP->PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(ae_plugin_idP->my_id, in_dataP->effect_ref, &ae_effect_refH);
		}

		return err;
	}
	//*********************************************************************************
	PF_Err
	QueryDynamicFlags(	
		PF_InData		*in_dataP,	
		PF_OutData		*out_dataP,	
		PF_ParamDef		*paramsP[],	
		void			*extraP,
		A_long			paramsCount
		)	
	{
		PF_Err	err				= PF_Err_NONE;
		m_mode = FsAE_QUERY_DYNAMIC_FLAGS;
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)|| (paramsCount<=0)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		CFsAE::in_data = in_dataP;
		CFsAE::out_data = out_dataP;

		ERR(GetFrame());
		ERR(GetSuite());

		if ( paramsP !=NULL){
			CFsAE::input		= &paramsP[0]->u.ld;
			for ( A_long i=0; i<paramsCount; i++) CFsAE::params[i] = paramsP[i];
			m_paramsCount = paramsCount;
		}

		return err;

	}
	//*********************************************************************************
	//デストラクタ
	//*********************************************************************************
	~CFsAE(){
		PF_Err 	err 	= PF_Err_NONE,
				err2 	= PF_Err_NONE;
		if (ae_effect_refH!=NULL){
			suitesP->EffectSuite2()->AEGP_DisposeEffect(ae_effect_refH);
			ae_effect_refH=NULL;
		}
		if (m_paramsCount>0){
			if ( m_isGetEffectStream==TRUE){
				for ( A_long i=0; i<m_paramsCount; i++){
					if (ae_item_streamH[i]!=NULL){
						ERR2(suitesP->StreamSuite2()->AEGP_DisposeStream(ae_item_streamH[i]));
						ae_item_streamH[i]=NULL;
					}
				}
			}
		}


		if ( suitesP != NULL) {
			delete suitesP;
			suitesP = NULL;
		}

		if (CFsAE::ws2P != NULL){
			ERR2(AEFX_ReleaseSuite(	in_data,
									out_data,
									kPFWorldSuite, 
									kPFWorldSuiteVersion2, 
									"Couldn't release suite."));
		}
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	PF_Handle NewPreRenderData(A_long handleSize)
	{
		if (m_mode == FsAE_SMART_PR_ERENDER){
			PreRenderH = suitesP->HandleSuite1()->host_new_handle(handleSize);
			return PreRenderH;
		}else{
			return NULL;
		}
	}
	//--------------------------------------------------------------------
	void * LockPreRenderData()
	{
		if (m_mode == FsAE_SMART_PR_ERENDER){
			if ( PreRenderH != NULL){
				return suitesP->HandleSuite1()->host_lock_handle(PreRenderH);
			}
		}else if (m_mode == FsAE_SMART_RENDER){
			if ( SRextraP != NULL){
				return suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
		return NULL;
	}
	//--------------------------------------------------------------------
	void  UnlockPreRenderData()
	{
		if (m_mode == FsAE_SMART_PR_ERENDER){
			if ( PreRenderH != NULL){
				suitesP->HandleSuite1()->host_unlock_handle(PreRenderH);
			}
		}else if (m_mode == FsAE_SMART_RENDER){
			if ( SRextraP != NULL){
				suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
	}
	//--------------------------------------------------------------------
	PF_Boolean SetHostPreRenderData()
	{
		PF_Boolean ret = FALSE;
		if ( m_mode == FsAE_SMART_PR_ERENDER){
			if ( PreRenderH != NULL){
				PRextraP->output->pre_render_data = PreRenderH;
				ret = TRUE;
			}
		}
		return ret;
	}
	//--------------------------------------------------------------------
	PF_Err UnSetPreRenderData(PF_RenderRequest *req, PF_CheckoutResult *in_result)
	{
		PF_Err err = PF_Err_NONE;
		if ( m_mode == FsAE_SMART_PR_ERENDER){
			ERR(PRextraP->cb->checkout_layer(	in_data->effect_ref,
											0,
											0,
											req,
											in_data->current_time,
											in_data->time_step,
											in_data->time_scale,
											in_result));
			
			UnionLRect(&in_result->result_rect, 		&PRextraP->output->result_rect);
			UnionLRect(&in_result->max_result_rect, 	&PRextraP->output->max_result_rect);		
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err UnSetPreRenderData()
	{
		PF_RenderRequest req = PRextraP->input->output_request;
		PF_CheckoutResult in_result;
		return UnSetPreRenderData(&req,&in_result);
	}
	//--------------------------------------------------------------------
	PF_Err UnsetSmartRender()
	{
		return SRextraP->cb->checkin_layer_pixels(in_data->effect_ref, 0);
	}
	//--------------------------------------------------------------------
	PF_Err UI_DISABLE(A_long idx,PF_Boolean b)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx<0)||(idx>=m_paramsCount)||(params == NULL))
		{
			err = PF_Err_BAD_CALLBACK_PARAM;
		}else{
			if (b == TRUE){
				params[idx]->ui_flags	|= PF_PUI_DISABLED;
			}else{
				params[idx]->ui_flags	&= ~PF_PUI_DISABLED;
			}
#if defined(PF_AE110_PLUG_IN_VERSION)
			err =suitesP->ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#else
			err =suitesP->ParamUtilsSuite1()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#endif
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err UpdateParamUI(A_long idx)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx<0)||(idx>=m_paramsCount)||(params == NULL)||(in_data==NULL))
		{
			err = PF_Err_BAD_CALLBACK_PARAM;
		}else{
#if defined(PF_AE110_PLUG_IN_VERSION)
			err =suitesP->ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#else
			err =suitesP->ParamUtilsSuite1()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#endif
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetNewEffectStreamAll()
	{
		PF_Err err = PF_Err_NONE;
		for ( A_long i=0; i<FsAE_ITEM_COUNT; i++) ae_item_streamH[i] = NULL;
		if ( m_paramsCount>0){
			if ( (ae_plugin_idP)&&(ae_effect_refH)){
				for ( A_long i=1; i<m_paramsCount; i++){
					ERR(suitesP->StreamSuite2()->AEGP_GetNewEffectStreamByIndex(ae_plugin_idP->my_id, ae_effect_refH, i, &ae_item_streamH[i]));
				}
			}else{
				err = FsAE_ERR;
			}
		}
		if (!err) m_isGetEffectStream =TRUE;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err SetDynamicStreamFlag(A_long idx,AEGP_DynStreamFlags flg,A_Boolean setB)
	{
		PF_Err err = PF_Err_NONE;
		if ( (m_paramsCount>0)&&(m_isGetEffectStream ==TRUE)){
			if ( (idx>=1)&&(idx<m_paramsCount)){
				if ( (ae_plugin_idP)&&(ae_effect_refH)){
					err = suitesP->DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(ae_item_streamH[idx],flg,FALSE,setB);
				}else{
					err = FsAE_ERR;
				}
			}else{
				err = PF_Err_INVALID_INDEX;
			}
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err DisposeEffectStream()
	{
		PF_Err 	err 	= PF_Err_NONE,
				err2 	= PF_Err_NONE;

		for ( A_long i=0; i<FsAE_ITEM_COUNT; i++){
			if (ae_item_streamH[i]!=NULL){
				ERR2(suitesP->StreamSuite2()->AEGP_DisposeStream(ae_item_streamH[i]));
				ae_item_streamH[i]=NULL;
			}
		}
		if (ae_effect_refH!=NULL){
			ERR2(suitesP->EffectSuite2()->AEGP_DisposeEffect(ae_effect_refH));
			ae_effect_refH=NULL;
		}

		return err;
	}

	//--------------------------------------------------------------------
	//*********************************************************************************
	PF_Err checkout_param(A_long idx,PF_ParamDef *paramP)
	{
		PF_Err err = PF_Err_NONE;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			case FsAE_QUERY_DYNAMIC_FLAGS:
				//PF_ParamDef param;
				//AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					paramP));
				if (!err) {
					//*paramP = param;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
			err = PF_Err_INVALID_INDEX;
		}
		return err;
	}
	//*********************************************************************************
	PF_Err GetADD(A_long idx,A_long *a)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret =0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.sd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = params[idx]->u.sd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*a = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED(A_long idx,PF_Fixed *f)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.sd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = params[idx]->u.sd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*f = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED2FpShort(A_long idx,PF_FpShort *d)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		err = GetFIXED(idx,&ret);
		if (!err){
			*d = (PF_FpShort)ret/65536; 
		}else{
			err =FsAE_ERR;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED2FpShort_P1(A_long idx,PF_FpShort *d)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		err = GetFIXED(idx,&ret);
		if (!err){
			*d = (PF_FpShort)ret/6553600; 
		}else{
			err =FsAE_ERR;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED_PER2ONE(A_long idx,PF_Fixed *d)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		err = GetFIXED(idx,&ret);
		if (!err){
			*d = (PF_Fixed)ret/100; 
		}else{
			err =FsAE_ERR;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetANGLE(A_long idx,PF_Fixed *r)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.ad.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = params[idx]->u.ad.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*r = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetANGLE_FLT(A_long idx, PF_FpLong *r)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed rr;
		err = GetANGLE(idx, &rr);

		*r = (PF_FpLong)rr / 65536.0;

		return err;
	}	
	//--------------------------------------------------------------------
	PF_Err GetCHECKBOX(A_long idx,PF_Boolean *b)
	{
		PF_Err err = PF_Err_NONE;
		PF_Boolean ret =FALSE;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = (PF_Boolean)param.u.bd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = (PF_Boolean)params[idx]->u.bd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*b = ret;
		return err;
	}
	PF_Err SetCHECKBOX(A_long idx, PF_Boolean b)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_mode == FsAE_USER_CHANGED_PARAM)) {
			params[idx]->u.bd.value = b;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetCOLOR(A_long idx,PF_Pixel* c)
	{
		PF_Err err = PF_Err_NONE;
		PF_Pixel ret = {0,0,0,0};
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.cd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = params[idx]->u.cd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*c = ret;
		return err;


	}
	PF_Err SetCOLOR(A_long idx, PF_Pixel col)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_mode == FsAE_USER_CHANGED_PARAM))
		{
			params[idx]->u.cd.value.alpha = col.alpha;
			params[idx]->u.cd.value.red = col.red;
			params[idx]->u.cd.value.green = col.green;
			params[idx]->u.cd.value.blue = col.blue;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFLOAT(A_long idx,PF_FpLong *f)
	{
		PF_Err err = PF_Err_NONE;
		PF_FpLong ret = 0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.fs_d.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = params[idx]->u.fs_d.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*f = ret;
		return err;
	}
	//--------------------------------------------------------------------
	 PF_Err GetFIXEDPOINT(A_long idx,PF_FixedPoint *pos)
	{
		PF_Err err = PF_Err_NONE;
		PF_FixedPoint ret;
		ret.x =-1;
		ret.y =-1;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret.x = param.u.td.x_value;
					ret.y = param.u.td.y_value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret.x = CFsAE::params[idx]->u.td.x_value;
				ret.y = CFsAE::params[idx]->u.td.y_value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*pos = ret;
		return err;
	}

	//--------------------------------------------------------------------
	PF_Err GetPOPUP(A_long idx,A_long *pop)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret = 1;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case FsAE_SMART_PR_ERENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.pd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case FsAE_RENDER:
			case FsAE_USER_CHANGED_PARAM:
				ret = params[idx]->u.pd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*pop = ret;
		return err;
	}
	PF_Err SetPOPUP(A_long idx, A_long b)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_mode == FsAE_USER_CHANGED_PARAM))
		{
			params[idx]->u.pd.value = b;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		return err;
	}
	//*********************************************************************************
	PF_Err iterate8(
		PF_EffectWorld *src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out),
		PF_EffectWorld* dst
		)
	{
		return suitesP->Iterate8Suite1()->iterate(	in_data,
												0,														// progress base
												dst->height,	// progress final
												src,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												dst);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterate16(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* in, PF_Pixel16* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->Iterate16Suite1()->iterate(in_data,
			0,														// progress base
			dst->height,	// progress final
			src,		// src 
			NULL,		// area - null for all pixels
			refcon,		// refcon - your custom data pointer
			pix_fn,		// pixel function pointer
			dst);	// dest

	}
	//*********************************************************************************
	PF_Err iterate32(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat* in, PF_PixelFloat* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->IterateFloatSuite1()->iterate(
			in_data,
			0,														// progress base
			dst->height,	// progress final
			src,		// src 
			NULL,		// area - null for all pixels
			refcon,		// refcon - your custom data pointer
			pix_fn,		// pixel function pointer
			dst);	// dest

	}
	
	//*********************************************************************************
	PF_Err iterate8_origin(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out),
		PF_EffectWorld* dst
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->Iterate8Suite1()->iterate_origin(in_data,
															0,
															dst->height,
															src,
															&dst->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															dst);
		
	}
	//*********************************************************************************
	PF_Err iterate16_origin(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16 *in, PF_Pixel16 *out),
		PF_EffectWorld* dst
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->Iterate16Suite1()->iterate_origin(in_data,
															0,
															dst->height,
															src,
															&dst->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															dst);
		
	}
	//*********************************************************************************
	PF_Err iterate32_origin(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out),
		PF_EffectWorld* dst
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->IterateFloatSuite1()->iterate_origin(in_data,
															0,
															dst->height,
															src,
															&dst->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															dst);
		
	}
	//*********************************************************************************
	//*********************************************************************************
	PF_Err SetOutFlag_NON_PARAM_VARY(PF_Boolean b)
	{
		PF_Err err = PF_Err_NONE;
		if( out_data !=NULL){
			if (b==TRUE){
				out_data->out_flags |= PF_OutFlag_NON_PARAM_VARY;
			}else{
				out_data->out_flags &= ~PF_OutFlag_NON_PARAM_VARY;
			}
		}
		return err;
	}
	//*********************************************************************************
	PF_Err checkin_param(PF_ParamDef *defP)
	{
		return PF_CHECKIN_PARAM(in_data, defP);
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	PF_Boolean			is8Bit(){ return (m_format == PF_PixelFormat_ARGB32);}
	PF_Boolean			is16Bit(){ return (m_format == PF_PixelFormat_ARGB64);}
	PF_Boolean			is32Bit(){ return (m_format == PF_PixelFormat_ARGB128);}
	PF_PixelFormat		pixelFormat() { return m_format;}
	A_long				frame(){ return m_frame; }
	A_long				mode(){ return m_mode; }
	PF_Err				resultErr() { return m_resultErr;}
	//*********************************************************************************
	
	
	//*********************************************************************************
	
	//*********************************************************************************
	PF_Err NewWorld(A_long w, A_long h,PF_PixelFormat pf, PF_EffectWorld *wld)
	{
		PF_Err err = PF_Err_NONE;
		if (m_mode == FsAE_SMART_RENDER) {
			ERR(CFsAE::ws2P->PF_NewWorld(in_data->effect_ref, w, h, TRUE, pf, wld));

		}
		else {
			PF_NewWorldFlags f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_NONE;
			if (pf == PF_PixelFormat_ARGB64)
				f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_DEEP_PIXELS;
			ERR((*in_data->utils->new_world)(in_data->effect_ref, w, h, f, wld));
		}
		return err;
	}
	PF_Err DisposeWorld(PF_EffectWorld* wld)
	{
		PF_Err err = PF_Err_NONE;
		if (wld != NULL) {
			if (m_mode == FsAE_SMART_RENDER) {
				ERR(CFsAE::ws2P->PF_DisposeWorld(in_data->effect_ref, wld));
			}
			else {
				ERR((*in_data->utils->dispose_world)(in_data->effect_ref, wld));
			}
		}
		return err;
	}
	//*********************************************************************************
	PF_Handle NewHandle(A_long size)
	{
		//return suitesP->HandleSuite1()->host_new_handle(size);
		return PF_NEW_HANDLE(size);
	}
	//*********************************************************************************
	void LockHandle(PF_Handle h)
	{
		//suitesP->HandleSuite1()->host_lock_handle(h);
		PF_LOCK_HANDLE(h);
	}
	//*********************************************************************************
	void UnlockHandle(PF_Handle h)
	{
		//suitesP->HandleSuite1()->host_unlock_handle(h);
		PF_UNLOCK_HANDLE(h);
	}
	//*********************************************************************************
	void DisposeHandle(PF_Handle h)
	{
		//suitesP->HandleSuite1()->host_dispose_handle(h);
		PF_DISPOSE_HANDLE(h);
	}
	
	//*********************************************************************************
	int ShowDialog(const A_char * p)
	{
		return PF_SPRINTF(	out_data->return_msg, p );
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	PF_FpLong downScale(PF_FpLong v)
	{
		v = v * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den;
		return v;
	}
	//--------------------------------------------------------------------
	PF_FpLong downSaleValue()
	{
		return (PF_FpLong)in_data->downsample_x.den / (PF_FpLong)in_data->downsample_x.num;
	}
	
	//--------------------------------------------------------------------
	PF_Err CopyInToOut()
	{
		PF_Err err = PF_Err_NONE;
		err = suitesP->WorldTransformSuite1()->copy_hq(in_data->effect_ref,	// This effect ref (unique id)
												input,						// Source
												output,						// Dest
												NULL,						// Source rect - null for all pixels
												NULL);						// Dest rect - null for all pixels
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err Copy(PF_EffectWorld *i,PF_EffectWorld *o)
	{
		PF_Err err = PF_Err_NONE;

		err = suitesP->WorldTransformSuite1()->copy_hq(in_data->effect_ref,	// This effect ref (unique id)
												i,						// Source
												o,						// Dest
												NULL,						// Source rect - null for all pixels
												NULL);						// Dest rect - null for all pixels
		return err;
	}
	
	//--------------------------------------------------------------------
	PF_Err premultiply_color(A_long mode, PF_Pixel *color)
	{
#if defined(PF_AE100_PLUG_IN_VERSION)
	return suitesP->FillMatteSuite2()->premultiply_color(
			in_data->effect_ref,
			input,
			color,
			mode,
			output);

#else
	return suitesP->FillMatteSuite1()->premultiply_color(
			in_data->effect_ref,
			input,
			color,
			mode,
			output);
#endif
	}
	//--------------------------------------------------------------------
	PF_Err premultiply(A_long mode)
	{
#if defined(PF_AE100_PLUG_IN_VERSION)
	return suitesP->FillMatteSuite2()->premultiply(
			in_data->effect_ref,
			mode,
			output);
#else
	return suitesP->FillMatteSuite1()->premultiply(
			in_data->effect_ref,
			mode,
			output);
#endif
	}
	//--------------------------------------------------------------------

};



#endif
