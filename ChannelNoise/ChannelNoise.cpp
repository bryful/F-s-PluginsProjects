//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "ChannelNoise.h"


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
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	Str_RED_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_RED_NOISE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	Str_RED_OPT,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_RED_OPT
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	Str_GREEN_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_GREEN_NOISE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	Str_GREEN_OPT,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_GREEN_OPT
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	Str_BLUE_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_BLUE_NOISE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	Str_BLUE_OPT,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_BLUE_OPT
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
	/*
	try{
		CFsAE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err){
			PF_Boolean b = FALSE;
			ERR(ae.GetCHECKBOX(ID_NOISE_FRAME,&b));
			ERR(ae.UI_DISABLE(ID_NOISE_OFFSET, b));
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
		}
	}catch ( PF_Err & errP){
		err = errP;
	}
	*/
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

//-------------------------------------------------------------------------------------------------
inline A_long randValue(A_long v)
{
	return F_RAND2(-v,v);
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (infoP){
		A_long aV=0;

		if ( outP->alpha>0){
			//red
			if ((infoP->red_value>0)&&(infoP->red_opt>0)){
				aV = randValue(infoP->red_add);
				aV = (A_long)((PF_FpShort)aV * infoP->red_opt);
				if ( aV!=0) outP->red = RoundByteLong((A_long)outP->red + aV);
			}else{
				F_RAND();
			}
			//green
			if ((infoP->green_value>0)&&(infoP->green_opt>0)){
				aV = randValue(infoP->green_add);
				aV = (A_long)((PF_FpShort)aV * infoP->green_opt);
				if ( aV!=0) outP->green = RoundByteLong((A_long)outP->green + aV);
			}else{
				F_RAND();
			}
			//blue
			if ((infoP->blue_value>0)&&(infoP->blue_opt>0)){
				aV = randValue(infoP->blue_add);
				aV = (A_long)((PF_FpShort)aV * infoP->green_opt);
				if ( aV!=0) outP->blue = RoundByteLong((A_long)outP->blue + aV);
			}else{
				F_RAND();
			}
		}else{
			F_RAND();//乱数の数合わせ
			F_RAND();
			F_RAND();//乱数の数合わせ
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (infoP){
		A_long aV=0;

		if ( outP->alpha>0){
			//red
			if ((infoP->red_value>0)&&(infoP->red_opt>0)){
				aV = randValue(infoP->red_add);
				aV = (A_long)((PF_FpShort)aV * infoP->red_opt);
				if ( aV!=0) outP->red = RoundShort((A_long)outP->red + aV);
			}else{
				F_RAND();
			}
			//green
			if ((infoP->green_value>0)&&(infoP->green_opt>0)){
				aV = randValue(infoP->green_add);
				aV = (A_long)((PF_FpShort)aV * infoP->green_opt);
				if ( aV!=0) outP->green = RoundShort((A_long)outP->green + aV);
			}else{
				F_RAND();
			}
			//blue
			if ((infoP->blue_value>0)&&(infoP->blue_opt>0)){
				aV = randValue(infoP->blue_add);
				aV = (A_long)((PF_FpShort)aV * infoP->green_opt);
				if ( aV!=0) outP->blue = RoundShort((A_long)outP->blue + aV);
			}else{
				F_RAND();
			}
		}else{
			F_RAND();//乱数の数合わせ
			F_RAND();
			F_RAND();//乱数の数合わせ
		}
	}


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (infoP){
		PF_FpLong aV=0;

		if ( outP->alpha>0){
			//red
			if ((infoP->red_value>0)&&(infoP->red_opt>0)){
				aV = (PF_FpShort)(randValue(infoP->red_add)) /65536;
				aV = aV * infoP->red_opt;
				if ( aV!=0) outP->red = RoundFpShortDouble(outP->red + aV);
			}else{
				F_RAND();
			}
			//green
			if ((infoP->green_value>0)&&(infoP->green_opt>0)){
				aV = (PF_FpShort)(randValue(infoP->green_add)) /65536;
				aV = aV * infoP->green_opt;
				if ( aV!=0) outP->green = RoundFpShortDouble(outP->green + aV);
			}else{
				F_RAND();
			}
			//blue
			if ((infoP->blue_value>0)&&(infoP->blue_opt>0)){
				aV = (PF_FpShort)(randValue(infoP->blue_add)) /65536;
				aV = aV * infoP->blue_opt;
				if ( aV!=0) outP->blue = RoundFpShortDouble(outP->blue + aV);
			}else{
				F_RAND();
			}
		}else{
			F_RAND();//乱数の数合わせ
			F_RAND();
			F_RAND();//乱数の数合わせ
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------

PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_Fixed f;

	ERR(ae->GetFIXED(ID_RED_NOISE,&f));
	if ( !err) infoP->red_value = (PF_FpShort)f /(100L<<16);
	ERR(ae->GetFIXED(ID_RED_OPT,&f));
	if ( !err) infoP->red_opt = (PF_FpShort)f / (100L<<16);

	ERR(ae->GetFIXED(ID_GREEN_NOISE,&f));
	if ( !err) infoP->green_value = (PF_FpShort)f /(100L<<16);
	ERR(ae->GetFIXED(ID_GREEN_OPT,&f));
	if ( !err) infoP->green_opt = (PF_FpShort)f / (100L<<16);
	
	ERR(ae->GetFIXED(ID_BLUE_NOISE,&f));
	if ( !err) infoP->blue_value = (PF_FpShort)f /(100L<<16);
	ERR(ae->GetFIXED(ID_BLUE_OPT,&f));
	if ( !err) infoP->blue_opt = (PF_FpShort)f / (100L<<16);


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ERR(ae->CopyInToOut());
	if (  ( (infoP->green_value<=0)||(infoP->green_opt<=0))
		&&( (infoP->blue_value <=0)||(infoP->blue_opt <=0))
		&&( (infoP->red_value  <=0)||(infoP->red_opt  <=0)))
	{
		return err;
	}
	F_SRAND(ae->frame());
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		infoP->blue_add		= (A_long)(PF_MAX_CHAN16 * infoP->blue_value);
		infoP->red_add		= (A_long)(PF_MAX_CHAN16 * infoP->red_value);
		infoP->green_add	= (A_long)(PF_MAX_CHAN16 * infoP->green_value);
		ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		infoP->blue_add		= (A_long)(PF_MAX_CHAN16 * infoP->blue_value);
		infoP->red_add		= (A_long)(PF_MAX_CHAN16 * infoP->red_value);
		infoP->green_add	= (A_long)(PF_MAX_CHAN16 * infoP->green_value);
		ERR(ae->iterate16((refconType)infoP,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		infoP->blue_add		= (A_long)(PF_MAX_CHAN8 * infoP->blue_value);
		infoP->red_add		= (A_long)(PF_MAX_CHAN8 * infoP->red_value);
		infoP->green_add	= (A_long)(PF_MAX_CHAN8 * infoP->green_value);
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (
	PF_InData			*in_data,
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
