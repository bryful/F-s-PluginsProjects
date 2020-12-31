//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "grayToColorize.h"

static PF_Err (*func8)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_Pixel8	*inP, 
			PF_Pixel8	*outP);

static PF_Err (*func16)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_Pixel16	*inP, 
			PF_Pixel16	*outP);

static PF_Err (*func32)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_PixelFloat 	*inP, 
			PF_PixelFloat	*outP);
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
	PF_ADD_FLOAT_SLIDER(STR_ALPHA_THRESHOLD,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						25,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_ALPHA_THRESHOLD
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COL1, 
					0xFF,
					0xF9,
					0x9C,
					ID_COL1
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COL2, //F5DD6D
					0xF5,
					0xDD,
					0x6D,
					ID_COL2
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COL3, //F3A447
					0xF3,
					0xA4,
					0x47,
					ID_COL3
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COL4, //DB9039
					0xDB,
					0x90,
					0x38,
					ID_COL4
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COL5, //F3C186
					0xF3, 
					0xC1,
					0x86,
					ID_COL5
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_CHECKBOX(STR_MAT_CB,
					STR_ON,
					FALSE,
					0,
					ID_MAT_CB
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_MAT_COL, 
					0xFF,
					0xFF,
					0xFF,
					ID_MAT_COLOR
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
	PF_Err				err					= PF_Err_NONE;

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
	PF_Err 	err 	= PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static A_long sampleCompare(PF_Pixel s,PF_Pixel d)
{
	double b = sqrt( pow((double)s.red,2)+pow((double)s.green,2)+pow((double)s.blue,2));
	return (A_long)((100.0 * b) + 0.5);
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
exec8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel c = *inP;
	//******************
	//Matカラー
	if (niP->mat_on){
		PF_Pixel m = niP->mat_color;
		c.red	= RoundByteLong((A_long)c.alpha*((A_long)c.red   -  (A_long)m.red)/PF_MAX_CHAN8 + (A_long)m.red);
		c.green = RoundByteLong((A_long)c.alpha*((A_long)c.green -  (A_long)m.green)/PF_MAX_CHAN8 + (A_long)m.green);
		c.blue	= RoundByteLong((A_long)c.alpha*((A_long)c.blue -   (A_long)m.blue)/PF_MAX_CHAN8 + (A_long)m.blue);
	}
	//******************
	//アルファー
	A_u_char a = 0;
	if (c.alpha > niP->alpha_border){
		a = PF_MAX_CHAN8;
	}else{
		a = 0;
	}
	//******************
	A_long v = (A_long)( ( 0.29891 * (PF_FpLong)c.red) + ( 0.58661 * (PF_FpLong)c.green) + ( 0.11448 * (PF_FpLong)c.blue)+ 0.5);
	v = FROM8_SAMPLE(v);
	if (v<0) v = 0;
	else if ( v>SAMPLE_MAX) v = SAMPLE_MAX;

	v = SAMPLE_MAX - v;
	*outP = niP->col[v];
	outP->alpha = a;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
exec16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP		= reinterpret_cast<ParamInfo16*>(refcon);

	PF_Pixel16 c = *inP;
	//******************
	//Matカラー
	if (niP->mat_on){
		PF_Pixel16 m = niP->mat_color;
		c.red	= RoundByteLong((A_long)c.alpha*((A_long)c.red   -  (A_long)m.red)/PF_MAX_CHAN16 + (A_long)m.red);
		c.green = RoundByteLong((A_long)c.alpha*((A_long)c.green -  (A_long)m.green)/PF_MAX_CHAN16 + (A_long)m.green);
		c.blue	= RoundByteLong((A_long)c.alpha*((A_long)c.blue -   (A_long)m.blue)/PF_MAX_CHAN16 + (A_long)m.blue);
	}
	//******************
	//アルファー
	A_u_short a = 0;
	if (c.alpha > niP->alpha_border){
		a = PF_MAX_CHAN16;
	}else{
		a = 0;
	}
	//******************
	A_long v = (A_long)( ( 0.29891 * (PF_FpLong)c.red) + ( 0.58661 * (PF_FpLong)c.green) + ( 0.11448 * (PF_FpLong)c.blue)+ 0.5);
	v = FROM16_SAMPLE(v);
	if (v<0) v = 0;
	else if ( v>SAMPLE_MAX) v = SAMPLE_MAX;

	v = SAMPLE_MAX - v;
	*outP = niP->col[v];
	outP->alpha = a;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
exec32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);

	PF_PixelFloat c = *inP;
	//******************
	//Matカラー
	if (niP->mat_on){
		PF_PixelFloat m = niP->mat_color;
		c.red	= RoundFpShortDouble(c.alpha*(c.red   -  m.red) + m.red);
		c.green = RoundFpShortDouble(c.alpha*(c.green -  m.green) + m.green);
		c.blue	= RoundFpShortDouble(c.alpha*(c.blue -   m.blue)/ + m.blue);
	}
	//******************
	//アルファー
	PF_FpShort a = 0;
	if (c.alpha > niP->alpha_border){
		a = 1.0;
	}else{
		a = 0;
	}
	//******************
	double v = ( ( 0.29891 * c.red) + ( 0.58661 * c.green) + ( 0.11448 * c.blue));
	v = FROM32_SAMPLE(v);
	if (v<0) v = 0;
	else if ( v>SAMPLE_MAX) v = SAMPLE_MAX;

	v = SAMPLE_MAX - v;
	*outP = niP->col[(A_long)v];
	outP->alpha = a;
	return err;
}
//-------------------------------------------------------------------------------------------------
static ParamInfo16 toParm16(ParamInfo *infoP)
{
	ParamInfo16 ret;
	ret.alpha_level = infoP->alpha_level;
	ret.alpha_border = RoundShortFpLong(PF_MAX_CHAN16 * infoP->alpha_level);
	for ( int i= 0; i<TBL_COUNT; i++){
		ret.col[i] = CONV8TO16(infoP->col[i]);
	}	
	ret.mat_on = infoP->mat_on;
	ret.mat_color = CONV8TO16(infoP->mat_color);
	return ret;

}
//-------------------------------------------------------------------------------------------------
static ParamInfo32 toParm32(ParamInfo *infoP)
{
	ParamInfo32 ret;
	ret.alpha_level = infoP->alpha_level;
	ret.alpha_border = RoundFpShortDouble( infoP->alpha_level);
	for ( int i= 0; i<TBL_COUNT; i++){
		ret.col[i] = CONV8TO32(infoP->col[i]);
	}	
	ret.mat_on = infoP->mat_on;
	ret.mat_color = CONV8TO32(infoP->mat_color);
	return ret;

}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	PF_FpLong d;
	ERR(ae->GetFLOAT(ID_ALPHA_THRESHOLD,&d));
	if (!err){
		infoP->alpha_level = d/100;
	}else{
		infoP->alpha_level = 0.5;
	}
	infoP->alpha_border = RoundByteFpLong(PF_MAX_CHAN8 * infoP->alpha_level);
	
	PF_Pixel c;
	for ( int i= 0; i<TBL_COUNT; i++){
		ERR(ae->GetCOLOR(ID_COL1+i,&c));
		if(!err){
			infoP->col[i] = c;
		}
	}
	PF_Boolean b;
	ERR(ae->GetCHECKBOX(ID_MAT_CB,&b));
	if(!err){
		infoP->mat_on = b;
	}
	ERR(ae->GetCOLOR(ID_MAT_COLOR,&c));
	if (!err) infoP->mat_color = c;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	ParamInfo16 info16;
	ParamInfo32 info32;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		info32 = toParm32(infoP);
		ERR(ae->iterate32((refconType)&info32,exec32));
		break;
	case PF_PixelFormat_ARGB64:
		info16 = toParm16(infoP);
		ERR(ae->iterate16((refconType)&info16,exec16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,exec8));
		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
/*
	SmartFXに対応していないホスト(After Effects7以前のもの)はこの関数が呼び出されて描画する
	この関数を書いておけば一応v6.5対応になる
*/
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
/*
	SmartFX対応の場合、まずこの関数が呼ばれてパラメータの獲得を行う
*/
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

#include "Fs_Entry.h"
