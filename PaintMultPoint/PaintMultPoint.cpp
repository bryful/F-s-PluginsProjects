//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------
#include "PaintMultPoint.h"


//-------------------------------------------------------------------------------------------------
// Aboutダイアログ
static PF_Err About (
	PF_InData		*in_data,
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
static PF_Err GlobalSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetup(in_data,out_data,params,output);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetdown(
	PF_InData	*in_data)
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
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Pixel	cols[POINT_COLOR_COUNT] ={
		{PF_MAX_CHAN8,PF_MAX_CHAN8,0,0},
		{PF_MAX_CHAN8,0,PF_MAX_CHAN8,0},
		{PF_MAX_CHAN8,0,0,PF_MAX_CHAN8},
		{PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,0},
		{PF_MAX_CHAN8,PF_MAX_CHAN8,0,PF_MAX_CHAN8},
		{PF_MAX_CHAN8,0,PF_MAX_CHAN8,PF_MAX_CHAN8},
		{PF_MAX_CHAN8,PF_MAX_CHAN8/2,0,0},
		{PF_MAX_CHAN8,0,PF_MAX_CHAN8/2,0}
	};

	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	char cap[255] = {"\0"};
	for (A_long i=0; i<POINT_COLOR_COUNT; i++){
		AEFX_CLR_STRUCT(def);	
		def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
		PF_SPRINTF(cap,STR_TOPIC,i);
		PF_ADD_TOPIC(cap, ID_UI_TOPIC(i));
		AEFX_CLR_STRUCT(def);
		PF_SPRINTF(cap,STR_EXECUTED_CB1,i);
		PF_ADD_CHECKBOX(cap,
						STR_EXECUTED_CB2,
						FALSE,
						0,
						ID_UI_CB(i)
						);
		AEFX_CLR_STRUCT(def);	
		PF_SPRINTF(cap,STR_POS,i);
		PF_ADD_POINT(cap,
					(5+10*i),// X
					(5+10*i),	// Y
					0,	// Flag
					ID_UI_POS(i)
					);
		AEFX_CLR_STRUCT(def);
		PF_SPRINTF(cap,STR_COLOR,i);
		PF_ADD_COLOR(	cap, 
						cols[i].red,	// Red
						cols[i].green,	//Green
						cols[i].blue,	//Blue
						ID_UI_COL(i)
						);
		AEFX_CLR_STRUCT(def);
		PF_SPRINTF(cap,STR_GUIDE_CB1,i);
		PF_ADD_CHECKBOX(cap,
						STR_EXECUTED_CB2,
						FALSE,
						0,
						ID_UI_GID(i)
						);

		AEFX_CLR_STRUCT(def);
		PF_END_TOPIC(ID_UI_TOPIC_END(i));
	}
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_GUIDE_ALL_CB1,
					STR_GUIDE_ALL_CB2,
					FALSE,
					0,
					ID_UI_GUIDE_VISIBLE
					);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
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
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	A_long idx =0;
	for (A_long i = 0 ;i <POINT_COLOR_COUNT; i++){
		PF_Boolean b;
		ERR(ae->GetCHECKBOX(ID_UI_CB(i),&b));
		if ((!err)&&(b==TRUE)){
			ERR(ae->GetFIXEDPOINT(ID_UI_POS(i),&infoP->prms[idx].pos));
			ERR(ae->GetCOLOR(ID_UI_COL(i),&infoP->prms[idx].color));
			ERR(ae->GetCHECKBOX(ID_UI_GID(i),&infoP->prms[idx].disp_guide));
			if (!err) idx++;
		}
		if (err) break;
	}
	infoP->Count = idx;
	ERR(ae->GetCHECKBOX(ID_UI_GUIDE_VISIBLE,&infoP->disp_guide_all));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	if (!err){
		if ( infoP->Count>0){
			for ( A_long i=0; i<infoP->Count; i++){
				ERR(ae->out->paint(infoP->prms[i].pos,infoP->prms[i].color));
			}
			if ( infoP->disp_guide_all == FALSE){
				for ( A_long i=0; i<infoP->Count; i++){
					if (infoP->prms[i].disp_guide == TRUE){
						PF_Pixel b;
						b.alpha = PF_MAX_CHAN8;
						b.blue	= PF_MAX_CHAN8 - infoP->prms[i].color.blue;
						b.green	= PF_MAX_CHAN8 - infoP->prms[i].color.green;
						b.red	= PF_MAX_CHAN8 - infoP->prms[i].color.red;
						ae->out->lineHeiht = 2;
						ae->out->XLine(0,ae->out->width(),infoP->prms[i].pos.y>>16,b);
						ae->out->YLine(infoP->prms[i].pos.x>>16,0,ae->out->height(),b);
					}

				}
			}
		}
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (
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
		ParamInfo info;
		ERR(GetParams(&ae,&info));
		
	}
	return err;
}
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extraP,sizeof(ParamInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
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
#endif
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extraP,ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){
		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
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
	void*			extraP)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extraP));

	return err;
}
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
