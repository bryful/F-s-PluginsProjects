#pragma once
#ifndef CAE_H
#define CAE_H

//******************************************************************************
//各種ヘッダー登録


#include "FsAEHeader.h"
#include "FsVersion.h"
#include "FsUtils.h"

#define FLOAT_PAR(x) do{x/=100;if(x<-1) x=1;else if(x>1) x=1;}while(0)


//******************************************************************************

//呼び出される関数の定数
enum
{
	AE_NONE = 0,
	AE_ABOUT,
	AE_GLOBAL_SETUP,
	AE_GLOBAL_SETDOWN,
	AE_PARAMS_SETUP,
	AE_SEQUENCE_SETUP,
	AE_SEQUENCE_SETDOWN,
	AE_SEQUENCE_RESETUP,
	AE_RENDER,
	AE_SMART_PR_ERENDER,
	AE_SMART_RENDER,
	AE_DO_DIALOG,
	AE_USER_CHANGED_PARAM,
	AE_QUERY_DYNAMIC_FLAGS,
	AE_FRAME_SETUP

};


#define AE_ERR	PF_Err_BAD_CALLBACK_PARAM
//-----------------------------------------------------------------------------------
/*
	プラグインID獲得のための構造体
*/
typedef struct {
	PF_Boolean		initializedB;
	AEGP_PluginID	my_id;
} ae_global_data, *ae_global_dataP, **ae_global_dataH;
//-----------------------------------------------------------------------------------


#define AE_ITEM_COUNT	256
#define AE_WORLD_COUNT	4
//******************************************************************************
class CAE{
private:
protected:
	PF_PixelFormat		m_format;
	A_long				m_mode;
	A_long				m_paramsCount;
	PF_Err				m_resultErr;
	PF_Boolean			m_isGetEffectStream;
	A_long				m_frame;

public:
	//--------------------------------------------------------------------
	PF_InData			*in_data;
	PF_OutData			*out_data;
	PF_ParamDef			*params[AE_ITEM_COUNT];

	PF_SmartRenderExtra	*SRextraP;
	PF_PreRenderExtra	*PRextraP;
	PF_WorldSuite2		*ws2P;

	PF_EffectWorld		*input;
	PF_EffectWorld		*output;
	
	PF_Handle			PreRenderH;
	AEGP_SuiteHandler	*suitesP;
	PF_Handle			ae_plugin_idH;
	ae_global_dataP		ae_plugin_idP;
	AEGP_StreamRefH		ae_item_streamH[AE_ITEM_COUNT];
	AEGP_EffectRefH		ae_effect_refH;

	//*********************************************************************************
	//*********************************************************************************
	void Init()
	{
		m_format			= PF_PixelFormat_INVALID;
		m_mode				= AE_NONE;
		m_paramsCount		= 0;
		m_isGetEffectStream	= FALSE;
		m_resultErr			= PF_Err_NONE;
		m_frame				= 0;

		CAE::in_data		= NULL;
		CAE::out_data		= NULL;
		CAE::input			= NULL;
		CAE::output			= NULL;

		CAE::PreRenderH		= NULL;
		CAE::PRextraP		= NULL;
		CAE::SRextraP		= NULL;
		CAE::ws2P			= NULL;
		CAE::suitesP		= NULL;
		CAE::ae_plugin_idH	= NULL;
		CAE::ae_plugin_idP	= NULL;

		CAE::ae_effect_refH	= NULL;
		for (A_long i=0; i<AE_ITEM_COUNT; i++) CAE::ae_item_streamH[i]=NULL;


	}
	//*********************************************************************************
	CAE()
	{
		Init();
	}
	//*********************************************************************************
	//Render
	//*********************************************************************************
	//--------------------------------
	CAE(
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
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)|| (paramsP ==NULL) || (outputP ==NULL)||(paramsCount<=0)){
			m_resultErr = AE_ERR;
			return m_resultErr;
		}

		if ( PF_WORLD_IS_DEEP(outputP) ==TRUE){
			m_format		= PF_PixelFormat_ARGB64;
		}else{
			m_format		= PF_PixelFormat_ARGB32;
		}
		//カレントフレームを求める画頭は０
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}
		CAE::in_data		= in_dataP;
		CAE::out_data		= out_dataP;
		
		if ( paramsP !=NULL){
			CAE::input		= &paramsP[0]->u.ld;
			for ( A_long i=0; i<paramsCount; i++) CAE::params[i] = paramsP[i];
			m_paramsCount		= paramsCount;
		}

		CAE::output		= outputP;

		CAE::suitesP	= new AEGP_SuiteHandler(in_dataP->pica_basicP);
		
		if (in_dataP->global_data){
			ae_plugin_idH	= in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}
		m_resultErr		= err;

		m_mode			= AE_RENDER;
		return err;
	}
	//*********************************************************************************
	//PreRender
	//*********************************************************************************
	//--------------------------------
	CAE(
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
		if ((in_dataP ==NULL) || ( out_dataP ==NULL)|| (extraP ==NULL)){
			m_resultErr = AE_ERR;
			return m_resultErr;
		}
		CAE::in_data		= in_dataP;
		CAE::out_data		= out_dataP;
		CAE::PRextraP		= extraP;
		m_paramsCount		= paramsCount;

		//カレントフレームを求める画頭は０
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}

		m_mode			= AE_SMART_PR_ERENDER;
		CAE::suitesP	= new AEGP_SuiteHandler(in_dataP->pica_basicP);
		if ( infoSize>0){
			NewPreRenderData(infoSize);
			if (PreRenderH==NULL){
				m_resultErr = PF_Err_OUT_OF_MEMORY;
				m_mode = AE_NONE;
				return m_resultErr;
			}
		}
		if (in_dataP->global_data){
			ae_plugin_idH	= in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}


		m_resultErr		= err;
		return err;
	}
	//*********************************************************************************
	//SmartRender
	//*********************************************************************************
	//--------------------------------
	CAE(
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
		if ((in_dataP ==NULL) || ( out_dataP ==NULL)|| (extraP ==NULL)){
			m_resultErr = AE_ERR;
			return m_resultErr;
		}

		//カレントフレームを求める画頭は０
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}

		CAE::in_data		= in_dataP;
		CAE::out_data		= out_dataP;
		CAE::SRextraP		= extraP;
		m_paramsCount		= paramsCount;

		CAE::suitesP	= new AEGP_SuiteHandler(in_dataP->pica_basicP);

		ERR((extraP->cb->checkout_layer_pixels(	in_dataP->effect_ref, 0, &(CAE::input))));
		ERR(extraP->cb->checkout_output(in_dataP->effect_ref, &(CAE::output)));
		ERR(AEFX_AcquireSuite(	in_dataP, 
								out_dataP, 
								kPFWorldSuite, 
								kPFWorldSuiteVersion2, 
								"Couldn't load suite.",
								(void**)&(CAE::ws2P)));
		ERR(CAE::ws2P->PF_GetPixelFormat(CAE::input, &m_format));
		if (err) {
			m_resultErr = PF_Err_BAD_CALLBACK_PARAM;
			return m_resultErr;
		}

		if (in_dataP->global_data){
			ae_plugin_idH	= in_dataP->global_data;
			ae_plugin_idP	= reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}


		m_resultErr = err;
		m_mode		= AE_SMART_RENDER;
		return err;
	}
	//*********************************************************************************
	//FrameSetup
	//*********************************************************************************
	//--------------------------------
	//*********************************************************************************
	PF_Err
		FrameSetup(
			PF_InData		*in_dataP,
			PF_OutData		*out_dataP,
			PF_ParamDef		*params[],
			PF_LayerDef		*output,
			A_long			paramsCount
		)
	{
		Init();
		PF_Err err = PF_Err_NONE;
		if ((in_dataP == NULL) || (out_dataP == NULL) ) {
			m_resultErr = AE_ERR;
			return m_resultErr;
		}
		CAE::in_data = in_dataP;
		CAE::out_data = out_dataP;
		m_paramsCount = paramsCount;

		//カレントフレームを求める画頭は０
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}

		m_mode = AE_FRAME_SETUP;
		CAE::suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
		
		if (in_dataP->global_data) {
			ae_plugin_idH = in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}


		m_resultErr = err;
		return err;
	}
	//*********************************************************************************
	//*********************************************************************************
	//その他の処理
	//*********************************************************************************
	PF_Err About
	(
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output)
	{
		PF_Err	err				= PF_Err_NONE;
		CAE::suitesP	= new AEGP_SuiteHandler(in_data->pica_basicP);
		if (in_data->global_data){
			ae_plugin_idH	= in_data->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_data->global_data));
		}
		//カレントフレームを求める画頭は０
		if ((in_data->current_time >= 0) && (in_data->time_step > 0)) {
			m_frame = (in_data->current_time / in_data->time_step);
		}
		if (ae_plugin_idP!=NULL){
			//スクリプトでダイアログ表示だけど使わない
			A_char scriptCode[1024*4] = {'\0'}; 
			PF_SPRINTF(	scriptCode,FS_ABOUT_STR,
				FS_NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				FS_DESCRIPTION,
				FS_CREATER,
				FS_CATEGORY
			);
			
			ERR(suitesP->UtilitySuite5()->AEGP_ExecuteScript(ae_plugin_idP->my_id, scriptCode, TRUE, NULL, NULL));
		}
		else {

			PF_SPRINTF(out_data->return_msg,
				"%s, v%d.%d\r%s",
				FS_NAME,
				MAJOR_VERSION,
				MINOR_VERSION,
				FS_DESCRIPTION);
		}
		m_resultErr = err;
		m_mode		= AE_ABOUT;
		return err;
	}
	//*********************************************************************************
	PF_Err GlobalSetup
	(
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output)
	{
		PF_Err	err				= PF_Err_NONE;
		CAE::suitesP			= new AEGP_SuiteHandler(in_data->pica_basicP);

		//***_Target.hで定義
		out_data->my_version	=	FS_VERSION;
		out_data->out_flags		=	FS_OUT_FLAGS;
		out_data->out_flags2	=	FS_OUT_FLAGS2;

		/**********************************************************
		プラグインIDを獲得して、グローバルにアクセスできるように保存
		*/
		ae_plugin_idH	=	suitesP->HandleSuite1()->host_new_handle(sizeof(ae_global_data));
	
		if (ae_plugin_idH) {
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(suitesP->HandleSuite1()->host_lock_handle(ae_plugin_idH));
			if (ae_plugin_idP) {
				ae_plugin_idP->initializedB 	= TRUE;

				if (in_data->appl_id != 'PrMr') {
					ERR(suitesP->UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, FS_NAME, &ae_plugin_idP->my_id));
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
		m_mode		= AE_GLOBAL_SETUP;

		return err;

	}
	//*********************************************************************************
	PF_Err GlobalSetdown(PF_InData	*in_data)
	{
		PF_Err	err				= PF_Err_NONE;
		CAE::suitesP	= new AEGP_SuiteHandler(in_data->pica_basicP);
	
		//ae_plugin_idH
		if (in_data->global_data) {
			suitesP->HandleSuite1()->host_dispose_handle(in_data->global_data);
		}
		m_mode		= AE_GLOBAL_SETDOWN;

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
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)||(paramsCount<=0)){
			m_resultErr = AE_ERR;
			return m_resultErr;
		}
		CAE::in_data		= in_dataP;
		CAE::out_data		= out_dataP;
		
		if ( outputP != NULL){
			if ( PF_WORLD_IS_DEEP(outputP) ==TRUE){
				m_format		= PF_PixelFormat_ARGB64;
			}else{
				m_format		= PF_PixelFormat_ARGB32;
			}
			CAE::output		= outputP;
		}
		//カレントフレームを求める画頭は０
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}
		if ( paramsP !=NULL){
			CAE::input		= &paramsP[0]->u.ld;
			for ( A_long i=0; i<paramsCount; i++) CAE::params[i] = paramsP[i];
			m_paramsCount		= paramsCount;
		}

		CAE::suitesP	= new AEGP_SuiteHandler(in_data->pica_basicP);
		if (in_dataP->global_data){
			ae_plugin_idH	= in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}
		if ( ae_plugin_idP){
			err = suitesP->PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(ae_plugin_idP->my_id, in_dataP->effect_ref, &ae_effect_refH);
		}
		if (!err)
			m_mode		= AE_USER_CHANGED_PARAM;

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
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)|| (paramsCount<=0)){
			m_resultErr = AE_ERR;
			return m_resultErr;
		}
		//カレントフレームを求める画頭は０
		if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
			m_frame = (in_dataP->current_time / in_dataP->time_step);
		}

		m_paramsCount		= paramsCount;

		CAE::in_data		= in_dataP;
		CAE::out_data		= out_dataP;

		if ( paramsP !=NULL){
			CAE::input		= &paramsP[0]->u.ld;
			for ( A_long i=0; i<paramsCount; i++) CAE::params[i] = paramsP[i];
		}
		CAE::suitesP	= new AEGP_SuiteHandler(in_data->pica_basicP);
		if (in_dataP->global_data){
			ae_plugin_idH	= in_dataP->global_data;
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(in_dataP->global_data));
		}
		m_mode		= AE_QUERY_DYNAMIC_FLAGS;

		return err;

	}
	//*********************************************************************************
	//デストラクタ
	//*********************************************************************************
	~CAE(){
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

		if (CAE::ws2P != NULL){
			PF_Err	err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;
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
		if (m_mode == AE_SMART_PR_ERENDER){
			PreRenderH = suitesP->HandleSuite1()->host_new_handle(handleSize);
			return PreRenderH;
		}else{
			return NULL;
		}
	}
	//--------------------------------------------------------------------
	void * LockPreRenderData()
	{
		if (m_mode == AE_SMART_PR_ERENDER){
			if ( PreRenderH != NULL){
				return suitesP->HandleSuite1()->host_lock_handle(PreRenderH);
			}
		}else if (m_mode == AE_SMART_RENDER){
			if ( SRextraP != NULL){
				return suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
		return NULL;
	}
	//--------------------------------------------------------------------
	void  UnlockPreRenderData()
	{
		if (m_mode == AE_SMART_PR_ERENDER){
			if ( PreRenderH != NULL){
				suitesP->HandleSuite1()->host_unlock_handle(PreRenderH);
			}
		}else if (m_mode == AE_SMART_RENDER){
			if ( SRextraP != NULL){
				suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
	}
	//--------------------------------------------------------------------
	PF_Boolean SetHostPreRenderData()
	{
		PF_Boolean ret = FALSE;
		if ( m_mode == AE_SMART_PR_ERENDER){
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
		if ( m_mode == AE_SMART_PR_ERENDER){
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
		for ( A_long i=0; i<AE_ITEM_COUNT; i++) ae_item_streamH[i] = NULL;
		if ( m_paramsCount>0){
			if ( (ae_plugin_idP)&&(ae_effect_refH)){
				for ( A_long i=1; i<m_paramsCount; i++){
					ERR(suitesP->StreamSuite2()->AEGP_GetNewEffectStreamByIndex(ae_plugin_idP->my_id, ae_effect_refH, i, &ae_item_streamH[i]));
				}
			}else{
				err = AE_ERR;
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
					err = AE_ERR;
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

		for ( A_long i=0; i<AE_ITEM_COUNT; i++){
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
			case AE_SMART_PR_ERENDER:
			case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_RENDER:
			case AE_FRAME_SETUP:
			case AE_USER_CHANGED_PARAM:
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
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_RENDER:
			case AE_FRAME_SETUP:
			case AE_USER_CHANGED_PARAM:
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
			err =AE_ERR;
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
			err =AE_ERR;
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
			err =AE_ERR;
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
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_RENDER:
			case AE_USER_CHANGED_PARAM:
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
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_FRAME_SETUP:
			case AE_RENDER:
			case AE_USER_CHANGED_PARAM:
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
	//--------------------------------------------------------------------
	PF_Err GetCOLOR(A_long idx,PF_Pixel* c)
	{
		PF_Err err = PF_Err_NONE;
		PF_Pixel ret = {0,0,0,0};
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_FRAME_SETUP:
			case AE_RENDER:
			case AE_USER_CHANGED_PARAM:
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
	//--------------------------------------------------------------------
	PF_Err GetFLOAT(A_long idx,PF_FpLong *f)
	{
		PF_Err err = PF_Err_NONE;
		PF_FpLong ret = 0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_mode)
			{
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_RENDER:
			case AE_USER_CHANGED_PARAM:
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
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_RENDER:
			case AE_FRAME_SETUP:
			case AE_USER_CHANGED_PARAM:
				ret.x = CAE::params[idx]->u.td.x_value;
				ret.y = CAE::params[idx]->u.td.y_value;
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
			case AE_SMART_PR_ERENDER:
			//case AE_QUERY_DYNAMIC_FLAGS:
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
			case AE_RENDER:
			case AE_FRAME_SETUP:
			case AE_USER_CHANGED_PARAM:
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

	//*********************************************************************************
	PF_Err iterate8(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out)
		)
	{
		return suitesP->Iterate8Suite1()->iterate(	in_data,
												0,														// progress base
												 output->height,	// progress final
												input,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												output);	// dest
		
	}
	//*********************************************************************************
	//*********************************************************************************
	PF_Err iterateEx8(
		refconType refcon,
		PF_EffectWorld	*i,
		PF_EffectWorld	*o,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out)
		)
	{
		return suitesP->Iterate8Suite1()->iterate(	in_data,
												0,														// progress base
												 o->height,	// progress final
												i,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												o);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterate8_origin(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out)
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->Iterate8Suite1()->iterate_origin(in_data,
															0,
															output->height,
															input,
															&output->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															output);
		
	}
	//*********************************************************************************
	PF_Err iterate16_origin(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16 *in, PF_Pixel16 *out)
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->Iterate16Suite1()->iterate_origin(in_data,
															0,
															output->height,
															input,
															&output->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															output);
		
	}
	//*********************************************************************************
	PF_Err iterate32_origin(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out)
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->IterateFloatSuite1()->iterate_origin(in_data,
															0,
															output->height,
															input,
															&output->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															output);
		
	}
	//*********************************************************************************
	PF_Err iterate16(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16 *in, PF_Pixel16 *out)
		)
	{
		return suitesP->Iterate16Suite1()->iterate(	in_data,
												0,														// progress base
												 output->height,	// progress final
												input,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												output);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterateEx16(
		refconType refcon,
		PF_EffectWorld	*i,
		PF_EffectWorld	*o,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16 *in, PF_Pixel16 *out)
		)
	{
		return suitesP->Iterate16Suite1()->iterate(	in_data,
												0,														// progress base
												o->height,	// progress final
												i,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												o);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterate32(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out)
		)
	{
		return suitesP->IterateFloatSuite1()->iterate(
												in_data,
												0,														// progress base
												 output->height,	// progress final
												input,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												output);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterate32(
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out),
		PF_EffectWorld *src,
		PF_EffectWorld *dst
		)
	{
		return suitesP->IterateFloatSuite1()->iterate(
												in_data,
												0,														// progress base
												 output->height,	// progress final
												src,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												dst);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterateEx32(
		refconType refcon,
		PF_EffectWorld	*i,
		PF_EffectWorld	*o,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out)
		)
	{
		return suitesP->IterateFloatSuite1()->iterate(	in_data,
												0,														// progress base
												o->height,	// progress final
												i,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												o);	// dest
		
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
	A_long				mode(){ return m_mode; }
	A_long				frame() { return m_frame; }
	PF_Err				resultErr() { return m_resultErr;}
	//*********************************************************************************
	PF_Handle NewHandle(A_long size)
	{
		return PF_NEW_HANDLE(size);
	}
	//*********************************************************************************
	void LockHandle(PF_Handle h)
	{
		PF_LOCK_HANDLE(h);
	}
	//*********************************************************************************
	void UnlockHandle(PF_Handle h)
	{
		PF_UNLOCK_HANDLE(h);
	}
	//*********************************************************************************
	void DisposeHandle(PF_Handle h)
	{
		PF_DISPOSE_HANDLE(h);
	}
	//*********************************************************************************
	int ShowDialog(const A_char * p)
	{
		return PF_SPRINTF(	out_data->return_msg, p );
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	A_long downScale(A_long v)
	{
		 v = v * in_data->downsample_x.num / in_data->downsample_x.den;
		 if (v==0) v =1;
		 return v;
	}
	//--------------------------------------------------------------------
	PF_FpLong downScaleNoClip(PF_FpLong v)
	{
		v = v * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den;
		return v;
	}
	//--------------------------------------------------------------------
	A_long downSaleValue()
	{
		return in_data->downsample_x.den / in_data->downsample_x.num;
	}
	//--------------------------------------------------------------------
	A_long downScaleNoClip(A_long v)
	{
		 return v * in_data->downsample_x.num / in_data->downsample_x.den;
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
	PF_Err premultiply(A_long mode)
	{
	return suitesP->FillMatteSuite2()->premultiply(
			in_data->effect_ref,
			mode,
			output);

	}
	//--------------------------------------------------------------------
	//*********************************************************************************

	PF_Err NewWorld(A_long w, A_long h, PF_PixelFormat format,PF_EffectWorldPtr world)
	{
		PF_Err err = PF_Err_NONE;


		if (m_mode == AE_SMART_RENDER) {
			ERR(CAE::ws2P->PF_NewWorld(in_data->effect_ref, w, h, TRUE, format, world));
		}
		else {
			PF_NewWorldFlags f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_NONE;
			if (format == PF_PixelFormat_ARGB64)
				f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_DEEP_PIXELS;

			ERR((*in_data->utils->new_world)(in_data->effect_ref, w, h, f, world));
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err NewWorld(PF_EffectWorldPtr world)
	{
		return NewWorld(output->width, output->height, m_format,world);
	}
	//--------------------------------------------------------------------
	PF_Err DisposeWorld(PF_EffectWorldPtr world)
	{
		PF_Err err = PF_Err_NONE;

		if (m_mode == AE_SMART_RENDER) {
			ERR(CAE::ws2P->PF_DisposeWorld(in_data->effect_ref, world));
		}
		else {
			ERR((*in_data->utils->dispose_world)(in_data->effect_ref, world));
		}
		return err;
	}
};

#endif
