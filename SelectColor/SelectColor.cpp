//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "SelectColor.h"

PF_Err (*subFunc8)(refconType	refcon, A_long xL, A_long yL,PF_Pixel8	*inP, PF_Pixel8	*outP);
PF_Err (*subFunc16)(refconType	refcon, A_long xL, A_long yL,PF_Pixel16	*inP, PF_Pixel16	*outP);
PF_Err (*subFunc32)(refconType	refcon, A_long xL, A_long yL,PF_PixelFloat	*inP, PF_PixelFloat	*outP);
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
static PF_Err ParamsSetup (	PF_InData		*in_data,
							PF_OutData		*out_data,
							PF_ParamDef		*params[],
							PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target0","実行する", FALSE,0, ID_TARGET0);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color0", 
					255,
					0,
					0,
					ID_SRCCOL0
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target1","実行する", FALSE,0, ID_TARGET1);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color1", 
					0,
					255,
					0,
					ID_SRCCOL1
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target2","実行する", FALSE,0, ID_TARGET2);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color2", 
					0,
					0,
					255,
					ID_SRCCOL2
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target3","実行する", FALSE,0, ID_TARGET3);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color3", 
					0,
					255,
					255,
					ID_SRCCOL3
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target4","実行する", FALSE,0, ID_TARGET4);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color4", 
					255,
					0,
					255,
					ID_SRCCOL4
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target5","実行する", FALSE,0, ID_TARGET5);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color5", 
					255,
					255,
					0,
					ID_SRCCOL5
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target6","実行する", FALSE,0, ID_TARGET6);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"color6", 
					255,
					255,
					255,
					ID_SRCCOL6
					);


	
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Target7","実行する", FALSE,0, ID_TARGET7);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	"元の色7", 
					0,
					0,
					0,
					ID_SRCCOL7
					);


	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("Rev","反転する", FALSE,0, ID_REV);

	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	"許容値",//パラメータの名前
					0, 				//数値入力する場合の最小値
					PF_MAX_CHAN8,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					15,			//スライダーの最大値
					0,				//デフォルトの値
					ID_LEVEL
					);

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
inline PF_Boolean ComparePix8(PF_Pixel8 *s, PF_Pixel8 *d,A_long lv)
{
	return ( (ABS(s->red - d->red)<=lv)&&(ABS(s->green - d->green)<=lv)&&(ABS(s->blue - d->blue)<=lv) );
}
//-----------------------------------------------------------------------------------
inline PF_Boolean ComparePix8Zero(PF_Pixel8 *s, PF_Pixel8 *d)
{
	return ( (s->red == d->red)&&(s->green == d->green)&&(s->blue == d->blue) );
}
//-----------------------------------------------------------------------------------
inline PF_Boolean ComparePix16(PF_Pixel16 *s, PF_Pixel16 *d,A_long lv)
{
	return ( (ABS(s->red - d->red)<=lv)&&(ABS(s->green - d->green)<=lv)&&(ABS(s->blue - d->blue)<=lv) );
}
//-----------------------------------------------------------------------------------
inline PF_Boolean ComparePix16Zero(PF_Pixel16 *s, PF_Pixel16 *d)
{
	return ( (s->red == d->red)&&(s->green == d->green)&&(s->blue == d->blue) );
}
//-----------------------------------------------------------------------------------
inline PF_Boolean ComparePix32(PF_PixelFloat *s, PF_PixelFloat *d,PF_FpShort lv)
{
	return ( (ABS(s->red - d->red)<=lv)&&(ABS(s->green - d->green)<=lv)&&(ABS(s->blue - d->blue)<=lv) );
}
//-----------------------------------------------------------------------------------
inline PF_Boolean ComparePix32Zero(PF_PixelFloat *s, PF_PixelFloat *d)
{
	return ( (s->red == d->red)&&(s->green == d->green)&&(s->blue == d->blue) );
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
		PF_Boolean b = FALSE;
		for (int i=0; i< niP->count;i++){
			PF_Pixel8 c = niP->src[i];
			if  ( ComparePix8(outP,&c,niP->lv)){
				b=TRUE;
				break;
			}
		}
		if ( b == niP->rev){
			outP->alpha = 0;
			outP->red = PF_MAX_CHAN8;
			outP->green = PF_MAX_CHAN8;
			outP->blue = PF_MAX_CHAN8;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Boolean b = FALSE;
		for (int i=0; i< niP->count;i++){
			PF_Pixel8 c = niP->src[i];
			if  ( ComparePix8Zero(outP,&c)){
				b=TRUE;
				break;
			}
		}
		if ( b == niP->rev){
			outP->alpha = 0;
			outP->red = PF_MAX_CHAN8;
			outP->green = PF_MAX_CHAN8;
			outP->blue = PF_MAX_CHAN8;
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
		PF_Boolean b = FALSE;
		PF_Pixel p = CONV16TO8(*outP);
		for (int i=0; i< niP->count;i++){
			PF_Pixel c = niP->src[i];
			if  ( ComparePix8(&p,&c,niP->lv)){
				b=TRUE;
				break;
			}
		}
		if ( b == niP->rev){
			outP->alpha = 0;
			outP->red = PF_MAX_CHAN16;
			outP->green = PF_MAX_CHAN16;
			outP->blue = PF_MAX_CHAN16;
		}

	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage16Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Boolean b = FALSE;
		PF_Pixel p = CONV16TO8(*outP);
		for (int i=0; i<niP->count;i++){
			PF_Pixel c = niP->src[i];
			if  ( ComparePix8Zero(&p,&c)){
				b = TRUE;
				break;
			}
		}
		if ( b == niP->rev){
			outP->alpha = 0;
			outP->red = PF_MAX_CHAN16;
			outP->green = PF_MAX_CHAN16;
			outP->blue = PF_MAX_CHAN16;
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
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Boolean b = FALSE;
		PF_Pixel p = CONV32TO8(*outP);
		for (int i=0; i<niP->count;i++){
			PF_Pixel c = niP->src[i];
			if  ( ComparePix8(&p,&c,niP->lv)){
				b = TRUE;
				break;
			}
		}
		if ( b == niP->rev){
			outP->alpha = 0;
			outP->red = 1.0;
			outP->green = 1.0;
			outP->blue = 1.0;
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage32Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Boolean b = FALSE;
		PF_Pixel p = CONV32TO8(*outP);
		for (int i=0; i<niP->count;i++){
			PF_Pixel c = niP->src[i];
			if  ( ComparePix8Zero(&p,&c)){
				b = TRUE;
				break;
			}
		}
		if ( b == niP->rev){
			outP->alpha = 0;
			outP->red = 1.0;
			outP->green = 1.0;
			outP->blue = 1.0;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	A_u_long idx =0;
	PF_Boolean b;
	PF_Pixel p;
	for ( A_long i=0; i<SelectColorCount; i++){
		ERR(ae->GetCHECKBOX(TARGET_POS(i),&b));
		if ( (!err)&&(b==TRUE)){
			ERR(ae->GetCOLOR(SRCCOL_POS(i),&p));
			if (!err){
				infoP->src[idx] = p;
				idx++;
			}
		}
	}
	if ( !err) infoP->count = idx;
	ERR(ae->GetCHECKBOX(ID_REV,&infoP->rev));
	ERR(ae->GetADD(ID_LEVEL,&infoP->lv));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if (infoP->count<=0)
	{
		return err;
	}

	if ( infoP->lv ==0){
		subFunc8 = FilterImage8Zero;
		subFunc16 = FilterImage16Zero;
		subFunc32 = FilterImage32Zero;
	}else{
		subFunc8 = FilterImage8;
		subFunc16 = FilterImage16;
		subFunc32 = FilterImage32;
	}
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,subFunc32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,subFunc16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,subFunc8));
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
	PF_PreRenderExtra	*extra)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extra,sizeof(ParamInfo),ID_NUM_PARAMS);
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
	PF_SmartRenderExtra		*extra)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extra,ID_NUM_PARAMS);
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
