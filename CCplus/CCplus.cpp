//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------
#include "CCplus.h"


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
	PF_ADD_CHECKBOX(StrORG_REV1,
					StrORG_REV2,
					TRUE,
					0,
					ID_ORG_REV
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	StrCOLOR_START, 
					0xFF,
					0xFF,
					0xFF,
					ID_COLOR_START
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	StrCOLOR_CENTER, 
					0x80,
					0x80,
					0xFF,
					ID_COLOR_CENTER
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	StrCOLOR_END, 
					0x0,
					0x0,
					0xFF,
					ID_COLOR_END
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	StrCOLOR_CENTER_POS,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR_CENTER_POS
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	StrALPHA_START,	//パラメータの名前
					-200, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_ALPHA_START
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	StrALPHA_END,	//パラメータの名前
					-200, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_ALPHA_END
					);

//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	StrNOISE_VALUE,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					64,		//数値入力する場合の最大値
					0,		//スライダーの最小値 
					8,		//スライダーの最大値
					2,		//デフォルトの値
					ID_NOISE_VALUE
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
//---------------------------------------------------------------------------------------------
PF_Pixel colorValue8(CCPInfo *infoP, A_u_char value)
{
	PF_Pixel r;

	if ( value == infoP->color_border){
		r = infoP->color_center;
	}else if ( value ==0){
		r = infoP->color_start;
	}else if (value >= PF_MAX_CHAN8){
		r = infoP->color_end;
	}else if ( value <= infoP->color_border){
		A_long max = infoP->color_border;
		A_long v1 = max - value;
		A_long v2 = value;
		r.blue	= RoundByteLong(infoP->color_start.blue * v1/max + infoP->color_center.blue * v2/max);
		r.green = RoundByteLong(infoP->color_start.green * v1/max + infoP->color_center.green * v2/max);
		r.red	= RoundByteLong(infoP->color_start.red * v1/max + infoP->color_center.red * v2/max);
	}else{
		A_long max = PF_MAX_CHAN8 - infoP->color_border;
		A_long v1 = max - (value - infoP->color_border);
		A_long v2 = value - infoP->color_border;
		r.blue	= RoundByteLong(infoP->color_center.blue * v1/max + infoP->color_end.blue * v2/max);
		r.green = RoundByteLong(infoP->color_center.green * v1/max + infoP->color_end.green * v2/max);
		r.red	= RoundByteLong(infoP->color_center.red * v1/max + infoP->color_end.red * v2/max);
	}
	r.alpha = RoundByteLong(infoP->alpha_start + (infoP->alpha_end -infoP->alpha_start) * value /  PF_MAX_CHAN8);
	return r;
}
//---------------------------------------------------------------------------------------------
PF_Pixel16 colorValue16(CCPInfo16 *infoP, A_u_short value)
{
	PF_Pixel16 r;

	if ( value == infoP->color_border){
		r = infoP->color_center;
	}else if ( value ==0){
		r = infoP->color_start;
	}else if (value >= PF_MAX_CHAN16){
		r = infoP->color_end;
	}else if ( value <= infoP->color_border){
		A_long max = infoP->color_border;
		A_long v1 = max - value;
		A_long v2 = value;
		r.blue	= RoundShort(infoP->color_start.blue * v1/max + infoP->color_center.blue * v2/max);
		r.green = RoundShort(infoP->color_start.green * v1/max + infoP->color_center.green * v2/max);
		r.red	= RoundShort(infoP->color_start.red * v1/max + infoP->color_center.red * v2/max);
	}else{
		A_long max = PF_MAX_CHAN16 - infoP->color_border;
		A_long v1 = max - (value - infoP->color_border);
		A_long v2 = value - infoP->color_border;
		r.blue	= RoundShortFpLong(infoP->color_center.blue * v1/max + infoP->color_end.blue * v2/max);
		r.green = RoundShortFpLong(infoP->color_center.green * v1/max + infoP->color_end.green * v2/max);
		r.red	= RoundShortFpLong(infoP->color_center.red * v1/max + infoP->color_end.red * v2/max);
	}
	r.alpha = RoundShortFpLong(infoP->alpha_start + (infoP->alpha_end -infoP->alpha_start) * value /  PF_MAX_CHAN16);
	return r;
}
//---------------------------------------------------------------------------------------------
PF_PixelFloat colorValue32(CCPInfo32 *infoP, PF_FpShort value)
{
	PF_PixelFloat r;

	if ( value == infoP->color_border){
		r = infoP->color_center;
	}else if ( value ==0){
		r = infoP->color_start;
	}else if (value >= 1.0){
		r = infoP->color_end;
	}else if ( value <= infoP->color_border){
		PF_FpShort max = infoP->color_border;
		PF_FpShort v1 = max - value;
		PF_FpShort v2 = value;
		r.blue	= RoundFpShortDouble((infoP->color_start.blue * v1 + infoP->color_center.blue * v2)/max);
		r.green = RoundFpShortDouble((infoP->color_start.green * v1 + infoP->color_center.green * v2)/max);
		r.red	= RoundFpShortDouble((infoP->color_start.red * v1 + infoP->color_center.red * v2)/max);
	}else{
		PF_FpShort max = (PF_FpShort)(1.0 - infoP->color_border);
		PF_FpShort v1 = max - (value - infoP->color_border);
		PF_FpShort v2 = value - infoP->color_border;
		r.blue	= RoundFpShortDouble((infoP->color_center.blue * v1 + infoP->color_end.blue * v2)/max);
		r.green = RoundFpShortDouble((infoP->color_center.green * v1 + infoP->color_end.green * v2)/max);
		r.red	= RoundFpShortDouble((infoP->color_center.red * v1 + infoP->color_end.red * v2)/max);
	}
	r.alpha = RoundFpShortDouble(infoP->alpha_start + (infoP->alpha_end -infoP->alpha_start) * value);
	return r;
}
//---------------------------------------------------------------------------------------------
//カラーテーブルの作成
void makeColorTbl8(CCPInfo *infoP)
{
	for ( A_long i=0; i<PF_MAX_CHAN8; i++)
		infoP->colorTbl[i] = colorValue8(infoP,(A_u_char)i);
}
//---------------------------------------------------------------------------------------------
//カラーテーブルの作成
void makeColorTbl16(CCPInfo16 *infoP)
{
	for ( A_long i=0; i<PF_MAX_CHAN16; i++)
		infoP->colorTbl[i] = colorValue16(infoP,(A_u_short)i);
}

//-----------------------------------------------------------------------------------------------
static PF_Err 
AddColor8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	CCPInfo* info = reinterpret_cast<CCPInfo*>(refcon);
	if (info){
		A_long v;
		if (inP->alpha==0){
				v =0;
		}else{
			v = Pixel8Level(*inP) * inP->alpha / PF_MAX_CHAN8;
			if (v<0) v=0;
			if (v>PF_MAX_CHAN8) v = PF_MAX_CHAN8;
		}
		if (info->org_rev) v = PF_MAX_CHAN8 -v ;
		PF_Pixel p = colorValue8(info,(A_u_char)v);
		/*
		PF_Pixel p  =info->colorTbl[v];		
		*/
		
		outP->alpha = p.alpha;
		outP->red	= p.red;
		outP->green	= p.green;
		outP->blue	= p.blue;
	}

	return err;
}
//-----------------------------------------------------------------------------------------------
static PF_Err 
AddColor16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	CCPInfo16* info = reinterpret_cast<CCPInfo16*>(refcon);
	if (info){
		A_long v;
		if (inP->alpha==0){
				v =0;
		}else{
			v = Pixel16Level(*inP) * inP->alpha / PF_MAX_CHAN16;
			if (v<0) v=0;
			if (v>PF_MAX_CHAN16) v = PF_MAX_CHAN16;
		}
		if (info->org_rev) v = PF_MAX_CHAN16 -v ;
		*outP = colorValue16(info,(A_u_short)v);
		/*
		PF_Pixel16 p  = info->colorTbl[v];
		outP->alpha = p.alpha;
		outP->red	= p.red;
		outP->green	= p.green;
		outP->blue	= p.blue;
		*/
	}

	return err;
}
//-----------------------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err 
AddColor32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	CCPInfo32* infoP = reinterpret_cast<CCPInfo32*>(refcon);
	if (infoP){
		PF_FpShort v;
		if (inP->alpha==0){
				v =0;
		}else{
			v = Pixel32Level(*inP) * inP->alpha;
			if (v<0) v=0;
			else if (v>1) v=1;
		}
		if (infoP->org_rev) v = (PF_FpShort)1.0 -v ;

		*outP = colorValue32(infoP,v);

	}

	return err;
}
#endif
//-----------------------------------------------------------------------------------------------
static PF_Err 
AddNoise8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	CCPInfo* info = reinterpret_cast<CCPInfo*>(refcon);
	if (info){
		A_long v;
		if (outP->alpha>0){
			v = F_RAND2(-info->noise_value,info->noise_value);
			outP->red		= RoundByteLong(outP->red   + v);
			outP->green		= RoundByteLong(outP->green + v);
			outP->blue		= RoundByteLong(outP->blue  + v);
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------------------
static PF_Err 
AddNoise16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	CCPInfo16* info = reinterpret_cast<CCPInfo16*>(refcon);
	if (info){
		A_long v;
		if (outP->alpha>0){
			v = F_RAND2(-info->noise_value,info->noise_value);
			outP->red		= RoundShort(outP->red   + v);
			outP->green		= RoundShort(outP->green + v);
			outP->blue		= RoundShort(outP->blue  + v);
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err 
AddNoise32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	CCPInfo32* info = reinterpret_cast<CCPInfo32*>(refcon);
	if (info){
		PF_FpShort v;
		if (outP->alpha>0){
			v = (PF_FpShort)F_RAND2(-info->noise_value,info->noise_value) /PF_MAX_CHAN16;
			outP->red		= RoundFpShort(outP->red   + v);
			outP->green		= RoundFpShort(outP->green + v);
			outP->blue		= RoundFpShort(outP->blue  + v);
		}
	}

	return err;
}
#endif
//-------------------------------------------------------------------------------------------------
static CCPInfo16 to16(CCPInfo *prm)
{
	CCPInfo16 prm16;

	prm16.org_rev			= prm->org_rev;
	prm16.color_start		= CONV8TO16(prm->color_start);
	prm16.color_center		= CONV8TO16(prm->color_center);
	prm16.color_end			= CONV8TO16(prm->color_end);
	prm16.color_border		= FS_CONVERT8TO16( prm->color_border);
	prm16.alpha_start		= FS_CONVERT8TO16(prm->alpha_start);
	prm16.alpha_end			= FS_CONVERT8TO16(prm->alpha_end);


	prm16.noise_value	= FS_CONVERT8TO16(prm->noise_value);

	return prm16;
}
#if defined(SUPPORT_SMARTFX)
static CCPInfo32 to32(CCPInfo *prm)
{
	CCPInfo32 prm32;

	prm32.org_rev			= prm->org_rev;
	prm32.color_start		= CONV8TO32(prm->color_start);
	prm32.color_center		= CONV8TO32(prm->color_center);
	prm32.color_end			= CONV8TO32(prm->color_end);
	prm32.color_border		= (PF_FpShort)prm->color_border /PF_MAX_CHAN8;
	prm32.alpha_start		= (PF_FpShort)prm->alpha_start/PF_MAX_CHAN8;
	prm32.alpha_end			= (PF_FpShort)prm->alpha_end/PF_MAX_CHAN8;

	prm32.noise_value	= FS_CONVERT8TO16(prm->noise_value);

	return prm32;
}
#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, CCPInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_ORG_REV,&infoP->org_rev));
	infoP->org_rev = !infoP->org_rev;

	ERR(ae->GetCOLOR(ID_COLOR_START,&infoP->color_start));
	ERR(ae->GetCOLOR(ID_COLOR_CENTER,&infoP->color_center));
	ERR(ae->GetCOLOR(ID_COLOR_END,&infoP->color_end));
	
	PF_FpShort f;
	ERR(ae->GetFIXED2FpShort(ID_COLOR_CENTER_POS,&f));
	if (!err) {
		infoP->color_border = RoundByteFpLong(PF_MAX_CHAN8 * f /100);
	}else{
		infoP->color_border = PF_MAX_CHAN8/2;
	}

	ERR(ae->GetFIXED2FpShort(ID_ALPHA_START,&f));
	if (!err){
		f = PF_MAX_CHAN8 * f/100;
		if ( f<-510) f = -510;
		else if ( f>510) f = 510;
		infoP->alpha_start = (A_long)f;
	}else{
		infoP->alpha_start = PF_MAX_CHAN8;
	}
	ERR(ae->GetFIXED2FpShort(ID_ALPHA_END,&f));
	if (!err) {
		f = PF_MAX_CHAN8 * f/100;
		if ( f<-510) f = -510;
		else if ( f>510) f = 510;
		infoP->alpha_end = (A_long)f;
	}else{
		infoP->alpha_end = 0;
	}
	ERR(ae->GetADD(ID_NOISE_VALUE,&infoP->noise_value));
	infoP->colorTbl			= NULL;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , CCPInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ae->out->clear();
	if ( infoP->noise_value>0) F_SRAND(ae->frame());
	CFsBuffer buf(ae->in_data);
	switch(ae->pixelFormat())
	{
#if defined(SUPPORT_SMARTFX)
	case PF_PixelFormat_ARGB128:
		CCPInfo32 info32;
		info32 = to32(infoP);
		ERR(ae->iterate32((refconType)&info32,AddColor32));
		if(!err){
			if (info32.noise_value>0){
				ERR(ae->iterate32((refconType)&info32,AddNoise32));	
			}
		}
		break;
#endif
	case PF_PixelFormat_ARGB64:
		CCPInfo16 info16;
		info16 = to16(infoP);
		if (buf.Create(COLOR_TABLE_SIZE16) == PF_Err_NONE){
			info16.colorTbl	= buf.bufPixel16();
			makeColorTbl16(&info16);
			ERR(ae->iterate16((refconType)&info16,AddColor16));
			buf.Dispose();

			if(!err){
				if (info16.noise_value>0){
					ERR(ae->iterate16((refconType)&info16,AddNoise16));	
				}
			}
		}else{
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		}
		break;
	case PF_PixelFormat_ARGB32:
		if (buf.Create(COLOR_TABLE_SIZE8) ==PF_Err_NONE){
			infoP->colorTbl	= buf.bufPixel();
			makeColorTbl8(infoP);
			ERR(ae->iterate8((refconType)infoP,AddColor8));
			buf.Dispose();
			if(!err){
				if (infoP->noise_value>0){
					ERR(ae->iterate8((refconType)infoP,AddNoise8));	
				}
			}
		}else{
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		}
		break;
	default:
		err = PF_Err_BAD_CALLBACK_PARAM;
		break;
	}
	
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (
	PF_InData			*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		CCPInfo info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;

	return err;
}
//-------------------------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extraP,sizeof(CCPInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		CCPInfo *infoP = reinterpret_cast<CCPInfo*>(ae.LockPreRenderData());
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
//-------------------------------------------------------------------------------------------------
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
		CCPInfo *infoP = reinterpret_cast<CCPInfo*>(ae.LockPreRenderData());
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