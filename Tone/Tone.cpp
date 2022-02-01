//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "Tone.h"


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
static PF_Err ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_Highlights_COLOR, 
					Highlights_def_red,
					Highlights_def_green,
					Highlights_def_blue,
					ID_Highlights_COLOR
					);

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_Midtones_COLOR, 
					Midtones_def_red,
					Midtones_def_green,
					Midtones_def_blue,
					ID_Midtones_COLOR
					);

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_Shadows_COLOR, 
					Shadows_def_red,
					Shadows_def_green,
					Shadows_def_blue,
					ID_Shadows_COLOR
					);

	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_Midtones_OFFSET,//パラメータの名前
					-100, 				//数値入力する場合の最小値
					100,				//数値入力する場合の最大値
					-50,				//スライダーの最小値 
					50,					//スライダーの最大値
					Midtones_def_offset,//デフォルトの値
					1,					//数値表示に関するフラグ 
					0,
					0,
					ID_Midtones_OFFSET
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_Blend_w_Original_FIXED,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					Blend_w_Original_def,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_Blend_w_Original_FIXED
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

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static PF_PixelFloat colcColor(ToneParam *prm)
{
	PF_PixelFloat ret= {0,0,0,0};
	PF_FpShort l = prm->level;
	PF_FpShort b = prm->MidtonesBorder;

	if (l<=0) {
		if ( b ==0) ret = prm->Midtones;
		else ret = prm->Shadows;
	}else if (l>=1.0) {
		if ( b ==1) ret = prm->Midtones;
		else ret = prm->Highlights;
	}else if (l==b) {
		ret = prm->Midtones;
	}else{
		PF_FpShort v;
		if ( l<b){
			v = l/b;
			ret.blue	= prm->Shadows.blue + ( prm->Midtones.blue - prm->Shadows.blue) * v;
			ret.green	= prm->Shadows.green + ( prm->Midtones.green - prm->Shadows.green) * v;
			ret.red		= prm->Shadows.red + ( prm->Midtones.red - prm->Shadows.red) * v;
		}else{
			v = (PF_FpShort)((l- b)/(1.0 - b));
			ret.blue	= (PF_FpShort)(prm->Midtones.blue  + ( prm->Highlights.blue  - prm->Midtones.blue ) * v);
			ret.green	= (PF_FpShort)(prm->Midtones.green + ( prm->Highlights.green - prm->Midtones.green) * v);
			ret.red		= (PF_FpShort)(prm->Midtones.red   + ( prm->Highlights.red   - prm->Midtones.red  ) * v);
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err makeColorTbl8(ToneParam *infoP)
{

	PF_Err	err			= PF_Err_NONE;
	PF_Pixel *tbl;
	tbl = (PF_Pixel *)infoP->colorTbl;
	for ( A_long i=0; i<=PF_MAX_CHAN8; i++){
		infoP->level = (PF_FpShort)i / PF_MAX_CHAN8;
		tbl[i] = CONV32TO8( colcColor(infoP) );
	}
	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err makeColorTbl16(ToneParam *infoP)
{
	PF_Err	err			= PF_Err_NONE;
	PF_Pixel16 *tbl;
	tbl = (PF_Pixel16 *)infoP->colorTbl;
	for ( A_long i=0; i<=PF_MAX_CHAN16; i++){
		infoP->level = (PF_FpShort)i / PF_MAX_CHAN16;
		tbl[i] = CONV32TO16( colcColor(infoP) );
	}
	return err;
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
	ToneParam *	niP		= reinterpret_cast<ToneParam*>(refcon);

	PF_Pixel *tbl;
	tbl = (PF_Pixel *)niP->colorTbl;
	PF_Pixel col;
	if (!tbl){
		err = PF_Err_BAD_CALLBACK_PARAM;
	}else{
		A_long v =((outP->red*77)+(outP->green*151)+(outP->blue*28)) >>8;
		if (v<0) v=0;
		if (v>PF_MAX_CHAN8) v=PF_MAX_CHAN8;
		col = tbl[v];

		if ( niP->Blend_w_Original == 0){
			outP->red	= col.red;
			outP->green = col.green;
			outP->blue	= col.blue;
		}else{
			PF_FpShort s = (PF_FpShort)1.0 - niP->Blend_w_Original;
			outP->red	= RoundByteFpLong(((PF_FpShort)outP->red   * niP->Blend_w_Original) + ((PF_FpShort)col.red   * s));
			outP->green	= RoundByteFpLong(((PF_FpShort)outP->green * niP->Blend_w_Original) + ((PF_FpShort)col.green * s));
			outP->blue	= RoundByteFpLong(((PF_FpShort)outP->blue  * niP->Blend_w_Original) + ((PF_FpShort)col.blue  * s));
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ToneParam *	niP		= reinterpret_cast<ToneParam*>(refcon);

	PF_Pixel16 *tbl;
	tbl = (PF_Pixel16 *)niP->colorTbl;
	PF_Pixel16 col;
	if (!tbl){
		err = PF_Err_BAD_CALLBACK_PARAM;
	}else{
		PF_FpShort v =(((PF_FpShort)outP->red*77/256)+((PF_FpShort)outP->green*151/256)+((PF_FpShort)outP->blue*28/256));
		if (v<0) v=0;
		if (v>PF_MAX_CHAN16) v=PF_MAX_CHAN16;
		
		col = tbl[(A_long)v];

		if ( niP->Blend_w_Original == 0){
			outP->red	= col.red;
			outP->green = col.green;
			outP->blue	= col.blue;
		}else{
			PF_FpShort s = (PF_FpShort)1.0 - niP->Blend_w_Original;
			outP->red	= RoundShortFpLong(((PF_FpShort)outP->red   * niP->Blend_w_Original) + ((PF_FpShort)col.red   * s));
			outP->green	= RoundShortFpLong(((PF_FpShort)outP->green * niP->Blend_w_Original) + ((PF_FpShort)col.green * s));
			outP->blue	= RoundShortFpLong(((PF_FpShort)outP->blue  * niP->Blend_w_Original) + ((PF_FpShort)col.blue  * s));
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ToneParam *	niP		= reinterpret_cast<ToneParam*>(refcon);
	PF_FpShort g;


	g =(PF_FpShort)((outP->red*77/256)+(outP->green*151/256)+(outP->blue*28/256));
	if (g<0) g=0;
	else if (g>1.0) g=1.0;

	PF_PixelFloat col;
	ToneParam tp;

	tp.Highlights = niP->Highlights;
	tp.Midtones = niP->Midtones;
	tp.Shadows = niP->Shadows;
	tp.MidtonesBorder = niP->MidtonesBorder;
	tp.level = g;
	col= colcColor(&tp);

	if ( niP->Blend_w_Original == 0){
		outP->red	= col.red;
		outP->green = col.green;
		outP->blue	= col.blue;
	}else{
		PF_FpShort s = (PF_FpShort)1.0 - niP->Blend_w_Original;
		outP->red	= RoundFpShort((outP->red   * niP->Blend_w_Original) + (col.red   * s));
		outP->green	= RoundFpShort((outP->green * niP->Blend_w_Original) + (col.green * s));
		outP->blue	= RoundFpShort((outP->blue  * niP->Blend_w_Original) + (col.blue  * s));
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
GetParams (CFsAE *ae, ToneParam *infoP)
{
	PF_Err	err = PF_Err_NONE;

	PF_Pixel p;
	ERR( ae->GetCOLOR(ID_Highlights_COLOR,&p));
	if ( !err) infoP->Highlights = CONV8TO32(p);
	ERR( ae->GetCOLOR(ID_Midtones_COLOR,&p));
	if ( !err) infoP->Midtones = CONV8TO32(p);
	ERR( ae->GetCOLOR(ID_Shadows_COLOR,&p));
	if ( !err) infoP->Shadows = CONV8TO32(p);
	PF_Fixed f;
	ERR( ae->GetFIXED(ID_Midtones_OFFSET,&f));
	if ( !err) { 
		PF_FpShort s;
		s = (PF_FpShort)f / (100L<<16);
		if ( s<-1) s = -1; else if (s>1) s =1;
		infoP->MidtonesBorder = (PF_FpShort)(0.5 * (s+1.0));
	}
	ERR( ae->GetFIXED(ID_Blend_w_Original_FIXED,&f));
	if ( !err) infoP->Blend_w_Original = (PF_FpShort)f / (100L<<16);
	infoP->level = 0;
	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err 
Exec (CFsAE *ae , ToneParam *tp)
{
	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	if (tp->Blend_w_Original < 1.0) {
		switch(ae->pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			ERR(ae->iterate32((refconType)tp,FilterImage32));
			break;
		case PF_PixelFormat_ARGB64:
			pixelTable = ae->NewHandle(COLOR_TABLE_SIZE16);
			if(pixelTable){
				tp->colorTbl = *(PF_PixelPtr*)pixelTable;
				makeColorTbl16(tp);
				ERR(ae->iterate16((refconType)tp,FilterImage16));
				ae->DisposeHandle(pixelTable);
			}

			break;
		case PF_PixelFormat_ARGB32:
			pixelTable = ae->NewHandle(COLOR_TABLE_SIZE8);
			if(pixelTable){
				tp->colorTbl = *(PF_PixelPtr*)pixelTable;
				makeColorTbl8(tp);
				ERR(ae->iterate8((refconType)tp,FilterImage8));
				ae->DisposeHandle(pixelTable);
			}
			break;
		}
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
		ToneParam info;
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
	CFsAE ae(in_data,out_data,extraP,sizeof(ToneParam),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ToneParam *infoP = reinterpret_cast<ToneParam*>(ae.LockPreRenderData());
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
		ToneParam *infoP = reinterpret_cast<ToneParam*>(ae.LockPreRenderData());
		if (infoP){
			ERR(ae.CopyInToOut());
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
