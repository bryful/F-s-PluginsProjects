//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/

//-----------------------------------------------------------------------------------

#include "VideoLine.h"

PF_Err (*subFunc8)(refconType	refcon, A_long xL, A_long yL,PF_Pixel8	*inP, PF_Pixel8	*outP);
PF_Err (*subFunc16)(refconType	refcon, A_long xL, A_long yL,PF_Pixel16	*inP, PF_Pixel16	*outP);
PF_Err (*subFunc32)(refconType	refcon, A_long xL, A_long yL,PF_PixelFloat	*inP, PF_PixelFloat	*outP);

//-----------------------------------------------------------------------------------
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

	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_BRIGHT,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					200,			//スライダーの最大値
					75,				//デフォルトの値
					1,//数値表示に関するフラグ 
					0,
					0,
					ID_BRIGHT
					);

	//2個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_HEIGHT,	//パラメータの名前
					1, 			//数値入力する場合の最小値
					100,		//数値入力する場合の最大値
					1,			//スライダーの最小値 
					16,			//スライダーの最大値
					2,	//デフォルトの値
					ID_HEIGHT
					);


	//3個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_REV1,STR_REV2, FALSE,0, ID_REV);

	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_INTER,	//パラメータの名前
					-100, 		//数値入力する場合の最小値
					100,		//数値入力する場合の最大値
					0,			//スライダーの最小値 
					10,			//スライダーの最大値
					0,	//デフォルトの値
					ID_INTER
					);

	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_DIR, 
						2,	//メニューの数
						1,	//デフォルト
						STR_DIRSTR,
						ID_DIR
						);

	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(
					STR_OFFSET,
					0,
					ID_OFFSET); 

	out_data->num_params = ID_NUM_PARAMS;

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
		if((!err)&&(in_data->appl_id != 'PrMr')){
			//--------------------
			
			ERR(ae.GetNewEffectStreamAll());
			if (!err){
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) hide_themB[i] =FALSE;
				PF_Boolean b;
				ERR(ae.GetCHECKBOX(ID_HIDDEN_ON,&b));
				if (b){
					for ( A_long i=ID_HIDDEN_ON+1; i<ID_NUM_PARAMS; i++)hide_themB[i] =TRUE;
				}
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) 
					ERR(ae.SetDynamicStreamFlag(i,AEGP_DynStreamFlag_HIDDEN,hide_themB[i]));
				
				
			}
			//--------------------
			if (!err){
				PF_Boolean b = FALSE;
				ERR(ae.GetCHECKBOX(ID_NOISE_FRAME,&b));
				ERR(ae.UI_DISABLE(ID_NOISE_OFFSET, b));
			}
			//--------------------
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
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		A_long w = niP->height + niP->inter;
		PF_Fixed fy;
		if ( niP->vurFlag == TRUE){
			fy = (yL<<16)   - (PF_Fixed)(niP->offset*(1L<<16));
		}else {
			fy = (xL<<16)   - (PF_Fixed)(niP->offset*(1L<<16));
		}
		PF_Fixed y = fy % (w<<16);
		A_long ya = y >> 16;
		PF_FpShort yd = (PF_FpShort)(y & 0xFFFF)/65536;
		PF_FpShort brt = 0;


		if (niP->revFlag ==FALSE){
			if ( ya==0){
				brt = niP->bright * yd;
			}else if ( ya <niP->height){
				brt = niP->bright;
			}else if (ya ==niP->height){
				brt = niP->bright *((PF_FpShort)1.0 - yd);
			}
		}else{
			if ( ya==0){
				brt = niP->bright *((PF_FpShort)1.0 - yd);
			}else if ((ya ==niP->height)){
				brt = niP->bright * yd;
			}else if (ya >niP->height){
				brt = niP->bright;
			}
		}
		if ( brt>0){
			if ( niP->minus == TRUE){
				outP->blue	=   RoundByteFpLong((PF_FpShort)outP->blue   - outP->blue * brt); 
				outP->green	=   RoundByteFpLong((PF_FpShort)outP->green  - outP->green * brt); 
				outP->red	=   RoundByteFpLong((PF_FpShort)outP->red    - outP->red * brt); 
			}else{
				outP->blue	=   RoundByteFpLong((PF_FpShort)outP->blue   + (PF_MAX_CHAN8 - outP->blue) * brt); 
				outP->green	=   RoundByteFpLong((PF_FpShort)outP->green  + (PF_MAX_CHAN8 - outP->green) * brt); 
				outP->red	=   RoundByteFpLong((PF_FpShort)outP->red    + (PF_MAX_CHAN8 - outP->red) * brt); 
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
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		A_long w = niP->height + niP->inter;
		PF_Fixed fy;
		if ( niP->vurFlag == TRUE){
			fy = (yL<<16)   - (PF_Fixed)(niP->offset*(1L<<16));
		}else {
			fy = (xL<<16)   - (PF_Fixed)(niP->offset*(1L<<16));
		}
		PF_Fixed y = fy % (w<<16);
		A_long ya = y >> 16;
		PF_FpShort yd = (PF_FpShort)(y & 0xFFFF)/65536;
		PF_FpShort brt = 0;


		if (niP->revFlag ==FALSE){
			if ( ya==0){
				brt = niP->bright * yd;
			}else if ( ya <niP->height){
				brt = niP->bright;
			}else if (ya ==niP->height){
				brt = niP->bright *((PF_FpShort)1.0 - yd);
			}
		}else{
			if ( ya==0){
				brt = niP->bright *((PF_FpShort)1.0 - yd);
			}else if ((ya ==niP->height)){
				brt = niP->bright * yd;
			}else if (ya >niP->height){
				brt = niP->bright;
			}
		}
		if ( brt>0){
			if ( niP->minus == TRUE){
				outP->blue	=   RoundShortFpLong(outP->blue   - outP->blue * brt); 
				outP->green	=   RoundShortFpLong(outP->green  - outP->green * brt); 
				outP->red	=   RoundShortFpLong(outP->red    - outP->red * brt); 
			}else{
				outP->blue	=   RoundShortFpLong(outP->blue   + (PF_MAX_CHAN16 - outP->blue) * brt); 
				outP->green	=   RoundShortFpLong(outP->green  + (PF_MAX_CHAN16 - outP->green) * brt); 
				outP->red	=   RoundShortFpLong(outP->red    + (PF_MAX_CHAN16 - outP->red) * brt); 
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err 
FilterImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		A_long w = niP->height + niP->inter;
		PF_Fixed fy;
		if ( niP->vurFlag == TRUE){
			fy = (yL<<16)   - (PF_Fixed)(niP->offset*(1L<<16));
		}else {
			fy = (xL<<16)   - (PF_Fixed)(niP->offset*(1L<<16));
		}
		PF_Fixed y = fy % (w<<16);
		A_long ya = y >> 16;
		PF_FpShort yd = (PF_FpShort)(y & 0xFFFF)/65536;
		PF_FpShort brt = 0;


		if (niP->revFlag ==FALSE){
			if ( ya==0){
				brt = niP->bright * yd;
			}else if ( ya <niP->height){
				brt = niP->bright;
			}else if (ya ==niP->height){
				brt = niP->bright *((PF_FpShort)1.0 - yd);
			}
		}else{
			if ( ya==0){
				brt = niP->bright *((PF_FpShort)1.0 - yd);
			}else if ((ya ==niP->height)){
				brt = niP->bright * yd;
			}else if (ya >niP->height){
				brt = niP->bright;
			}
		}
		if ( brt>0){
			if ( niP->minus == TRUE){
				outP->blue	=   RoundFpShortDouble(outP->blue   - outP->blue * brt); 
				outP->green	=   RoundFpShortDouble(outP->green  - outP->green * brt); 
				outP->red	=   RoundFpShortDouble(outP->red    - outP->red * brt); 
			}else{
				outP->blue	=   RoundFpShortDouble(outP->blue   + ((PF_FpShort)1.0 - outP->blue) * brt); 
				outP->green	=   RoundFpShortDouble(outP->green  + ((PF_FpShort)1.0 - outP->green) * brt); 
				outP->red	=   RoundFpShortDouble(outP->red    + ((PF_FpShort)1.0 - outP->red) * brt); 
			}
		}
	}

	return err;
}

#endif
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_Fixed f;
	ERR(ae->GetFIXED(ID_BRIGHT,&f));
	if (!err){
		f = ( f-(100L<<16)) / 100;
		if ( f<0){
			infoP->minus = TRUE;
			f *= -1;
		}else{
			infoP->minus = FALSE;
		}
		infoP->bright =  (PF_FpShort)f / (PF_FpShort)(1L<<16);
	}
	ERR(ae->GetADD(ID_HEIGHT,&infoP->height));
	if(!err) infoP->height = ae->downScale(infoP->height);
	ERR(ae->GetCHECKBOX(ID_REV,&infoP->revFlag));
	ERR(ae->GetADD(ID_INTER,&infoP->inter));
	if(!err) {
		infoP->inter = ae->downScaleNoClip(infoP->inter);
		infoP->inter += infoP->height;
		if (infoP->inter<=0) infoP->inter = 1;
	}
	ERR(ae->GetCHECKBOX(ID_DIR,&infoP->vurFlag));

	ERR(ae->GetFIXED(ID_OFFSET,&f));
	if (!err){
		f = f % (360L<<16);
		infoP->offset = (PF_FpShort)f/65536;
		infoP->offset = infoP->offset/360; 
		if ( infoP->offset<0)infoP->offset = 1 + infoP->offset;
		infoP->offset = infoP->offset * (infoP->height + infoP->inter);
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if (infoP->bright!=0){
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

//-------------------------------------------------------------------------------------------------
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
											"RespondtoAEGP : %s",	
											reinterpret_cast<A_char*>(extraP));

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
