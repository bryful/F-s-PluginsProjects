//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Extract-Hi.h"

static PF_FpLong(*levelFunc8)(PF_Pixel8		*col, ParamInfo *infoP);
static PF_FpLong(*levelFunc16)(PF_Pixel16	*col, ParamInfo *infoP);
static PF_FpLong(*levelFunc32)(PF_Pixel32	*col, ParamInfo *infoP);

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
	PF_ADD_FLOAT_SLIDER(STR_BORDER,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						100,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						90,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_BORDER
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_SOFTNESS,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						100,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						20,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
					ID_SOFTNESS
						);
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_TARGET,
		STR_TARGET_COUNT,	//メニューの数
		STR_TARGET_DFLT,	//デフォルト
		STR_TARGET_ITEM,
		ID_TARGET
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(STR_CUSTUM_COLOR,
		0xFF,
		0x00,
		0x00,
		ID_CUSTUM_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_SUPERVISE |
		//PF_ParamFlag_CANNOT_TIME_VARY |
		//PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_INVERT,
		STR_INVERT_ON,
		FALSE,
		0,
		ID_INVERT
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
	PF_Err				err = PF_Err_NONE;
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
	PF_Err 	err = PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelLuminunce8(PF_Pixel8	*col, ParamInfo *infoP)
{
	return ((0.29891 * (PF_FpLong)col->red) + (0.58661 * (PF_FpLong)col->green) + (0.11448 * (PF_FpLong)col->blue ))/PF_MAX_CHAN8;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelLuminunce16(PF_Pixel16	*col, ParamInfo *infoP)
{
	return ((0.29891 * (PF_FpLong)col->red) + (0.58661 * (PF_FpLong)col->green) + (0.11448 * (PF_FpLong)col->blue)) / PF_MAX_CHAN16;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelLuminunce32(PF_Pixel32	*col, ParamInfo *infoP)
{
	PF_FpLong r = (0.29891 * (PF_FpLong)col->red) + (0.58661 * (PF_FpLong)col->green) + (0.11448 * (PF_FpLong)col->blue);

	if (r < 0) r = 0; else if (r > 1.0) r = 1.0;

	return r;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelRed8(PF_Pixel8	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->red  - ((PF_FpLong)col->green + (PF_FpLong)col->blue)/2)/PF_MAX_CHAN8;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelRed16(PF_Pixel16	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->red - ((PF_FpLong)col->green + (PF_FpLong)col->blue) / 2) / PF_MAX_CHAN16;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelRed32(PF_Pixel32	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->red - ((PF_FpLong)col->green + (PF_FpLong)col->blue)/2);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelGreen8(PF_Pixel8	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->green - ((PF_FpLong)col->red + (PF_FpLong)col->blue) / 2) / PF_MAX_CHAN8;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelGreen16(PF_Pixel16	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->green - ((PF_FpLong)col->red + (PF_FpLong)col->blue) / 2) / PF_MAX_CHAN16;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelGreen32(PF_Pixel32	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->green - ((PF_FpLong)col->red + (PF_FpLong)col->blue) / 2);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelBlue8(PF_Pixel8	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->blue - ((PF_FpLong)col->red + (PF_FpLong)col->green) / 2) / PF_MAX_CHAN8;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelBlue16(PF_Pixel16	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->blue - ((PF_FpLong)col->red + (PF_FpLong)col->green) / 2) / PF_MAX_CHAN16;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelBlue32(PF_Pixel32	*col, ParamInfo *infoP)
{
	return ((PF_FpLong)col->blue - ((PF_FpLong)col->red + (PF_FpLong)col->green) / 2);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelCyan8(PF_Pixel8	*col, ParamInfo *infoP)
{
	PF_Pixel8 col2;
	col2.red	= PF_MAX_CHAN8 - col->red;
	col2.green	= PF_MAX_CHAN8 - col->green;
	col2.blue	= PF_MAX_CHAN8 - col->blue;
	return levelRed8(&col2,infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelCyan16(PF_Pixel16	*col, ParamInfo *infoP)
{
	PF_Pixel16 col2;
	col2.red = PF_MAX_CHAN16 - col->red;
	col2.green = PF_MAX_CHAN16 - col->green;
	col2.blue = PF_MAX_CHAN16 - col->blue;
	return levelRed16(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelCyan32(PF_Pixel32	*col, ParamInfo *infoP)
{
	PF_Pixel32 col2;
	col2.red = (PF_FpShort)(1.0 - col->red);
	col2.green = (PF_FpShort)(1.0 - col->green);
	col2.blue = (PF_FpShort)(1.0 - col->blue);
	if (col2.red < 0) col2.red = 0;
	if (col2.green < 0) col2.green = 0;
	if (col2.blue < 0) col2.blue = 0;

	return levelRed32(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelMagenta8(PF_Pixel8	*col, ParamInfo *infoP)
{
	PF_Pixel8 col2;
	col2.red = PF_MAX_CHAN8 - col->red;
	col2.green = PF_MAX_CHAN8 - col->green;
	col2.blue = PF_MAX_CHAN8 - col->blue;
	return levelGreen8(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelMagenta16(PF_Pixel16	*col, ParamInfo *infoP)
{
	PF_Pixel16 col2;
	col2.red = PF_MAX_CHAN16 - col->red;
	col2.green = PF_MAX_CHAN16 - col->green;
	col2.blue = PF_MAX_CHAN16 - col->blue;
	return levelGreen16(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelMagenta32(PF_Pixel32	*col, ParamInfo *infoP)
{
	PF_Pixel32 col2;
	col2.red = (PF_FpShort)(1.0 - col->red);
	col2.green = (PF_FpShort)(1.0 - col->green);
	col2.blue = (PF_FpShort)(1.0 - col->blue);
	if (col2.red < 0) col2.red = 0;
	if (col2.green < 0) col2.green = 0;
	if (col2.blue < 0) col2.blue = 0;
	return levelGreen32(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelYellow8(PF_Pixel8	*col, ParamInfo *infoP)
{
	PF_Pixel8 col2;
	col2.red = PF_MAX_CHAN8 - col->red;
	col2.green = PF_MAX_CHAN8 - col->green;
	col2.blue = PF_MAX_CHAN8 - col->blue;
	return levelBlue8(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelYellow16(PF_Pixel16	*col, ParamInfo *infoP)
{
	PF_Pixel16 col2;
	col2.red = PF_MAX_CHAN16 - col->red;
	col2.green = PF_MAX_CHAN16 - col->green;
	col2.blue = PF_MAX_CHAN16 - col->blue;
	return levelBlue16(&col2, infoP);
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelYellow32(PF_Pixel32	*col, ParamInfo *infoP)
{
	PF_Pixel32 col2;
	col2.red = PF_MAX_CHAN16 - col->red;
	col2.green = PF_MAX_CHAN16 - col->green;
	col2.blue = PF_MAX_CHAN16 - col->blue;
	if (col2.red < 0) col2.red = 0;
	if (col2.green < 0) col2.green = 0;
	if (col2.blue < 0) col2.blue = 0;
	return levelBlue32(&col2, infoP);
}

//-------------------------------------------------------------------------------------------------
static PF_FpLong levelRGBMax8(PF_Pixel8	*col, ParamInfo *infoP)
{
	PF_FpLong v = (PF_FpLong)(MAX(MAX(col->red, col->green), col->blue))/PF_MAX_CHAN8;

	return v;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelRGBMax16(PF_Pixel16	*col, ParamInfo *infoP)
{
	PF_FpLong v = (PF_FpLong)(MAX(MAX(col->red, col->green), col->blue)) / PF_MAX_CHAN16;

	return v;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelRGBMax32(PF_Pixel32	*col, ParamInfo *infoP)
{
	PF_FpLong v = (PF_FpLong)(MAX(MAX(col->red, col->green), col->blue));

	return v;
}

//-------------------------------------------------------------------------------------------------
static PF_FpLong levelColor8(PF_Pixel8	*col,ParamInfo *infoP)
{
	PF_FpLong r = (PF_FpLong)col->red / PF_MAX_CHAN8;
	PF_FpLong g = (PF_FpLong)col->green / PF_MAX_CHAN8;
	PF_FpLong b = (PF_FpLong)col->blue / PF_MAX_CHAN8;

	
	PF_FpLong v = sqrt(pow((r - infoP->r), 2) + pow((g - infoP->g), 2) + pow((b - infoP->b), 2));
	v /= sqrt(3);
	if (v > 1) v = 1;
	v = 1.0 - v;

	return v;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelColor16(PF_Pixel16	*col, ParamInfo *infoP)
{
	PF_FpLong r = (PF_FpLong)col->red / PF_MAX_CHAN16;
	PF_FpLong g = (PF_FpLong)col->green / PF_MAX_CHAN16;
	PF_FpLong b = (PF_FpLong)col->blue / PF_MAX_CHAN16;


	PF_FpLong v = sqrt(pow((r - infoP->r), 2) + pow((g - infoP->g), 2) + pow((b - infoP->b), 2));
	v /= sqrt(3);
	if (v > 1) v = 1;
	v = 1.0 - v;

	return v;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelColor32(PF_Pixel32	*col, ParamInfo *infoP)
{
	PF_FpLong r = (PF_FpLong)col->red;
	PF_FpLong g = (PF_FpLong)col->green;
	PF_FpLong b = (PF_FpLong)col->blue;
	if (r > 1.0) r = 0;
	if (g > 1.0) g = 0;
	if (b > 1.0) b = 0;
	PF_FpLong v = sqrt(pow((r - infoP->r), 2) + pow((g - infoP->g), 2) + pow((b - infoP->b), 2));
	v /= sqrt(3);
	if (v > 1) v = 1;
	v = 1.0 - v;

	return v;
}
//-------------------------------------------------------------------------------------------------
static PF_FpLong levelComp(PF_FpLong v, ParamInfo * infoP)
{
	if (v < 0) v = 0; else if (v > 1.0) v = 1.0;


	if (v < infoP->low) {
		v = 0;
	}
	else if (v >= infoP->hi) {
		v = 1.0;
	}
	else {
		v = (v - infoP->low) / (infoP->hi - infoP->low);
	}
	if (v < 0) v = 0; else if (v > 1.0) v = 1.0;

	if (infoP->invert == TRUE) {
		v = 1.0 - v;
	}
	return v;

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
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = levelFunc8(outP, niP);

	v = ((PF_FpLong)outP->alpha / PF_MAX_CHAN8) * levelComp(v,niP);

	outP->alpha = RoundByteLong((A_long)(v* PF_MAX_CHAN8 + 0.5));

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
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = levelFunc16(outP, niP);

	v = ((PF_FpLong)outP->alpha / PF_MAX_CHAN16) * levelComp(v, niP);

	outP->alpha = RoundShortFpLong((A_long)(v* PF_MAX_CHAN16 + 0.5));
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
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong v = levelFunc32(outP, niP);

	v = (PF_FpLong)outP->alpha  * levelComp(v, niP);

	outP->alpha = RoundFpShortDouble(v);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_FpLong b = 90;
	PF_FpLong s = 20;
	ERR(ae->GetFLOAT(ID_BORDER,&b));
	ERR(ae->GetFLOAT(ID_SOFTNESS,&s));


	infoP->hi =  b / 100;
	infoP->low = (b - s) / 100;

	ERR(ae->GetPOPUP(ID_TARGET, &infoP->taget));
	ERR(ae->GetCOLOR(ID_CUSTUM_COLOR, &infoP->custumColor));

	infoP->r = (PF_FpLong)infoP->custumColor.red / PF_MAX_CHAN8;
	infoP->g = (PF_FpLong)infoP->custumColor.green / PF_MAX_CHAN8;
	infoP->b = (PF_FpLong)infoP->custumColor.blue / PF_MAX_CHAN8;


	ERR(ae->GetCHECKBOX(ID_INVERT,&infoP->invert));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	switch (infoP->taget) {
	case 2:
		levelFunc8 = levelRed8;
		levelFunc16 = levelRed16;
		levelFunc32 = levelRed32;
		break;
	case 3:
		levelFunc8 = levelGreen8;
		levelFunc16 = levelGreen16;
		levelFunc32 = levelGreen32;
		break;
	case 4:
		levelFunc8 = levelBlue8;
		levelFunc16 = levelBlue16;
		levelFunc32 = levelBlue32;
		break;
	case 5:
		levelFunc8 = levelCyan8;
		levelFunc16 = levelCyan16;
		levelFunc32 = levelCyan32;
		break;
	case 6:
		levelFunc8 = levelMagenta8;
		levelFunc16 = levelCyan16;
		levelFunc32 = levelCyan32;
		break;
	case 7:
		levelFunc8 = levelYellow8;
		levelFunc16 = levelLuminunce16;
		levelFunc32 = levelLuminunce32;
		break;
	case 8:
		levelFunc8 = levelRGBMax8;
		levelFunc16 = levelRGBMax16;
		levelFunc32 = levelRGBMax32;
		break;
	case 9:
		levelFunc8 = levelColor8;
		levelFunc16 = levelColor16;
		levelFunc32 = levelColor32;
		break;
	case 1:
	default:
		levelFunc8 = levelLuminunce8;
		levelFunc16 = levelLuminunce16;
		levelFunc32 = levelLuminunce32;
		break;
	}

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
