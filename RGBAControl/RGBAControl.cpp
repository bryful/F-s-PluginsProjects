//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------
#include "RGBAControl.h"


///-----------------------------------------------------------------------------------
static PF_Err 
About (	
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

//-----------------------------------------------------------------------------------
static PF_Err 
GlobalSetup (
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
static PF_Err ParamsSetup (PF_InData		*in_data,
					PF_OutData		*out_data,
					PF_ParamDef		*params[],
					PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_RED,	//パラメータの名前
					-100, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_RED_FIXED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_GREEN,	//パラメータの名前
					-100, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_GREEN_FIXED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_BLUE,	//パラメータの名前
					-100, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_BLUE_FIXED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_ALPHA,	//パラメータの名前
					-100, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_ALPHA_FIXED
					);


	//----------------------------------------------------------------
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
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		if ( niP->alpha ==0) {
			outP->alpha = inP->alpha;
		}else if ( niP->alpha <0){
			outP->alpha	= RoundByteFpShort((PF_FpShort)inP->alpha + (PF_FpShort)inP->alpha * niP->alpha);
		}else{
			outP->alpha = RoundByteFpShort( (PF_FpShort)inP->alpha + (PF_FpShort)(PF_MAX_CHAN8 - inP->alpha) * niP->alpha);
		}

		if ( niP->green ==0) {
			outP->green = inP->green;
		}else if ( niP->green <0){
			outP->green	= RoundByteFpShort((PF_FpShort)inP->green + (PF_FpShort)inP->green * niP->green);
		}else{
			outP->green = RoundByteFpShort( (PF_FpShort)inP->green + (PF_FpShort)(PF_MAX_CHAN8 - inP->green) * niP->green);
		}
		if ( niP->blue ==0) { outP->blue = inP->blue;}
		else if ( niP->blue <0){
			outP->blue	= RoundByteFpShort((PF_FpShort)inP->blue + (PF_FpShort)inP->blue * niP->blue);
		}else{
			outP->blue = RoundByteFpShort( (PF_FpShort)inP->blue + (PF_FpShort)(PF_MAX_CHAN8 - inP->blue) * niP->blue);
		}
		if ( niP->red ==0) { outP->red = inP->red;}
		else if ( niP->red <0){
			outP->red	= RoundByteFpShort((PF_FpShort)inP->red + (PF_FpShort)inP->red * niP->red);
		}else{
			outP->red = RoundByteFpShort( (PF_FpShort)inP->red + (PF_FpShort)(PF_MAX_CHAN8 - inP->red) * niP->red);
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		if ( niP->alpha ==0) {outP->alpha = inP->alpha;}
		else if ( niP->alpha <0){
			outP->alpha	= RoundShortFpLong((PF_FpShort)inP->alpha + (PF_FpShort)inP->alpha * niP->alpha);
		}else{
			outP->alpha = RoundShortFpLong( (PF_FpShort)inP->alpha + (PF_FpShort)(PF_MAX_CHAN16 - inP->alpha) * niP->alpha);
		}

		if ( niP->green ==0) { outP->green = inP->green;}
		else if ( niP->green <0){
			outP->green	= RoundShortFpLong((PF_FpShort)inP->green + (PF_FpShort)inP->green * niP->green);
		}else{
			outP->green = RoundShortFpLong( (PF_FpShort)inP->green + (PF_FpShort)(PF_MAX_CHAN16 - inP->green) * niP->green);
		}
		if ( niP->blue ==0) { outP->blue = inP->blue;}
		else if ( niP->blue <0){
			outP->blue	= RoundShortFpLong((PF_FpShort)inP->blue + (PF_FpShort)inP->blue * niP->blue);
		}else{
			outP->blue = RoundShortFpLong( (PF_FpShort)inP->blue + (PF_FpShort)(PF_MAX_CHAN16 - inP->blue) * niP->blue);
		}
		if ( niP->red ==0) { outP->red = inP->red;}
		else if ( niP->red <0){
			outP->red	= RoundShortFpLong((PF_FpShort)inP->red + (PF_FpShort)inP->red * niP->red);
		}else{
			outP->red = RoundShortFpLong( (PF_FpShort)inP->red + (PF_FpShort)(PF_MAX_CHAN16 - inP->red) * niP->red);
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		if ( niP->alpha ==0) {outP->alpha = inP->alpha;}
		else if ( niP->alpha <0){
			outP->alpha	= RoundFpShort((PF_FpShort)inP->alpha + (PF_FpShort)inP->alpha * niP->alpha);
		}else{
			outP->alpha = RoundFpShort( (PF_FpShort)inP->alpha + (PF_FpShort)((PF_FpShort)1 - inP->alpha) * niP->alpha);
		}

		if ( niP->green ==0) { outP->green = inP->green;}
		else if ( niP->green <0){
			outP->green	= RoundFpShort((PF_FpShort)inP->green + (PF_FpShort)inP->green * niP->green);
		}else{
			outP->green = RoundFpShort( (PF_FpShort)inP->green + (PF_FpShort)((PF_FpShort)1 - inP->green) * niP->green);
		}
		if ( niP->blue ==0) { outP->blue = inP->blue;}
		else if ( niP->blue <0){
			outP->blue	= RoundFpShort((PF_FpShort)inP->blue + (PF_FpShort)inP->blue * niP->blue);
		}else{
			outP->blue = RoundFpShort( (PF_FpShort)inP->blue + (PF_FpShort)((PF_FpShort)1 - inP->blue) * niP->blue);
		}
		if ( niP->red ==0) { outP->red = inP->red;}
		else if ( niP->red <0){
			outP->red	= RoundFpShort((PF_FpShort)inP->red + (PF_FpShort)inP->red * niP->red);
		}else{
			outP->red = RoundFpShort( (PF_FpShort)inP->red + (PF_FpShort)((PF_FpShort)1 - inP->red) * niP->red);
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_Fixed f;
	ERR(ae->GetFIXED(ID_ALPHA_FIXED, &f));
	if (!err) infoP->alpha = (PF_FpShort)f / (PF_FpShort)(100L<<16);
	ERR(ae->GetFIXED(ID_RED_FIXED, &f));
	if (!err) infoP->red = (PF_FpShort)f / (PF_FpShort)(100L<<16);
	ERR(ae->GetFIXED(ID_GREEN_FIXED, &f));
	if (!err) infoP->green = (PF_FpShort)f / (PF_FpShort)(100L<<16);
	ERR(ae->GetFIXED(ID_BLUE_FIXED, &f));
	if (!err) infoP->blue = (PF_FpShort)f / (PF_FpShort)(100L<<16);

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if ((infoP->alpha ==0)&&(infoP->red==0)&&(infoP->green ==0)&&(infoP->blue ==0)) {
		ERR(ae->CopyInToOut());
	}else{
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
	}
	return err;
}

///-------------------------------------------------------------------------------------------------
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
		ParamInfo info;
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

//-----------------------------------------------------------------------------------
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
