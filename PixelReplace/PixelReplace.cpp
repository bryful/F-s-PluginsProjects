//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "PixelReplace.h"
#include <stdio.h>
#include "PixelReplace_def.h"

//-------------------------------------------------------------------------------------------------
//About表示
static PF_Err About (	PF_InData		*in_data,
						PF_OutData		*out_data,
						PF_ParamDef		*params[],
						PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.About(in_data,out_data,params,output);
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetup (	PF_InData		*in_data,
							PF_OutData		*out_data,
							PF_ParamDef		*params[],
							PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetup(in_data,out_data,params,output);
	return err;
}
//-----------------------------------------------------------------------------
static PF_Err	
GlobalSetdown(
	PF_InData		*in_data)
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetdown(in_data);
	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetdown (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


//-------------------------------------------------------------------------------------------------
static PF_Err SequenceResetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


//-------------------------------------------------------------------------------------------------
static PF_Err
HandleChangedParam(
	PF_InData					*in_data,
	PF_OutData					*out_data,
	PF_ParamDef					*params[],
	PF_LayerDef					*outputP,
	PF_UserChangedParamExtra	*extraP)
{
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

	try{
		CFsAE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if((!err)&&(in_data->appl_id != 'PrMr')){
			//--------------------
			
			ERR(ae.GetNewEffectStreamAll());
			if (!err){
				A_Boolean hide_themB[PixelReplace_NUM_PARAMS];
				for ( A_long i=0; i<PixelReplace_NUM_PARAMS; i++) hide_themB[i] =TRUE;
				hide_themB[PixelReplace_POP] =FALSE;
				hide_themB[PixelReplace_LV] =FALSE;
				A_long count=0;
				ERR(ae.GetPOPUP(PixelReplace_POP,&count));
				count *= paramsOffset_num;
				for ( A_long i=1; i<=count; i++){
					hide_themB[i] =FALSE;
				}
				for ( A_long i=1; i<ID_NUM_PARAMS; i++){
					ERR(ae.SetDynamicStreamFlag(i,AEGP_DynStreamFlag_HIDDEN,hide_themB[i]));
					ae.params[i]->flags |= PF_ParamFlag_START_COLLAPSED;
				}
				
			}
			//--------------------
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
		}
	}catch ( PF_Err & errP){
		err = errP;
	}
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err
QueryDynamicFlags(	
	PF_InData		*in_data,	
	PF_OutData		*out_data,	
	PF_ParamDef		*params[],	
	void			*extra)	
{
	PF_Err 	err 	= PF_Err_NONE,
			err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	/*
	CFsAE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_NOISE_FRAME,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	*/
	return err;
}

//-----------------------------------------------------------------------------
static PF_Err ParamsSetup (	PF_InData		*in_data,
							PF_OutData		*out_data,
							PF_ParamDef		*params[],
							PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;
	
	char num[255] = {"\0"};
	A_long i;
	for (i=0; i< PR_PRM_COUNT; i++){ 
#ifdef AE_OS_WIN
		sprintf_s(num,"target%d",i+1);
#else
		sprintf(num,"target%d",i+1);
#endif
		//*************************************************
		AEFX_CLR_STRUCT(def);	
		def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//
		PF_ADD_TOPIC(num,
			PARAMS_IDX(i, paramsOffset_topic)
			);
		//-----------------
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(UI_ENABLED1,UI_ENABLED2,FALSE,0,
			PARAMS_IDX(i, paramsOffset_enabled)
			);
		//-----------------
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(	UI_SRC_COLOR, 
						pr_colorDef[i%COLOR_DEF_SIZE][0],	// Red
						pr_colorDef[i%COLOR_DEF_SIZE][1],	//Green
						pr_colorDef[i%COLOR_DEF_SIZE][2],	//Blue
						PARAMS_IDX(i, paramsOffset_s_color)
						);
		//-----------------
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(	UI_REP_COLOR, 
						pr_colorDef[i%COLOR_DEF_SIZE][3],	// Red
						pr_colorDef[i%COLOR_DEF_SIZE][4],	//Green
						pr_colorDef[i%COLOR_DEF_SIZE][5],	//Blue
						PARAMS_IDX(i, paramsOffset_d_color)
						);
		//-----------------
		AEFX_CLR_STRUCT(def);
		PF_ADD_PERCENT(	UI_REP_OPACITY, 100,
						PARAMS_IDX(i, paramsOffset_d_opacity)
						);
		//-----------------
		AEFX_CLR_STRUCT(def);
		PF_END_TOPIC(
						PARAMS_IDX(i, paramsOffset_topic_end)
						);
		//*************************************************
	}

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(UI_LV,	//Name
						0,							//VALID_MIN
						100,						//VALID_MAX
						0,							//SLIDER_MIN
						100,						//SLIDER_MAX
						0,							//CURVE_TOLERANCE
						1,							//DFLT
						1,							//PREC
						0,							//DISP
						0,							//WANT_PHASE
						PixelReplace_LV
						);


	//-----------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE			|
						PF_ParamFlag_CANNOT_TIME_VARY	|
						PF_ParamFlag_CANNOT_INTERP;
						
	//def.ui_flags	=	PF_PUI_STD_CONTROL_ONLY; 
	PF_ADD_POPUP(UI_POP1, 
				PR_PRM_COUNT,	//メニューの数
				PR_PRM_COUNT,	//デフォルト
				UI_POP2,
				PixelReplace_POP
				);
	out_data->num_params = PixelReplace_NUM_PARAMS;

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo8 *	niP		= reinterpret_cast<ParamInfo8*>(refcon);
					
	if (niP){
		for (A_long i=0; i<niP->count;i++){
			if ( compPix8Lv(*outP, niP->src[i],niP->Lv)) {
				outP->alpha	= niP->dst[i].alpha;
				outP->red	= niP->dst[i].red;
				outP->green	= niP->dst[i].green;
				outP->blue	= niP->dst[i].blue;
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo8 *	niP		= reinterpret_cast<ParamInfo8*>(refcon);
					
	if (niP){
		for (A_long i=0; i<niP->count;i++){
			if ( compPix16_8Lv(*outP, niP->src[i],niP->Lv)) {
				outP->alpha	= niP->dst16[i].alpha;
				outP->red	= niP->dst16[i].red;
				outP->green	= niP->dst16[i].green;
				outP->blue	= niP->dst16[i].blue;
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo8 *	niP		= reinterpret_cast<ParamInfo8*>(refcon);
					
	if (niP){
		for (A_long i=0; i<niP->count;i++){
			if ( compPix32_8Lv(*outP, niP->src[i],niP->Lv)) {
				outP->alpha	= niP->dst32[i].alpha;
				outP->red	= niP->dst32[i].red;
				outP->green	= niP->dst32[i].green;
				outP->blue	= niP->dst32[i].blue;
			}
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo8 *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetPOPUP(PixelReplace_POP,&infoP->dispCount));
	PF_FpLong d;
	ERR(ae->GetFLOAT(PixelReplace_LV,&d));
	if (!err) infoP->Lv = RoundByteFpLong(PF_MAX_CHAN8 * d/100);

	PF_Boolean flg;
	A_long count =0;
	for ( A_long i=0; i < infoP->dispCount;i++){
		ERR(ae->GetCHECKBOX(PARAMS_IDX(i, paramsOffset_enabled),&flg));
		if ( (!err)&&(flg == TRUE)){
			ERR(ae->GetCOLOR(PARAMS_IDX(i, paramsOffset_s_color),&infoP->src[count]));
			ERR(ae->GetCOLOR(PARAMS_IDX(i, paramsOffset_d_color),&infoP->dst[count]));
			PF_Fixed f;
			ERR(ae->GetFIXED(PARAMS_IDX(i, paramsOffset_d_opacity),&f));
			if (!err) {
				f /=100;	
				infoP->dst[count].alpha = RoundByteLong(PF_MAX_CHAN8 * f /(1L<<16) );
				infoP->dst16[count] =CONV8TO16(infoP->dst[count]);
				infoP->dst32[count] =CONV8TO32(infoP->dst[count]);
			}
			if (!err) count++;
		}
		if (err) break;
	}
	infoP->count = count;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo8 *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
		break;
	}
	return err;
}
//-----------------------------------------------------------------------------
static PF_Err 
Render ( 
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ERR(ae.CopyInToOut());
		ParamInfo8 info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;
}
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extra)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extra,sizeof(ParamInfo8),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo8 *infoP = reinterpret_cast<ParamInfo8*>(ae.LockPreRenderData());
		if (infoP){
			ae.SetHostPreRenderData();
			ERR(GetParams(&ae,infoP));
			ERR(ae.UnSetPreRenderData());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extra)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extra,ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){
		ParamInfo8 *infoP = reinterpret_cast<ParamInfo8*>(ae.LockPreRenderData());
		if (infoP){
			ERR(Exec(&ae,infoP));
			ERR2(ae.UnsetSmartRender());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif
//-----------------------------------------------------------------------------------
static PF_Err 
RespondtoAEGP ( 	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extra)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extra));

	return err;
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
DllExport	PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP)
{
	PF_Err		err = PF_Err_NONE;
	
	try
	{
		CFsAE ae;
		switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_data);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceResetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
					err = Render(in_data,out_data,params,output);
					break;
#if defined(SUPPORT_SMARTFX)
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_data,out_data,reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(	in_data,out_data,reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
#endif
			case PF_Cmd_COMPLETELY_GENERAL:
				err = RespondtoAEGP(in_data,out_data,params,output,extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;		
			case PF_Cmd_USER_CHANGED_PARAM:
				err = HandleChangedParam(	in_data,
											out_data,
											params,
											output, 
											reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_data,
										out_data,
										params,
										reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
