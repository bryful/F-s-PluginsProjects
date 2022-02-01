//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "grayToCountourLine.h"


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
	PF_ADD_SLIDER(STR_LO,	//パラメータの名前
		1, 					//数値入力する場合の最小値
		10,					//数値入力する場合の最大値
		1,					//スライダーの最小値 
		10,					//スライダーの最大値
		1,					//デフォルトの値
		ID_COUNT_LO
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_MID,	//パラメータの名前
		1, 					//数値入力する場合の最小値
		10,					//数値入力する場合の最大値
		1,					//スライダーの最小値 
		10,					//スライダーの最大値
		1,					//デフォルトの値
		ID_COUNT_MID
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_HI,	//パラメータの名前
		1, 					//数値入力する場合の最小値
		10,					//数値入力する場合の最大値
		1,					//スライダーの最小値 
		10,					//スライダーの最大値
		1,					//デフォルトの値
		ID_COUNT_HI
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_DRAW_COL,
		STR_ON,
		FALSE,
		0,
		ID_DRAW_COL
	);

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
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	double v = ((0.29891 * (PF_FpLong)inP->red) + (0.58661 * (PF_FpLong)inP->green) + (0.11448 * (PF_FpLong)inP->blue));
	if (inP->alpha < PF_MAX_CHAN8) {
		v *= (PF_FpLong)inP->alpha / PF_MAX_CHAN8;
	}
	if (v > PF_MAX_CHAN8) v = PF_MAX_CHAN8;
	A_u_char v2 = infoP->tbl[(A_long)v];

	outP->alpha = PF_MAX_CHAN8;
	outP->red = outP->green = outP->blue = v2;

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
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel c = CONV16TO8(*inP);

	double v = ((0.29891 * (PF_FpLong)c.red) + (0.58661 * (PF_FpLong)c.green) + (0.11448 * (PF_FpLong)c.blue));
	if (c.alpha < PF_MAX_CHAN8) {
		v *= (PF_FpLong)c.alpha / PF_MAX_CHAN8;
	}
	if (v > PF_MAX_CHAN8) v = PF_MAX_CHAN8;

	A_u_char v2 = infoP->tbl[(A_long)v];

	c.alpha = PF_MAX_CHAN8;
	c.red = c.green = c.blue = v2;

	*outP = CONV8TO16(c);

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
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel c = CONV32TO8(*inP);

	double v = ((0.29891 * (PF_FpLong)c.red) + (0.58661 * (PF_FpLong)c.green) + (0.11448 * (PF_FpLong)c.blue));
	if (c.alpha < PF_MAX_CHAN8) {
		v *= (PF_FpLong)c.alpha / PF_MAX_CHAN8;
	}
	if (v > PF_MAX_CHAN8) v = PF_MAX_CHAN8;

	A_u_char v2 = infoP->tbl[(A_long)v];

	c.alpha = PF_MAX_CHAN8;
	c.red = c.green = c.blue = v2;

	*outP = CONV8TO32(c);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
DrawCol8(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	//if (infoP->drawCol == FALSE) return err;

	A_long x = 0;
	A_long y = 0;
	PF_Pixel c = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
	for (A_long i = 0; i < infoP->colCount; i++)
	{
		c.red = c.green = c.blue = infoP->col[i];
		A_long pX0 = x * infoP->colWidth;
		A_long pY0 = y * infoP->colHeight;
		A_long pX1 = pX0 + infoP->colWidth;
		A_long pY1 = pY0 + infoP->colHeight;
		ae->out->boxFill8(pX0, pY0, pX1,pY1,c);
		x++;
		if (x >= 10) {
			x = 0;
			y++;
		}
	}
	return err;
}
static PF_Err
DrawCol16(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	//if (infoP->drawCol == FALSE) return err;

	A_long x = 0;
	A_long y = 0;
	PF_Pixel16 c = { PF_MAX_CHAN16,PF_MAX_CHAN16, PF_MAX_CHAN16, PF_MAX_CHAN16 };
	for (A_long i = 0; i < infoP->colCount; i++)
	{
		c.red = c.green = c.blue =  FS_CONVERT8TO16(infoP->col[i]);
		A_long pX0 = x * infoP->colWidth;
		A_long pY0 = y * infoP->colHeight;
		A_long pX1 = pX0 + infoP->colWidth;
		A_long pY1 = pY0 + infoP->colHeight;
		ae->out->boxFill16(pX0, pY0, pX1, pY1, c);
		x++;
		if (x >= 10) {
			x = 0;
			y++;
		}
	}
	return err;
}
static PF_Err
DrawCol32(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	//if (infoP->drawCol == FALSE) return err;

	A_long x = 0;
	A_long y = 0;
	PF_PixelFloat c = {1,1,1,1, };
	for (A_long i = 0; i < infoP->colCount; i++)
	{
		c.red = c.green = c.blue = (PF_FpShort)FS_CONVERT8TO32(infoP->col[i]);
		A_long pX0 = x * infoP->colWidth;
		A_long pY0 = y * infoP->colHeight;
		A_long pX1 = pX0 + infoP->colWidth;
		A_long pY1 = pY0 + infoP->colHeight;
		ae->out->boxFill32(pX0, pY0, pX1, pY1, c);
		x++;
		if (x >= 10) {
			x = 0;
			y++;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	A_long boders[3];

	ERR(ae->GetADD(ID_COUNT_LO,&boders[0]));
	ERR(ae->GetADD(ID_COUNT_MID, &boders[1]));
	ERR(ae->GetADD(ID_COUNT_HI, &boders[2]));
	ERR(ae->GetCHECKBOX(ID_DRAW_COL, &infoP->drawCol));

	A_long boderCount = 0;
	for (A_long i = 0; i < 3; i++)
	{
		boderCount += boders[i];
	}
	infoP->colCount = boderCount;
	A_u_char col[30];
	for (A_long i = 0; i < 30; i++)
	{
		col[i] = PF_MAX_CHAN8;
		infoP->col[i];
	}
	for (A_long i = 0; i < boderCount; i++)
	{
		double b = PF_MAX_CHAN8 * (double)i / (double)boderCount + 0.5;
		if (b > PF_MAX_CHAN8) b = PF_MAX_CHAN8;
		col[i] = (A_u_char)b;
		infoP->col[i] = col[i];
	}
	A_long v = 0;
	A_long bdrValue = (GBDR::LO - GBDR::ZERO) / boders[0];
	A_long cnt = 0;
	A_long cntMax = 0;
	if (boders[0] <= 1) {
		for (A_long i = GBDR::ZERO; i < GBDR::LO; i++)
		{
			infoP->tbl[i] = col[v];
		}
	}
	else {
		cnt = 0;
		cntMax = 0;
		for (A_long i = GBDR::ZERO; i < GBDR::LO; i++)
		{
			infoP->tbl[i] = col[v];
			cnt++;
			if (cnt >= bdrValue) {
				cntMax++;
				if (cntMax < boders[0]) {
					v++;
				}
				cnt = 0;
			}
		}
	}
	v++;
	bdrValue = (GBDR::HI - GBDR::LO) / boders[1];
	if (boders[1] <= 1) {
		for (A_long i = GBDR::LO; i < GBDR::HI; i++)
		{
			infoP->tbl[i] = col[v];
		}
	}
	else {
		cnt = 0;
		cntMax = 0;
		for (A_long i = GBDR::LO; i < GBDR::HI; i++)
		{
			infoP->tbl[i] = col[v];
			cnt++;
			if (cnt >= bdrValue) {
				cntMax++;
				if (cntMax < boders[1]) {
					v++;
				}
				cnt = 0;
			}
		}
	}
	v++;
	bdrValue = (GBDR::MAX - GBDR::HI) / boders[2];
	if (boders[2] <= 1) {
		for (A_long i = GBDR::HI; i < GBDR::MAX; i++)
		{
			infoP->tbl[i] = col[v];
		}
	}
	else {
		cnt = 0;
		cntMax = 0;
		for (A_long i = GBDR::HI; i < GBDR::MAX; i++)
		{
			infoP->tbl[i] = col[v];
			cnt++;
			if (cnt >= bdrValue) {
				cntMax++;
				if (cntMax < boders[2]) {
					v++;
				}
				cnt = 0;
			}
		}
	}

	

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	//ERR(ae->CopyInToOut());
	
	infoP->colWidth = ae->out->width() / 10;
	infoP->colHeight = ae->out->height() / 10;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,FilterImage32));
		if (infoP->drawCol == TRUE) {
			ERR(DrawCol32(ae, infoP));
		}
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,FilterImage16));
		if (infoP->drawCol == TRUE) {
			ERR(DrawCol16(ae, infoP));
		}
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
		if (infoP->drawCol == TRUE) {
			ERR(DrawCol8(ae, infoP));
		}
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
