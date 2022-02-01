//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "ToGray.h"

static PF_Err (*ToGrayFunc8)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_Pixel8	*inP, 
			PF_Pixel8	*outP);

static PF_Err (*ToGrayFunc16)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_Pixel16	*inP, 
			PF_Pixel16	*outP);

static PF_Err (*ToGrayFunc32)(
			refconType	refcon, 
			A_long		xL, 
			A_long		yL, 
			PF_PixelFloat 	*inP, 
			PF_PixelFloat	*outP);

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
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_GRAY_MODE_POP1, 
						STR_GRAY_MODE_COUNT,	//メニューの数
						STR_GRAY_MODE_DEF,	//デフォルト
						STR_GRAY_MODE_POP2,
						ID_GRAY_MODE
						);

	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_ORG_BLEND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					2,				//数値表示に関するフラグ 
					0,
					0,
					ID_ORG_BLEND
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
//**********************************************************************************************
inline PF_Pixel GMIX8(PF_Pixel *s, A_long v, PF_FpShort p)
{
	PF_Pixel r;
	if ( v<0) v=0;
	else if ( v> PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	if ( p <=0){
		r.alpha = s->alpha;
		r.red =
		r.green =
		r.blue = (A_u_char)v;
	}else{
		PF_FpShort d0 = p;
		PF_FpShort d1 = 1 - p;
		r.alpha = s->alpha;
		r.red	= RoundByteFpLong(((PF_FpShort)s->red   * d0 + (PF_FpShort)v * d1));
		r.green	= RoundByteFpLong(((PF_FpShort)s->green * d0 + (PF_FpShort)v * d1));
		r.blue	= RoundByteFpLong(((PF_FpShort)s->blue  * d0 + (PF_FpShort)v * d1));
	}
	return r;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc8_yuv (
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err	err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = (A_long)( ( 0.29891 * (PF_FpLong)inP->red) + ( 0.58661 * (PF_FpLong)inP->green) + ( 0.11448 * (PF_FpLong)inP->blue));
		*outP = GMIX8(inP,v,niP->org_blend); 
	}	 
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc8_hls (
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = (MAX( MAX(inP->red,inP->green), inP->blue ) + MIN( MIN(inP->red,inP->green), inP->blue )) >>1;
		*outP = GMIX8(inP,v,niP->org_blend); 
	}	
	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc8_rgb(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = (inP->red + inP->green +inP->blue )/3;
		*outP = GMIX8(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc8_r(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = inP->red;
		*outP = GMIX8(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc8_g(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = inP->green;
		*outP = GMIX8(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc8_b(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = inP->blue;
		*outP = GMIX8(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ToGrayFunc8_rgbmax(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP) {
		A_long v = MAX(MAX(inP->red, inP->green), inP->blue);
		*outP = GMIX8(inP, v, niP->org_blend);
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ToGrayFunc8_lab(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP) {
		PF_PixelFloat c = CONV8TO32(*outP);
		LABA lab = RgbToLab(c);

		A_long v = (A_long)(lab.L * PF_MAX_CHAN8 +0.5);
		*outP = GMIX8(inP, v, niP->org_blend);
	}
	return err;
}
//**********************************************************************************************
inline PF_Pixel16 GMIX16(PF_Pixel16 *s, A_long v, PF_FpShort p)
{
	PF_Pixel16 r;
	if ( v<0) v=0;
	else if ( v> PF_MAX_CHAN16) v = PF_MAX_CHAN16;
	if ( p <=0){
		r.alpha = s->alpha;
		r.red =
		r.green =
		r.blue = (A_u_short)v;
	}else{
		PF_FpShort d0 = p;
		PF_FpShort d1 = 1 - p;
		r.alpha = s->alpha;
		r.red	= RoundShortFpLong(((PF_FpShort)s->red * d0 + (PF_FpShort)v * d1));
		r.green	= RoundShortFpLong(((PF_FpShort)s->green *d0 + (PF_FpShort)v * d1));
		r.blue	= RoundShortFpLong(((PF_FpShort)s->blue * d0 + (PF_FpShort)v * d1));
	}
	return r;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc16_yuv (
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err	err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = (A_long)( ( 0.29891 * (PF_FpLong)inP->red) + ( 0.58661 * (PF_FpLong)inP->green) + ( 0.11448 * (PF_FpLong)inP->blue));
		*outP = GMIX16(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc16_hls (
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = (MAX( MAX(inP->red,inP->green), inP->blue ) + MIN( MIN(inP->red,inP->green), inP->blue )) >>1;
		*outP = GMIX16(inP,v,niP->org_blend); 
	}	
	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc16_rgb(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = (inP->red + inP->green +inP->blue )/3;
		*outP = GMIX16(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc16_r(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = inP->red;
		*outP = GMIX16(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc16_g(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = inP->green;
		*outP = GMIX16(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc16_b(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		A_long v = inP->blue;
		*outP = GMIX16(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ToGrayFunc16_rgbmax(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP) {
		A_long v = MAX(MAX(inP->red, inP->green), inP->blue);
		*outP = GMIX16(inP, v, niP->org_blend);
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ToGrayFunc16_lab(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP) {
		PF_PixelFloat c = CONV16TO32(*outP);
		LABA lab = RgbToLab(c);

		A_long v = RoundShortFpLong(lab.L * PF_MAX_CHAN16 + 0.5);
		*outP = GMIX16(inP, v, niP->org_blend);
	}
	return err;
}
//**********************************************************************************************
inline PF_Pixel32 GMIX32(PF_PixelFloat *s, PF_FpLong v, PF_FpShort p)
{
	PF_PixelFloat r;
	if ( v<0) v=0;
	else if ( v> 1) v = 1;
	if ( p <=0){
		r.alpha = s->alpha;
		r.red =
		r.green =
		r.blue = (PF_FpShort)v;
	}else{
		PF_FpShort d0 = p;
		PF_FpShort d1 = 1 - p;
		r.alpha = s->alpha;
		r.red	= RoundFpShortDouble(((PF_FpShort)s->red * d0 + (PF_FpShort)v * d1));
		r.green	= RoundFpShortDouble(((PF_FpShort)s->green *d0 + (PF_FpShort)v * d1));
		r.blue	= RoundFpShortDouble(((PF_FpShort)s->blue * d0 + (PF_FpShort)v * d1));
	}
	return r;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc32_yuv (
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err	err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		PF_FpShort v = (PF_FpShort)( ( 0.29891 * (PF_FpLong)inP->red) + ( 0.58661 * (PF_FpLong)inP->green) + ( 0.11448 * (PF_FpLong)inP->blue));
		*outP = GMIX32(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc32_hls (
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		PF_FpShort v = (MAX( MAX(inP->red,inP->green), inP->blue ) + MIN( MIN(inP->red,inP->green), inP->blue )) /2;
		*outP = GMIX32(inP,v,niP->org_blend); 
	}	
	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc32_rgb(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		PF_FpShort v = (inP->red + inP->green +inP->blue )/3;
		*outP = GMIX32(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc32_r(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		PF_FpShort v = inP->red;
		*outP = GMIX32(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc32_g(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		PF_FpShort v = inP->green;
		*outP = GMIX32(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ToGrayFunc32_b(
	refconType	refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	if (niP){
		PF_FpShort v = inP->blue;
		*outP = GMIX32(inP,v,niP->org_blend); 
	}	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ToGrayFunc32_rgbmax(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP) {
		PF_FpLong v = MAX(MAX(inP->red, inP->green), inP->blue);
		*outP = GMIX32(inP, v, niP->org_blend);
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ToGrayFunc32_lab(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err		err = PF_Err_NONE;
	ParamInfo *	niP = reinterpret_cast<ParamInfo*>(refcon);
	if (niP) {
		LABA lab = RgbToLab(*outP);

		PF_FpLong v = lab.L;
		if (v > 1) v = 1;
		*outP = GMIX32(inP, v, niP->org_blend);
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetPOPUP(ID_GRAY_MODE,&infoP->gray_mode));
	ERR(ae->GetFIXED2FpShort_P1(ID_ORG_BLEND,&infoP->org_blend));
	infoP->in_data = ae->in_data;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー

	ae->CopyInToOut();
	if ( infoP->org_blend<1){
		switch(infoP->gray_mode)
		{//#define STR_GRAY_MODE_POP2	"(Y)uv|h(L)s|(R+G+B)/3|(R)gb|r(G)b|rg(B)"
		case 1:
			ToGrayFunc8 = ToGrayFunc8_yuv;
			ToGrayFunc16 = ToGrayFunc16_yuv;
			ToGrayFunc32 = ToGrayFunc32_yuv;
			break;
		case 2:
			ToGrayFunc8 = ToGrayFunc8_hls;
			ToGrayFunc16 = ToGrayFunc16_hls;
			ToGrayFunc32 = ToGrayFunc32_hls;
			break;
		case 3:
			ToGrayFunc8 = ToGrayFunc8_rgb;
			ToGrayFunc16 = ToGrayFunc16_rgb;
			ToGrayFunc32 = ToGrayFunc32_rgb;
			break;
		case 4:
			ToGrayFunc8 = ToGrayFunc8_r;
			ToGrayFunc16 = ToGrayFunc16_r;
			ToGrayFunc32 = ToGrayFunc32_r;
			break;
		case 5:
			ToGrayFunc8 = ToGrayFunc8_g;
			ToGrayFunc16 = ToGrayFunc16_g;
			ToGrayFunc32 = ToGrayFunc32_g;
			break;
		case 6:
			ToGrayFunc8 = ToGrayFunc8_b;
			ToGrayFunc16 = ToGrayFunc16_b;
			ToGrayFunc32 = ToGrayFunc32_b;
			break;
		case 7:
			ToGrayFunc8 = ToGrayFunc8_rgbmax;
			ToGrayFunc16 = ToGrayFunc16_rgbmax;
			ToGrayFunc32 = ToGrayFunc32_rgbmax;
			break;
		case 8:
			ToGrayFunc8 = ToGrayFunc8_lab;
			ToGrayFunc16 = ToGrayFunc16_lab;
			ToGrayFunc32 = ToGrayFunc32_lab;
			break;
		}
		switch(ae->pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			ERR(ae->iterate32((refconType)infoP,ToGrayFunc32));
			break;
		case PF_PixelFormat_ARGB64:
			ERR(ae->iterate16((refconType)infoP,ToGrayFunc16));
			break;
		case PF_PixelFormat_ARGB32:
			ERR(ae->iterate8((refconType)infoP,ToGrayFunc8));
			break;
		}
	}else{
		ERR(ae->CopyInToOut());
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
