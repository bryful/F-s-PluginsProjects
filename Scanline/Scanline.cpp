//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Scanline.h"


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
	PF_ADD_SLIDER(STR_HEIGHT,	//パラメータの名前
		1, 			//数値入力する場合の最小値
		20,			//数値入力する場合の最大値
		2,			//スライダーの最小値 
		10,			//スライダーの最大値
		3,				//デフォルトの値
		ID_HEIGHT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LEVEL0,	//Name
		-100,						//VALID_MIN
		100,						//VALID_MAX
		-100,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		-50,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LEVEL0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OPACITY0,	//Name
		0,						//VALID_MIN
		100,						//VALID_MAX
		0,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		50,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OPACITY0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LEVEL1,	//Name
		-100,						//VALID_MIN
		100,						//VALID_MAX
		-100,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LEVEL1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OPACITY1,	//Name
		0,						//VALID_MIN
		100,						//VALID_MAX
		0,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OPACITY1
	);

	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_DIR,
		2,	//メニューの数
		1,	//デフォルト
		STR_DIRSTR,
		ID_DIR
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
//-------------------------------------------------------------------------------------
inline PF_Pixel PX8(PF_Pixel c, double lv, double opa)
{
	PF_Pixel ret = { 0,0,0,0 };
	if (opa <= 0)
	{
		return ret;
	}
	else if(opa>=1){
		ret.alpha = c.alpha;

	}
	else {
		ret.alpha = RoundByteLong((A_long)((double)c.alpha * opa + 0.5));
	}
	
	if (lv == 0) {
		ret = c;
	}
	else if (lv > 0) {
		ret.red = RoundByteLong( (A_long)((double)c.red + (PF_MAX_CHAN8 - (double)c.red)*lv + 0.5));
		ret.green = RoundByteLong((A_long)((double)c.green + (PF_MAX_CHAN8 - (double)c.green)*lv + 0.5));
		ret.blue = RoundByteLong((A_long)((double)c.blue + (PF_MAX_CHAN8 - (double)c.blue)*lv + 0.5));
	}
	else {
		ret.red = RoundByteLong((A_long)((double)c.red + ((double)c.red)*lv + 0.5));
		ret.green = RoundByteLong((A_long)((double)c.green + ((double)c.green)*lv + 0.5));
		ret.blue = RoundByteLong((A_long)((double)c.blue + ((double)c.blue)*lv + 0.5));
	}

	return ret;

}
//-------------------------------------------------------------------------------------
inline PF_Pixel16 PX16(PF_Pixel16 c, double lv, double opa)
{
	PF_Pixel16 ret = { 0,0,0,0 };
	if (opa <= 0)
	{
		return ret;
	}
	else if (opa >= 1) {
		ret.alpha = c.alpha;

	}
	else {
		ret.alpha = RoundShort((A_long)((double)c.alpha * opa + 0.5));
	}

	if (lv == 0) {
		ret = c;
	}
	else if (lv > 0) {
		ret.red = RoundShort((A_long)((double)c.red + (PF_MAX_CHAN16 - (double)c.red)*lv + 0.5));
		ret.green = RoundShort((A_long)((double)c.green + (PF_MAX_CHAN16 - (double)c.green)*lv + 0.5));
		ret.blue = RoundShort((A_long)((double)c.blue + (PF_MAX_CHAN16 - (double)c.blue)*lv + 0.5));
	}
	else {
		ret.red = RoundShort((A_long)((double)c.red + ((double)c.red)*lv + 0.5));
		ret.green = RoundShort((A_long)((double)c.green + ((double)c.green)*lv + 0.5));
		ret.blue = RoundShort((A_long)((double)c.blue + ((double)c.blue)*lv + 0.5));
	}

	return ret;

}
//-------------------------------------------------------------------------------------
inline PF_PixelFloat PX32(PF_PixelFloat c, double lv, double opa)
{
	PF_PixelFloat ret = { 0,0,0,0 };
	if (opa <= 0)
	{
		return ret;
	}
	else if (opa >= 1) {
		ret.alpha = c.alpha;

	}
	else {
		ret.alpha = RoundFpShortDouble(c.alpha * opa);
	}

	if (lv == 0) {
		ret = c;
	}
	else if (lv > 0) {
		ret.red = RoundFpShortDouble(((double)c.red + (1 - (double)c.red)*lv));
		ret.green = RoundFpShortDouble(((double)c.green + (1 - (double)c.green)*lv));
		ret.blue = RoundFpShortDouble(((double)c.blue + (1 - (double)c.blue)*lv));
	}
	else {
		ret.red = RoundFpShortDouble(((double)c.red + ((double)c.red)*lv));
		ret.green = RoundFpShortDouble(((double)c.green + ((double)c.green)*lv));
		ret.blue = RoundFpShortDouble(((double)c.blue + ((double)c.blue)*lv));
	}

	return ret;

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

	if (infoP->dir == 1)
	{
		A_long y = yL / infoP->height;
		if ((y % 2) == 0) {//偶数
			if ((infoP->level0 != 0) || (infoP->opacity0 != 1)){
				*outP = PX8(*outP, infoP->level0, infoP->opacity0);
			}
		}
		else {
			if ((infoP->level1 != 0) || (infoP->opacity1 != 1)) {
				*outP = PX8(*outP, infoP->level1, infoP->opacity1);
			}
		}
	}
	else {
		A_long x = xL / infoP->height;
		if ((x % 2) == 0) {//偶数
			if ((infoP->level0 != 0) || (infoP->opacity0 != 1)) {
				*outP = PX8(*outP, infoP->level0, infoP->opacity0);
			}
		}
		else {
			if ((infoP->level1 != 0) || (infoP->opacity1 != 1)) {
				*outP = PX8(*outP, infoP->level1, infoP->opacity1);
			}
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
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (infoP->dir == 1)
	{
		A_long y = yL / infoP->height;
		if ((y % 2) == 0) {//偶数
			if ((infoP->level0 != 0) || (infoP->opacity0 != 1)) {
				*outP = PX16(*outP, infoP->level0, infoP->opacity0);
			}
		}
		else {
			if ((infoP->level1 != 0) || (infoP->opacity1 != 1)) {
				*outP = PX16(*outP, infoP->level1, infoP->opacity1);
			}
		}
	}
	else {
		A_long x = xL / infoP->height;
		if ((x % 2) == 0) {//偶数
			if ((infoP->level0 != 0) || (infoP->opacity0 != 1)) {
				*outP = PX16(*outP, infoP->level0, infoP->opacity0);
			}
		}
		else {
			if ((infoP->level1 != 0) || (infoP->opacity1 != 1)) {
				*outP = PX16(*outP, infoP->level1, infoP->opacity1);
			}
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
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	if (infoP->dir == 1)
	{
		A_long y = yL / infoP->height;
		if ((y % 2) == 0) {//偶数
			if ((infoP->level0 != 0) || (infoP->opacity0 != 1)) {
				*outP = PX32(*outP, infoP->level0, infoP->opacity0);
			}
		}
		else {
			if ((infoP->level1 != 0) || (infoP->opacity1 != 1)) {
				*outP = PX32(*outP, infoP->level1, infoP->opacity1);
			}
		}
	}
	else {
		A_long x = xL / infoP->height;
		if ((x % 2) == 0) {//偶数
			if ((infoP->level0 != 0) || (infoP->opacity0 != 1)) {
				*outP = PX32(*outP, infoP->level0, infoP->opacity0);
			}
		}
		else {
			if ((infoP->level1 != 0) || (infoP->opacity1 != 1)) {
				*outP = PX32(*outP, infoP->level1, infoP->opacity1);
			}
		}

	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	ERR(ae->GetADD(ID_HEIGHT, &infoP->height));


	ERR(ae->GetFLOAT(ID_LEVEL0,&infoP->level0));
	infoP->level0 /= 100;
	ERR(ae->GetFLOAT(ID_LEVEL1, &infoP->level1));
	infoP->level1 /= 100;

	ERR(ae->GetFLOAT(ID_OPACITY0, &infoP->opacity0));
	infoP->opacity0 /= 100;
	ERR(ae->GetFLOAT(ID_OPACITY1, &infoP->opacity1));
	infoP->opacity1 /= 100;

	ERR(ae->GetPOPUP(ID_DIR, &infoP->dir));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	

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
