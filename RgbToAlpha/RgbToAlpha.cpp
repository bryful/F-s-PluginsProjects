//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "RgbToAlpha.h"


static PF_Pixel COL_W = {0xFF,0xFF,0xFF,0xFF};
static PF_Pixel COL_BL = {0xFF,0,0,0};

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
static PF_Err ParamsSetup (PF_InData		*in_data,
					PF_OutData		*out_data,
					PF_ParamDef		*params[],
					PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	AEFX_CLR_STRUCT(def);	
	def.flags		=	PF_ParamFlag_SUPERVISE			|
						PF_ParamFlag_CANNOT_INTERP;
						
	PF_ADD_POPUP(		STR_POP1, 
						STR_POP,	//メニューの数
						STR_POP_DEF,	//デフォルト
						STR_POP2,
						ID_POP
						);

	AEFX_CLR_STRUCT(def);
	//def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_COLOR(	STR_COLOR, 
					0xFF,
					0x00,
					0x00,
					ID_COLOR
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_REV1,
					STR_REV2,
					FALSE,
					0,
					ID_CB
					);
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-----------------------------------------------------------------------------------
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
		/*
		CFsAE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err){
			A_long pop=0;
			ERR(ae.GetPOPUP(ID_POP,&pop));
			ERR(ae.UI_DISABLE(ID_COLOR,(pop!=3) ));
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
		}
		*/
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

//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo8 *	niP		= reinterpret_cast<ParamInfo8*>(refcon);
					
	if (niP){
		A_long v = ((inP->red * 77) + (inP->green * 151) + (inP->blue *28) )*inP->alpha / (256 *PF_MAX_CHAN8);
		outP->blue	= niP->col.blue;
		outP->green	= niP->col.green;
		outP->red	= niP->col.red;
		if ( niP->rev){
			outP->alpha = RoundByteFpLong(v) ^ PF_MAX_CHAN8;
		}else{
			outP->alpha = RoundByteFpLong(v);
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
	
	ParamInfo16 *	niP		= reinterpret_cast<ParamInfo16*>(refcon);
					
	if (niP){
		A_long v = ((inP->red * 77) + (inP->green * 151) + (inP->blue *28) ) / 256;
		v = v *inP->alpha / PF_MAX_CHAN16;
		outP->blue	= niP->col.blue;
		outP->green	= niP->col.green;
		outP->red	= niP->col.red;
		if ( niP->rev){
			outP->alpha =  PF_MAX_CHAN16 - RoundShort(v);
		}else{
			outP->alpha = RoundShort(v);
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
	
	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);
					
	if (niP){
		PF_FpShort v = ((inP->red * 77/256) + (inP->green * 151/256) + (inP->blue *28/256) ) * inP->alpha;
		outP->blue	= niP->col.blue;
		outP->green	= niP->col.green;
		outP->red	= niP->col.red;
		if ( niP->rev){
			outP->alpha = (PF_FpShort)1.0 -RoundFpShort(v);
		}else{
			outP->alpha = RoundFpShort(v);
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static  ParamInfo16 to16(ParamInfo8 *prm8)
{
	ParamInfo16 prm16;
	prm16.col = CONV8TO16( prm8->col);
	prm16.rev = prm8->rev;
	return prm16;
}
//-------------------------------------------------------------------------------------------------
static  ParamInfo32 to32(ParamInfo8 *prm8)
{
	ParamInfo32 prm32;
	prm32.col = CONV8TO32( prm8->col);
	prm32.rev = prm8->rev;
	return prm32;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo8 *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	A_long p =0;
	ERR(ae->GetPOPUP(ID_POP,&p));
	switch(p)
	{
	case 1:
		infoP->col = COL_W;
		break;
	case 2:
		infoP->col = COL_BL;
		break;
	case 3:
	default:
		ERR(ae->GetCOLOR(ID_COLOR,&infoP->col));
		break;
	}
	ERR(ae->GetCHECKBOX(ID_CB,&infoP->rev));

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
		ParamInfo32 info32;
		info32 = to32(infoP);
		ERR(ae->iterate32((refconType)&info32,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		ParamInfo16 info16;
		info16 = to16(infoP);
		ERR(ae->iterate16((refconType)&info16,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
		break;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
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

