//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "graytoneToColorize.h"

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
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_TARGET,
		STR_TARGET_COUNT,	//メニューの数
		STR_TARGET_DEFLT,	//デフォルト
		STR_TARGET_ITEMS,
		ID_TARGET
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL0, 0x9E, 0x87, 0x2A, ID_COL0);//9E872A
	PF_ADD_COLOR(STR_COL1, 0xF2, 0xF3, 0x9C, ID_COL1);//F2F39C
	PF_ADD_COLOR(STR_COL2, 0xA7, 0x99, 0x39, ID_COL2);//A79939
	PF_ADD_COLOR(STR_COL3, 0xF8, 0xF0, 0x68, ID_COL3);//F8F068
	PF_ADD_COLOR(STR_COL4, 0xBA, 0x9B, 0x0A, ID_COL4);//BA9B0A
	PF_ADD_COLOR(STR_COL5, 0xD3, 0xBE, 0x11, ID_COL5);//D3BE11
	PF_ADD_COLOR(STR_COL6, 0xF9, 0xDF, 0x4C, ID_COL6);//F9DF4C
	PF_ADD_COLOR(STR_COL7, 0xFC, 0xF5, 0xCC, ID_COL7);//FCF5CC
	PF_ADD_COLOR(STR_COL8, 0x9E, 0x87, 0x2A, ID_COL8);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_REP,	//Name
		1,							//VALID_MIN
		10,						//VALID_MAX
		1,							//SLIDER_MIN
		5,							//SLIDER_MAX
		0,							//CURVE_TOLERANCE
		2.5,							//DFLT
		1,							//PREC
		0,							//DISP
		0,							//WANT_PHASE
		ID_REP
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_OFFSET_ON,
		"on",
		TRUE,
		0,
		ID_OFFSET_ON
	);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_OFFSET, 0, ID_OFFSET);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OFFSET_SPEED,	//Name
		-10,							//VALID_MIN
		10,						//VALID_MAX
		-2,							//SLIDER_MIN
		2,							//SLIDER_MAX
		0,							//CURVE_TOLERANCE
		0.2,						//DFLT
		2,							//PREC
		0,							//DISP
		0,							//WANT_PHASE
		ID_OFFSET_SPEED
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
static PF_Err
execTargetV8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	double a = (double)inP->alpha / PF_MAX_CHAN8;
	switch (infoP->target)
	{

	case 2:
		outP->blue = outP->green = outP->red = RoundByteLong((A_long)((double)inP->red *a + 0.5));
		break;
	case 3:
		outP->blue = outP->green = outP->red = RoundByteLong((A_long)((double)inP->green *a + 0.5));
		break;
	case 4:
		outP->blue = outP->green = outP->red = RoundByteLong((A_long)((double)inP->blue *a + 0.5));
		break;
	case 5:
		outP->blue = outP->green = outP->red = inP->alpha;
		break;
	default:
	case 1:
		A_long r = (A_long)((double)inP->red *a + 0.5);
		if (r > PF_MAX_CHAN8)r = PF_MAX_CHAN8;
		A_long g = (A_long)((double)inP->green *a + 0.5);
		if (g > PF_MAX_CHAN8)g = PF_MAX_CHAN8;
		A_long b = (A_long)((double)inP->blue *a + 0.5);
		if (b > PF_MAX_CHAN8)b = PF_MAX_CHAN8;

		A_long v = (A_long)((0.29891 * (double)r) + (0.58661 * (double)g) + (0.11448 * (double)b) +0.5);
		outP->blue = outP->green = outP->red = RoundByteLong(v);
		break;
	}
	outP->alpha = inP->alpha;

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err
execTargetV16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	double a = (double)inP->alpha / PF_MAX_CHAN16;
	switch (infoP->target)
	{

	case 2:
		outP->blue = outP->green = outP->red = RoundShort((A_long)((double)inP->red *a + 0.5));
		break;
	case 3:
		outP->blue = outP->green = outP->red = RoundShort((A_long)((double)inP->green *a + 0.5));
		break;
	case 4:
		outP->blue = outP->green = outP->red = RoundShort((A_long)((double)inP->blue *a + 0.5));
		break;
	case 5:
		outP->blue = outP->green = outP->red = inP->alpha;
		break;
	default:
	case 1:
		A_long r = (A_long)((double)inP->red *a + 0.5);
		if (r > PF_MAX_CHAN16)r = PF_MAX_CHAN16;
		A_long g = (A_long)((double)inP->green *a + 0.5);
		if (g > PF_MAX_CHAN16)g = PF_MAX_CHAN16;
		A_long b = (A_long)((double)inP->blue *a + 0.5);
		if (b > PF_MAX_CHAN16)b = PF_MAX_CHAN16;

		A_long v = (A_long)((0.29891 * (double)r) + (0.58661 * (double)g) + (0.11448 * (double)b) + 0.5);
		outP->blue = outP->green = outP->red = RoundShort(v);
		break;
	}
	outP->alpha = inP->alpha;

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err
execTargetV32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	double a = (double)inP->alpha;
	switch (infoP->target)
	{

	case 2:
		outP->blue = outP->green = outP->red = RoundFpShortDouble(inP->red *a);
		break;
	case 3:
		outP->blue = outP->green = outP->red = RoundFpShortDouble(inP->green *a);
		break;
	case 4:
		outP->blue = outP->green = outP->red = RoundFpShortDouble(inP->blue *a);
		break;
	case 5:
		outP->blue = outP->green = outP->red = inP->alpha;
		break;
	default:
	case 1:
		double r = inP->red *a;
		if (r > 1)r = 1;
		double g = inP->green *a;
		if (g > 1)g = 1;
		double b = inP->blue *a;
		if (b > 1)b = 1;

		double v = ((0.29891 * r) + (0.58661 * g) + (0.11448 * b));
		outP->blue = outP->green = outP->red = RoundFpShortDouble(v);
		break;
	}
	outP->alpha = inP->alpha;

	return err;

}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel colBlend8(PF_Pixel s, PF_Pixel d, double p)
{
	PF_Pixel ret;
	if (p == 0) return s;
	else if (p >= 1) return d;
	ret.red = RoundByteLong((A_long)((double)d.red * p + (double)s.red * (1 - p)));
	ret.green = RoundByteLong((A_long)((double)d.green * p + (double)s.green * (1 - p)));
	ret.blue = RoundByteLong((A_long)((double)d.blue * p + (double)s.blue * (1 - p)));
	return ret;
}
//-------------------------------------------------------------------------------------------------
inline PF_Pixel16 colBlend16(PF_Pixel16 s, PF_Pixel16 d, double p)
{
	PF_Pixel16 ret;
	if (p == 0) return s;
	else if (p >= 1) return d;
	ret.red = RoundShortFpLong((A_long)((double)d.red * p + (double)s.red * (1 - p)));
	ret.green = RoundShortFpLong((A_long)((double)d.green * p + (double)s.green * (1 - p)));
	ret.blue = RoundShortFpLong((A_long)((double)d.blue * p + (double)s.blue * (1 - p)));
	return ret;
}
//-------------------------------------------------------------------------------------------------
inline PF_PixelFloat colBlend32(PF_PixelFloat s, PF_PixelFloat d, double p)
{
	PF_PixelFloat ret;
	if (p == 0) return s;
	else if (p >= 1) return d;
	ret.red = RoundFpShortDouble(((double)d.red * p + (double)s.red * (1 - p)));
	ret.green = RoundFpShortDouble(((double)d.green * p + (double)s.green * (1 - p)));
	ret.blue = RoundFpShortDouble(((double)d.blue * p + (double)s.blue * (1 - p)));
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
execColorize8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);


	A_u_char a = outP->alpha;

	A_long v = outP->red;

	if (infoP->offset_rep > 1) {
		v = (A_long)((double)v*infoP->offset_rep) % PF_MAX_CHAN8;
	}

	if (infoP->offset_enabled ==TRUE) {

		double offset = infoP->offset + infoP->offset_speed*(double)infoP->frame;
		v = (A_long)((double)v + offset*PF_MAX_CHAN8);
		v = v % (PF_MAX_CHAN8+1);

	}

	A_long idx = 0;
	for (int i = 0; i < COL_COUNT; i++)
	{
		if (v <= infoP->colBorder[i]) {
			idx = i;
			break;
		}
	}
	if (idx == 0) {
		*outP = infoP->col[0];
	}
	else {
		double p = ((double)v - (double)infoP->colBorder[idx - 1]) / (double)(infoP->colBorder[idx] - infoP->colBorder[idx - 1]);
		*outP = colBlend8(infoP->col[idx - 1], infoP->col[idx], p);
	}

	outP->alpha = a;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
execColorize16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP = reinterpret_cast<ParamInfo16*>(refcon);


	A_u_short a = outP->alpha;

	A_long v = outP->red;

	if (infoP->offset_rep > 1) {
		v = (A_long)((double)v*infoP->offset_rep) % PF_MAX_CHAN16;
	}

	if (infoP->offset_enabled == TRUE) {

		double offset = infoP->offset + infoP->offset_speed*(double)infoP->frame;
		v = (A_long)((double)v + offset * PF_MAX_CHAN16);
		v = v % (PF_MAX_CHAN16+1);

	}

	A_long idx = 0;
	for (int i = 0; i < COL_COUNT; i++)
	{
		if (v <= infoP->colBorder[i]) {
			idx = i;
			break;
		}
	}
	if (idx == 0) {
		*outP = infoP->col[0];
	}
	else {
		double p = ((double)v - (double)infoP->colBorder[idx - 1]) / (double)(infoP->colBorder[idx] - infoP->colBorder[idx - 1]);
		*outP = colBlend16(infoP->col[idx - 1], infoP->col[idx], p);
	}

	outP->alpha = a;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
execColorize32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	infoP = reinterpret_cast<ParamInfo32*>(refcon);


	PF_FpShort a = outP->alpha;

	A_long v = (A_long)(outP->red * PF_MAX_CHAN16+0.5);
	if (v > PF_MAX_CHAN16) v = PF_MAX_CHAN16;

	if (infoP->offset_rep > 1) {
		v = (A_long)((double)v*infoP->offset_rep) % PF_MAX_CHAN16;
	}

	if (infoP->offset_enabled == TRUE) {

		double offset = infoP->offset + infoP->offset_speed*(double)infoP->frame;
		v = (A_long)((double)v + offset * PF_MAX_CHAN16);
		v = v % (PF_MAX_CHAN16+1);

	}

	A_long idx = 0;
	for (int i = 0; i < COL_COUNT; i++)
	{
		if (v <= infoP->colBorder[i]) {
			idx = i;
			break;
		}
	}
	if (idx == 0) {
		*outP = infoP->col[0];
	}
	else {
		double p = ((double)v - (double)infoP->colBorder[idx - 1]) / (double)(infoP->colBorder[idx] - infoP->colBorder[idx - 1]);
		*outP = colBlend32(infoP->col[idx - 1], infoP->col[idx], p);
	}

	outP->alpha = a;

	return err;
}
//-------------------------------------------------------------------------------------------------
static ParamInfo16 toParm16(ParamInfo *infoP)
{
	ParamInfo16 ret;
	ret.target = infoP->target;
	ret.offset = infoP->offset;
	ret.offset_speed = infoP->offset_speed;
	ret.offset_rep = infoP->offset_rep;
	ret.frame = infoP->frame;
	ret.offset_enabled = infoP->offset_enabled;

	for (int i = 0; i < COL_COUNT; i++) {
		ret.col[i] = CONV8TO16(infoP->col[i]);
	}
	for (int i = 0; i <= COL_COUNT - 1; i++)
	{
		double v = (double)i*PF_MAX_CHAN16 / (COL_COUNT - 1);
		ret.colBorder[i] = RoundShort((A_long)(v + 0.5));
	}
	return ret;

}
//-------------------------------------------------------------------------------------------------
static ParamInfo32 toParm32(ParamInfo *infoP)
{
	ParamInfo32 ret;
	ret.target = infoP->target;
	ret.offset = infoP->offset;
	ret.offset_speed = infoP->offset_speed;
	ret.offset_rep = infoP->offset_rep;
	ret.frame = infoP->frame;
	ret.offset_enabled = infoP->offset_enabled;

	for (int i = 0; i < COL_COUNT; i++) {
		ret.col[i] = CONV8TO32(infoP->col[i]);
	}
	for (int i = 0; i <= COL_COUNT - 1; i++)
	{
		double v = (double)i*PF_MAX_CHAN16 / (COL_COUNT - 1);
		ret.colBorder[i] = RoundShort((A_long)(v + 0.5));
	}
	return ret;

}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetPOPUP(ID_TARGET, &infoP->target));

	PF_Pixel c;
	for (int i = 0; i < COL_COUNT; i++) {
		ERR(ae->GetCOLOR(ID_COL0 + i, &c));
		if (!err) {
			infoP->col[i] = c;
			infoP->col[i].alpha = PF_MAX_CHAN8;;
		}
	}
	for (int i = 0; i <= COL_COUNT-1; i++)
	{
		double v = (double)i*PF_MAX_CHAN8 / (COL_COUNT-1);
		A_long vv = (A_long)(v + 0.5);
		if (vv > PF_MAX_CHAN8) vv = PF_MAX_CHAN8;
		infoP->colBorder[i] = vv;
	}

	ERR(ae->GetFLOAT(ID_REP, &infoP->offset_rep));


	ERR(ae->GetCHECKBOX(ID_OFFSET_ON, &infoP->offset_enabled));

	if ( infoP->offset_enabled == TRUE) {
		PF_Fixed r;
		ERR(ae->GetANGLE(ID_OFFSET, &r));
		r = r % (360 << 16L);
		if (r < 0) r += (360 << 16L);
		infoP->offset = (PF_FpShort)((double)r / 65536.0);
		infoP->offset /= 360;
		ERR(ae->GetFLOAT(ID_OFFSET_SPEED, &infoP->offset_speed));
		infoP->offset_speed /= 24;
	}
	else {
		infoP->offset = 0;
		infoP->offset_speed = 0;
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
	infoP->frame = ae->frame();
	ParamInfo16 info16;
	ParamInfo32 info32;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		info32 = toParm32(infoP);
		info32.frame = ae->frame();
		ERR(ae->iterate32((refconType)&info32, execTargetV32));
		ERR(ae->iterate32((refconType)&info32, execColorize32));
		break;
	case PF_PixelFormat_ARGB64:
		info16 = toParm16(infoP);
		info16.frame = ae->frame();
		ERR(ae->iterate16((refconType)&info16, execTargetV16));
		ERR(ae->iterate16((refconType)&info16, execColorize16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP, execTargetV8));
		ERR(ae->iterate8((refconType)infoP, execColorize8));
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
