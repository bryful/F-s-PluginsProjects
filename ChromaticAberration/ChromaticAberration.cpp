//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ChromaticAberration.h"


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
	PF_ADD_FLOAT_SLIDER(STR_R,	//Name
		0,						//VALID_MIN
		150,					//VALID_MAX
		90,						//SLIDER_MIN
		110,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		98,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_R
		);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_G,	//Name
		0,						//VALID_MIN
		150,					//VALID_MAX
		90,						//SLIDER_MIN
		110,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_G
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_B,	//Name
		0,						//VALID_MIN
		150,					//VALID_MAX
		90,						//SLIDER_MIN
		110,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		102,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_B
	);

	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POINT(STR_CENTER,			/*"New Center"*/
				50,	// X
				50,	// Y
				0,	// Flag
				ID_CENTER
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

	PF_Pixel col = { 0,0,0,0 };
	A_u_char r = 0;
	A_u_char g = 0;
	A_u_char b = 0;
	A_u_char a = 0;
	PF_FpLong dx = (PF_FpLong)xL - infoP->cx;
	PF_FpLong dy = (PF_FpLong)yL - infoP->cy;


	// red
	PF_FpLong pdx = dx * infoP->r  + infoP->cx;
	PF_FpLong pdy = dy * infoP->r  + infoP->cy;

	col = infoP->ae->in->getPixelFloat8(pdx, pdy);
	r = col.red;
	a = col.alpha;

	pdx = dx * infoP->g  + infoP->cx;
	pdy = dy * infoP->g  + infoP->cy;
	col = infoP->ae->in->getPixelFloat8(pdx, pdy);
	g = col.green;
	a = RoundByteLong((A_long)a + (A_long)col.alpha - (A_long)a * (A_long)col.alpha/PF_MAX_CHAN8);

	pdx = dx * infoP->b  + infoP->cx;
	pdy = dy * infoP->b  + infoP->cy;
	col = infoP->ae->in->getPixelFloat8(pdx, pdy);
	b = col.blue;
	a = RoundByteLong((A_long)a + (A_long)col.alpha - (A_long)a * (A_long)col.alpha / PF_MAX_CHAN8);

	outP->red = r;
	outP->green = g;
	outP->blue = b;
	outP->alpha = a;

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
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 col = { 0,0,0,0 };
	A_u_short r = 0;
	A_u_short g = 0;
	A_u_short b = 0;
	A_u_short a = 0;
	PF_FpLong dx = (PF_FpLong)xL - infoP->cx;
	PF_FpLong dy = (PF_FpLong)yL - infoP->cy;


	// red
	PF_FpLong pdx = dx * infoP->r + infoP->cx;
	PF_FpLong pdy = dy * infoP->r + infoP->cy;

	col = infoP->ae->in->getPixelFloat16(pdx, pdy);
	r = col.red;
	a = col.alpha;

	pdx = dx * infoP->g + infoP->cx;
	pdy = dy * infoP->g + infoP->cy;
	col = infoP->ae->in->getPixelFloat16(pdx, pdy);
	g = col.green;
	a = RoundShort((A_long)a + (A_long)col.alpha - (A_long)a * (A_long)col.alpha/PF_MAX_CHAN16);

	pdx = dx * infoP->b + infoP->cx;
	pdy = dy * infoP->b + infoP->cy;
	col = infoP->ae->in->getPixelFloat16(pdx, pdy);
	b = col.blue;
	a = RoundShort((A_long)a + (A_long)col.alpha - (A_long)a * (A_long)col.alpha / PF_MAX_CHAN16);

	/*
		PF_MAX_CHAN16-((PF_MAX_CHAN16-a)*(PF_MAX_CHAN16-b))
		PF_MAX_CHAN16-PF_MAX_CHAN16*(PF_MAX_CHAN16-b) +a *(PF_MAX_CHAN16-b)
		PF_MAX_CHAN16-(PF_MAX_CHAN16*PF_MAX_CHAN16 - PF_MAX_CHAN16*b) + (a *PF_MAX_CHAN16 - a *b)
		
		PF_MAX_CHAN16-PF_MAX_CHAN16*PF_MAX_CHAN16 + PF_MAX_CHAN16*b + a *PF_MAX_CHAN16 - a *b
		PF_MAX_CHAN16(1-PF_MAX_CHAN16 + b + a - a *b/PF_MAX_CHAN16)





	*/

	outP->red = r;
	outP->green = g;
	outP->blue = b;
	outP->alpha = a;
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
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel32 col = { 0,0,0,0 };
	PF_FpShort r = 0;
	PF_FpShort g = 0;
	PF_FpShort b = 0;
	PF_FpShort a = 0;
	PF_FpLong dx = (PF_FpLong)xL - infoP->cx;
	PF_FpLong dy = (PF_FpLong)yL - infoP->cy;


	// red
	PF_FpLong pdx = dx * infoP->r + infoP->cx;
	PF_FpLong pdy = dy * infoP->r + infoP->cy;

	col = infoP->ae->in->getPixelFloat32(pdx, pdy);
	r = col.red;
	a = col.alpha;

	pdx = dx * infoP->g + infoP->cx;
	pdy = dy * infoP->g + infoP->cy;
	col = infoP->ae->in->getPixelFloat32(pdx, pdy);
	g = col.green;
	a = RoundFpShortDouble((double)a + (double)col.alpha - (double)a * (double)col.alpha);

	pdx = dx * infoP->b + infoP->cx;
	pdy = dy * infoP->b + infoP->cy;
	col = infoP->ae->in->getPixelFloat32(pdx, pdy);
	b = col.blue;
	a = RoundFpShortDouble((double)a + (double)col.alpha - (double)a * (double)col.alpha);

	outP->red = r;
	outP->green = g;
	outP->blue = b;
	outP->alpha = a;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	PF_FpLong f;
	ERR(ae->GetFLOAT(ID_R,&f));
	if (!err)
	{
		infoP->r = f/100;
	}
	ERR(ae->GetFLOAT(ID_G,&f));
	if (!err)
	{
		infoP->g = f / 100;
	}
	ERR(ae->GetFLOAT(ID_B,&f));
	if (!err)
	{
		infoP->b = f / 100;
	}


	PF_FixedPoint d;
	ERR(ae->GetFIXEDPOINT(ID_CENTER,&d));
	if (!err)
	{
		infoP->cx = (PF_FpLong)d.x / 65536.0;
		infoP->cy = (PF_FpLong)d.y / 65536.0;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;


	infoP->ae = ae;
	//画面をコピー

	ERR(ae->NewTmpWorld());
	ERR(ae->Copy(ae->input, ae->tmpP));

	
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ae->tmp->toBlackMat32();
		ERR(ae->iterate32TempToOutput((refconType)infoP, FilterImage32));
		ae->out->fromBlackMat32();
		break;
	case PF_PixelFormat_ARGB64:
		ae->tmp->toBlackMat16();
		ERR(ae->iterate16TempToOutput((refconType)infoP, FilterImage16));
		ae->out->fromBlackMat16();
		break;
	case PF_PixelFormat_ARGB32:
		ae->tmp->toBlackMat8();
		ERR(ae->iterate8TempToOutput((refconType)infoP,FilterImage8));
		ae->out->fromBlackMat8();
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
