//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "smokeThreshold.h"


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
	PF_ADD_FLOAT_SLIDER(STR_ALPHA,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						25,						//SLIDER_MIN
						75,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_ALPHA
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_HI_E,
					STR_ON,
					TRUE,
					0,
					ID_HI_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_SDW1_E,
					STR_ON,
					TRUE,
					0,
					ID_SDW1_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_SDW2_E,
					STR_ON,
					TRUE,
					0,
					ID_SDW2_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HI,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						60,						//SLIDER_MIN
						100,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						80,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_HI
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_SDW1,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						20,						//SLIDER_MIN
						60,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						40,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_SDW1
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_SDW2,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						40,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						20,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_SDW2
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_HI_COL, 
					0xF0,
					0xF0,
					0xF0,
					ID_HI_COL
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_NOR_COL, 
					0x90,
					0x90,
					0x90,
					ID_NOR_COL
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_SDW1_COL, 
					0x50,
					0x50,
					0x50,
					ID_SDW1_COL
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_SDW2_COL, 
					0x30,
					0x30,
					0x30,
					ID_SDW2_COL
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_OUTLINE, ID_LINE_TOPIC);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_LINE_WEIGHT,	//パラメータの名前
					1, 				//数値入力する場合の最小値
					5,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					5,			//スライダーの最大値
					2,				//デフォルトの値
					ID_LINE_WEIGHT
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_NOR_LINE_E,
					STR_ON,
					TRUE,
					0,
					ID_NOR_LINE_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_SDW1_LINE_E,
					STR_ON,
					TRUE,
					0,
					ID_SDW1_LINE_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_SDW2_LINE_E,
					STR_ON,
					TRUE,
					0,
					ID_SDW2_LINE_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_NOR_LINE, 
					0x50,
					0x50,
					0x50,
					ID_NOR_LINE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_SDW1_LINE_E, 
					0x30,
					0x30,
					0x30,
					ID_SDW1_LINE
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_SDW2_LINE_E, 
					0x20,
					0x20,
					0x20,
					ID_SDW2_LINE
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_LINE_TOPIC_END);
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
alpha8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	niP		= reinterpret_cast<ParamInfo8*>(refcon);

	A_long v = (A_long)( ( 0.29891 * (double)outP->red) + ( 0.58661 * (double)outP->green) + ( 0.11448 * (double)outP->blue)+0.5);

	if ( outP->alpha >=niP->alpha){
		outP->red = 
		outP->blue = 
		outP->alpha = PF_MAX_CHAN8;
	}else{
		outP->red = 
		outP->blue = 
		outP->alpha = 0;
	}
	if (outP->alpha == 0){
		outP->green = 0;
	}else{
		if ((niP->h_on==TRUE)&&(v>=niP->hi)){
			outP->green = 2;
		}else if ((niP->s2_on==TRUE)&&(v<=niP->sdw2)){
			outP->green = 8;
		}else if ((niP->s1_on==TRUE)&&(v<=niP->sdw1)){
			outP->green = 6;
		}else{
			outP->green = 4;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
alpha16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP		= reinterpret_cast<ParamInfo16*>(refcon);

	A_long v = (A_long)( ( 0.29891 * (double)outP->red) + ( 0.58661 * (double)outP->green) + ( 0.11448 * (double)outP->blue)+0.5);

	if ( outP->alpha >=niP->alpha){
		outP->red = 
		outP->blue = 
		outP->alpha = PF_MAX_CHAN16;
	}else{
		outP->red = 
		outP->blue = 
		outP->alpha = 0;
	}
	if (outP->alpha == 0){
		outP->green = 0;
	}else{
		if ((niP->h_on==TRUE)&&(v>=niP->hi)){
			outP->green = 2;
		}else if ((niP->s2_on==TRUE)&&(v<=niP->sdw2)){
			outP->green = 8;
		}else if ((niP->s1_on==TRUE)&&(v<=niP->sdw1)){
			outP->green = 6;
		}else{
			outP->green = 4;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
alpha32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);

	PF_FpShort v = (PF_FpShort)( ( 0.29891 * outP->red) + ( 0.58661 * outP->green) + ( 0.11448 * outP->blue));

	if ( outP->alpha >=niP->alpha){
		outP->red = 
		outP->blue = 
		outP->alpha = 1.0;
	}else{
		outP->red = 
		outP->blue = 
		outP->alpha = 0;
	}
	if (outP->alpha == 0){
		outP->green = 0;
	}else{
		if ((niP->h_on==TRUE)&&(v>=niP->hi)){
			outP->green = 2;
		}else if ((niP->s2_on==TRUE)&&(v<=niP->sdw2)){
			outP->green = 8;
		}else if ((niP->s1_on==TRUE)&&(v<=niP->sdw1)){
			outP->green = 6;
		}else{
			outP->green = 4;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
outline8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo8 *	niP		= reinterpret_cast<ParamInfo8*>(refcon);

	bool ol = ((outP->red - outP->alpha)>0); 
	A_u_char a = 0 , g = 0;
	a = outP->red;
	g = outP->green;
	if (a>0){
		if (g == 2){
			*outP = niP->hiCol;
		}else if(g == 6){
			if (ol && niP->s1l_on){
				*outP = niP->sdw1Line;
			}else{
				*outP = niP->sdw1Col;
			}
		}else if(g == 8){
			if (ol && niP->s2l_on){
				*outP = niP->sdw2Line;
			}else{
				*outP = niP->sdw2Col;
			}
		}else{
			if (ol && niP->nl_on){
				*outP = niP->norLine;
			}else{
				*outP = niP->norCol;
			}
		}
	}
	outP->alpha = a;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
outline16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	niP		= reinterpret_cast<ParamInfo16*>(refcon);

	bool ol = ((outP->red - outP->alpha)>0); 
	A_u_short a = 0 , g = 0;
	a = outP->red;
	g = outP->green;
	if (a>0){
		if (g == 2){
			*outP = niP->hiCol;
		}else if(g == 6){
			if (ol && niP->s1l_on){
				*outP = niP->sdw1Line;
			}else{
				*outP = niP->sdw1Col;
			}
		}else if(g == 8){
			if (ol && niP->s2l_on){
				*outP = niP->sdw2Line;
			}else{
				*outP = niP->sdw2Col;
			}
		}else{
			if (ol && niP->nl_on){
				*outP = niP->norLine;
			}else{
				*outP = niP->norCol;
			}
		}
	}
	outP->alpha = a;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
outline32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);

	bool ol = ((outP->red - outP->alpha)>0); 
	PF_FpShort a = 0 , g = 0;
	a = outP->red;
	g = outP->green;
	if (a>0){
		if (g == 2){
			*outP = niP->hiCol;
		}else if(g == 6){
			if (ol && niP->s1l_on){
				*outP = niP->sdw1Line;
			}else{
				*outP = niP->sdw1Col;
			}
		}else if(g == 8){
			if (ol && niP->s2l_on){
				*outP = niP->sdw2Line;
			}else{
				*outP = niP->sdw2Col;
			}
		}else{
			if (ol && niP->nl_on){
				*outP = niP->norLine;
			}else{
				*outP = niP->norCol;
			}
		}
	}
	outP->alpha = a;
	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_ALPHA,&infoP->alpha));
	infoP->alpha /= 100.0;


	ERR(ae->GetCHECKBOX(ID_HI_ENABLED,&infoP->h_on));
	ERR(ae->GetCHECKBOX(ID_SDW1_ENABLED,&infoP->s1_on));
	ERR(ae->GetCHECKBOX(ID_SDW2_ENABLED,&infoP->s2_on));

	ERR(ae->GetCHECKBOX(ID_NOR_LINE_ENABLED,&infoP->nl_on));
	ERR(ae->GetCHECKBOX(ID_SDW1_LINE_ENABLED,&infoP->s1l_on));
	ERR(ae->GetCHECKBOX(ID_SDW2_LINE_ENABLED,&infoP->s2l_on));

	ERR(ae->GetFLOAT(ID_HI,&infoP->hi));
	infoP->hi /= 100.0;
	ERR(ae->GetFLOAT(ID_SDW1,&infoP->sdw1));
	infoP->sdw1 /= 100.0;
	ERR(ae->GetFLOAT(ID_SDW2,&infoP->sdw2));
	infoP->sdw2 /= 100.0;


	ERR(ae->GetCOLOR(ID_HI_COL,&infoP->hiCol));
	ERR(ae->GetCOLOR(ID_NOR_COL,&infoP->norCol));
	ERR(ae->GetCOLOR(ID_SDW1_COL,&infoP->sdw1Col));
	ERR(ae->GetCOLOR(ID_SDW2_COL,&infoP->sdw2Col));

	ERR(ae->GetADD(ID_LINE_WEIGHT,&infoP->lineWeight));

	ERR(ae->GetCOLOR(ID_NOR_LINE,&infoP->norLine));
	ERR(ae->GetCOLOR(ID_SDW1_LINE,&infoP->sdw1Line));
	ERR(ae->GetCOLOR(ID_SDW2_LINE,&infoP->sdw2Line));

	return err;
}
//-------------------------------------------------------------------------------------------------
static ParamInfo8 toInfo8(ParamInfo *infoP)
{
	ParamInfo8 ret;
	ret.h_on	= infoP->h_on;
	ret.s1_on	= infoP->s1_on;
	ret.s2_on	= infoP->s2_on;

	ret.nl_on	= infoP->nl_on;
	ret.s1l_on	= infoP->s1l_on;
	ret.s2l_on	= infoP->s2l_on;

	ret.lineWeight = infoP->lineWeight;

	ret.alpha	= RoundByteFpLong((double)PF_MAX_CHAN8 * infoP->alpha + 0.5);

	ret.hi		= RoundByteFpLong((double)PF_MAX_CHAN8 * infoP->hi + 0.5);
	ret.sdw1	= RoundByteFpLong((double)PF_MAX_CHAN8 * infoP->sdw1 + 0.5);
	ret.sdw2	= RoundByteFpLong((double)PF_MAX_CHAN8 * infoP->sdw2 + 0.5);

	ret.hiCol	= infoP->hiCol;
	ret.norCol	= infoP->norCol;
	ret.sdw1Col	= infoP->sdw1Col;
	ret.sdw2Col	= infoP->sdw2Col;

	ret.norLine	= infoP->norLine;
	ret.sdw1Line= infoP->sdw1Line;
	ret.sdw2Line= infoP->sdw2Line;

	return ret;
}
//-------------------------------------------------------------------------------------------------
static ParamInfo16 toInfo16(ParamInfo *infoP)
{
	ParamInfo16 ret;
	ret.h_on	= infoP->h_on;
	ret.s1_on	= infoP->s1_on;
	ret.s2_on	= infoP->s2_on;

	ret.nl_on	= infoP->nl_on;
	ret.s1l_on	= infoP->s1l_on;
	ret.s2l_on	= infoP->s2l_on;

	ret.lineWeight = infoP->lineWeight;

	ret.alpha	= RoundShortFpLong((double)PF_MAX_CHAN16 * infoP->alpha + 0.5);

	ret.hi		= RoundShortFpLong((double)PF_MAX_CHAN16 * infoP->hi + 0.5);
	ret.sdw1	= RoundShortFpLong((double)PF_MAX_CHAN16 * infoP->sdw1 + 0.5);
	ret.sdw2	= RoundShortFpLong((double)PF_MAX_CHAN16 * infoP->sdw2 + 0.5);

	ret.hiCol	= CONV8TO16(infoP->hiCol);
	ret.norCol	= CONV8TO16(infoP->norCol);
	ret.sdw1Col	= CONV8TO16(infoP->sdw1Col);
	ret.sdw2Col	= CONV8TO16(infoP->sdw2Col);

	ret.norLine	= CONV8TO16(infoP->norLine);
	ret.sdw1Line= CONV8TO16(infoP->sdw1Line);
	ret.sdw2Line= CONV8TO16(infoP->sdw2Line);

	return ret;
}
//-------------------------------------------------------------------------------------------------
static ParamInfo32 toInfo32(ParamInfo *infoP)
{
	ParamInfo32 ret;
	ret.h_on	= infoP->h_on;
	ret.s1_on	= infoP->s1_on;
	ret.s2_on	= infoP->s2_on;

	ret.nl_on	= infoP->nl_on;
	ret.s1l_on	= infoP->s1l_on;
	ret.s2l_on	= infoP->s2l_on;

	ret.lineWeight = infoP->lineWeight;

	ret.alpha	= RoundFpShortDouble(infoP->alpha);

	ret.hi		= RoundFpShortDouble(infoP->hi);
	ret.sdw1	= RoundFpShortDouble(infoP->sdw1);
	ret.sdw2	= RoundFpShortDouble(infoP->sdw2);

	ret.hiCol	= CONV8TO32(infoP->hiCol);
	ret.norCol	= CONV8TO32(infoP->norCol);
	ret.sdw1Col	= CONV8TO32(infoP->sdw1Col);
	ret.sdw2Col	= CONV8TO32(infoP->sdw2Col);

	ret.norLine	= CONV8TO32(infoP->norLine);
	ret.sdw1Line= CONV8TO32(infoP->sdw1Line);
	ret.sdw2Line= CONV8TO32(infoP->sdw2Line);

	return ret;
}//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	ParamInfo8 info8;
	ParamInfo16 info16;
	ParamInfo32 info32;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		info32 = toInfo32(infoP);
		ERR(ae->iterate32((refconType)&info32,alpha32));
		if ((infoP->nl_on)||(infoP->s1l_on)||(infoP->s2l_on)){
			ERR(ae->out->MinimaxA32(infoP->lineWeight*-1));
		}
		ERR(ae->iterate32((refconType)&info32,outline32));
		break;
	case PF_PixelFormat_ARGB64:
		info16 = toInfo16(infoP);
		ERR(ae->iterate16((refconType)&info16,alpha16));
		if ((infoP->nl_on)||(infoP->s1l_on)||(infoP->s2l_on)){
			ERR(ae->out->MinimaxA16(infoP->lineWeight*-1));
		}
		ERR(ae->iterate16((refconType)&info16,outline16));
		break;
	case PF_PixelFormat_ARGB32:
		info8 = toInfo8(infoP);
		ERR(ae->iterate8((refconType)&info8,alpha8));
		if ((infoP->nl_on)||(infoP->s1l_on)||(infoP->s2l_on)){
			ERR(ae->out->MinimaxA8(infoP->lineWeight*-1));
		}
		ERR(ae->iterate8((refconType)&info8,outline8));
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
