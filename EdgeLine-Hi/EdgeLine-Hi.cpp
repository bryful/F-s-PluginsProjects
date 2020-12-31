//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "EdgeLine-Hi.h"


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
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(STR_TARGET_COLOR,
		0xFF,
		0xFF,
		0x00,
		ID_TARGET_COLOR
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(STR_SAMPLE_COLOR,
		0xFF,
		0x00,
		0x00,
		ID_SAMPLE_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LEVEL,	//Name
		0,						//VALID_MIN
		100,						//VALID_MAX
		0,						//SLIDER_MIN
		50,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LEVEL
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(STR_LENGTH,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		20,				//スライダーの最大値
		0,				//デフォルトの値
		ID_LENGTH
	);	
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ROT, 0, ID_ROT);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(STR_DRAW_COLOR,
		0xFF,
		0x00,
		0xFF,
		ID_DRAW_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_SCANFLAG,
		STR_SCANFLAG2,
		FALSE,
		0,
		ID_SCANFLAG
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
inline PF_Boolean compPix8E(PF_Pixel s, PF_Pixel d, A_u_char lv)
{
	return (( (F_ABS((A_long)s.red - (A_long)d.red)<= (A_long)lv))
		&& (F_ABS((A_long)s.green - (A_long)d.green) <= (A_long)lv)
		&& (F_ABS((A_long)s.blue - (A_long)d.blue) <= (A_long)lv));
}
//-------------------------------------------------------------------------------------------------
inline PF_Boolean compPix16E(PF_Pixel s, PF_Pixel16 d, A_u_char lv)
{
	PF_Pixel d2 = CONV16TO8(d);
	return (((F_ABS((A_long)s.red - (A_long)d2.red) <= (A_long)lv))
		&& (F_ABS((A_long)s.green - (A_long)d2.green) <= (A_long)lv)
		&& (F_ABS((A_long)s.blue - (A_long)d2.blue) <= (A_long)lv));
}
inline PF_Boolean compPix32E(PF_Pixel s, PF_PixelFloat d, A_u_char lv)
{
	PF_Pixel d2 = CONV32TO8(d);
	return (((F_ABS((A_long)s.red - (A_long)d2.red) <= (A_long)lv))
		&& (F_ABS((A_long)s.green - (A_long)d2.green) <= (A_long)lv)
		&& (F_ABS((A_long)s.blue - (A_long)d2.blue) <= (A_long)lv));
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
	CFsAE *ae;
	ae = infoP->ae;
	CFsGraph *in = ae->in;
	CFsGraph *out = ae->out;
	A_long w = in->width();
	A_long h = in->height();

	A_u_char lv = infoP->level;

	if (compPix8E(*inP, infoP->target_color, lv) == TRUE)
	{
		A_long x, xx, y, yy;
		xx = -65536; yy = -65536;
		for (int i = 0; i <= infoP->length; i++)
		{
			x = (A_long)((double)xL + infoP->xscan*(double)i+0.5);
			y = (A_long)((double)yL + infoP->yscan*(double)i+0.5);
			if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) break;
			if ((x == xx) && (y == yy)) continue;
			PF_Pixel c = in->getPixel8(x, y);
			if (compPix8E(c, infoP->sample_color,lv) == TRUE)
			{
				*outP = infoP->draw_color;
				break;
			}
			else if (infoP->GiveUpSoon == TRUE)
			{
				if (compPix8E(c, infoP->target_color, lv) == FALSE)
				{
					break;
				}
			}
			xx = x; yy = y;

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
	CFsAE *ae;
	ae = infoP->ae;
	CFsGraph *in = ae->in;
	CFsGraph *out = ae->out;
	A_long w = in->width();
	A_long h = in->height();

	A_u_char lv = infoP->level;

	if (compPix16E(infoP->target_color, *inP, lv) == TRUE)
	{
		A_long x, xx, y, yy;
		xx = -65536; yy = -65536;
		for (int i = 0; i <= infoP->length; i++)
		{
			x = (A_long)((double)xL + infoP->xscan*(double)i + 0.5);
			y = (A_long)((double)yL + infoP->yscan*(double)i + 0.5);
			if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) break;
			if ((x == xx) && (y == yy)) continue;
			PF_Pixel16 c = in->getPixel16(x, y);
			if (compPix16E( infoP->sample_color,c, lv) == TRUE)
			{
				*outP = infoP->draw_color16;
				break;
			}
			else if (infoP->GiveUpSoon == TRUE)
			{
				if (compPix16E(infoP->target_color, c,lv) == FALSE)
				{
					break;
				}
			}
			xx = x; yy = y;

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
	CFsAE *ae;
	ae = infoP->ae;
	CFsGraph *in = ae->in;
	CFsGraph *out = ae->out;
	A_long w = in->width();
	A_long h = in->height();

	A_u_char lv = infoP->level;

	if (compPix32E(infoP->target_color, *inP, lv) == TRUE)
	{
		A_long x, xx, y, yy;
		xx = -65536; yy = -65536;
		for (int i = 0; i <= infoP->length; i++)
		{
			x = (A_long)((double)xL + infoP->xscan*(double)i + 0.5);
			y = (A_long)((double)yL + infoP->yscan*(double)i + 0.5);
			if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) break;
			if ((x == xx) && (y == yy)) continue;
			PF_Pixel32 c = in->getPixel32(x, y);
			if (compPix32E(infoP->sample_color, c, lv) == TRUE)
			{
				*outP = infoP->draw_color32;
				break;
			}
			else if (infoP->GiveUpSoon == TRUE)
			{
				if (compPix32E(infoP->target_color, c, lv) == FALSE)
				{
					break;
				}
			}
			xx = x; yy = y;

		}
	}
	return err;


}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCOLOR(ID_TARGET_COLOR,&infoP->target_color));
	ERR(ae->GetCOLOR(ID_SAMPLE_COLOR,&infoP->sample_color));
	ERR(ae->GetADD(ID_LENGTH,&infoP->length));
	ERR(ae->GetANGLE(ID_ROT, &infoP->rot));

	CRotCalc rt(ae->in_data);
	rt.SetRotLength(infoP->rot, infoP->length <<16L);

	infoP->xscan = rt.xFLT()/ (double)infoP->length;
	infoP->yscan = rt.yFLT() / (double)infoP->length;

	double b;
	ERR(ae->GetFLOAT(ID_LEVEL, &b));
	infoP->level = (A_u_char)(PF_MAX_CHAN8 * b/100 + 0.5);

	ERR(ae->GetCOLOR(ID_DRAW_COLOR, &infoP->draw_color));

	infoP->draw_color16 = CONV8TO16(infoP->draw_color);
	infoP->draw_color32 = CONV8TO32(infoP->draw_color);

	ERR(ae->GetCHECKBOX(ID_SCANFLAG,&infoP->GiveUpSoon));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	infoP->ae = ae;
	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if ( infoP->length<=0)
	{
		return err;
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
