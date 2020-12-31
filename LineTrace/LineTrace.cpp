//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "LineTrace.h"
#include <stdio.h>


static PF_Pixel			WHT8	= {PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8}; 
static PF_Pixel			BLK8	= {PF_MAX_CHAN8,0,0,0}; 
static PF_Pixel16		WHT16	= {PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16}; 
static PF_Pixel16		BLK16	= {PF_MAX_CHAN16,0,0,0}; 
static PF_PixelFloat	WHT32	= {1,1,1,1}; 
static PF_PixelFloat	BLK32	= {1,0,0,0}; 
//-----------------------------------------------------------------------------
static PF_Err 
ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;
	
	//-----------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ENABLED_BLACK,
					UI_ENABLED2,
					TRUE,
					0,
					enabled_black
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_BORDER_BLACK,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					border_black
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_DRAW_BLACK, 
					0x00,
					0x00,
					0x00,
					color_black
					);
	//-----------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ENABLED_RED,
					UI_ENABLED2,
					TRUE,
					0,
					enabled_red
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_BORDER_RED,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					border_red
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_DRAW_RED, 
					PF_MAX_CHAN8,
					0x00,
					0x00,
					color_red
					);

	//-----------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ENABLED_GREEN,
					UI_ENABLED2,
					TRUE,
					0,
					enabled_green
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_BORDER_GREEN,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					border_green
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_DRAW_GREEN, 
					0x00,
					PF_MAX_CHAN8,
					0x00,
					color_green
					);
	//-----------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ENABLED_BLUE,
					UI_ENABLED2,
					TRUE,
					0,
					enabled_blue
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_BORDER_BLUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					border_blue
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_DRAW_BLUE, 
					0x00,
					0x00,
					PF_MAX_CHAN8,
					color_blue
					);
	//-----------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ENABLED_YELLOW,
					UI_ENABLED2,
					TRUE,
					0,
					enabled_yellow
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_BORDER_YELLOW,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					border_yellow
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_DRAW_YELLOW, 
					PF_MAX_CHAN8,
					PF_MAX_CHAN8,
					0x00,
					color_yellow
					);
	//-----------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ENABLED_VIOLET,
					UI_ENABLED2,
					TRUE,
					0,
					enabled_violet
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_BORDER_VIOLET,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					border_violet
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_DRAW_VIOLET, 
					PF_MAX_CHAN8,
					0x00,
					PF_MAX_CHAN8,
					color_violet
					);


	out_data->num_params = ID_NUM_PARAMS;

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
	ParamInfo8 *	infoP		= reinterpret_cast<ParamInfo8*>(refcon);
	if (infoP){
		PF_Pixel p = *outP;
		PF_FpShort op = (PF_FpShort)p.alpha/PF_MAX_CHAN8;
		if(op>1) op=1;

		if (op<=0){
			p = WHT8;
		}else{
			if ( op<1) {
				PF_FpShort op1 = 1- op;
				p.blue	= RoundByteFpShort(p.blue * op + PF_MAX_CHAN8*op1);
				p.green	= RoundByteFpShort(p.green* op + PF_MAX_CHAN8*op1);
				p.blue	= RoundByteFpShort(p.blue * op + PF_MAX_CHAN8*op1);
			}
			PF_HLS_Pixel hls;
			ERR(infoP->cs->RGBtoHLS(infoP->effect_ref,&p,hls));

			if ( hls[HLS_L] >=TARGET_WHITE){
					p = WHT8;	
			}else if ( hls[HLS_L] < TARGET_BLACK){
				if ( infoP->ParamTable[LT_BLACK].enabled == TRUE){
					p = BLK8;	
				}else{
					p = WHT8;	
				}
			}else if ( hls[HLS_S] < TARGET_GRAY){
				if ( (infoP->ParamTable[LT_BLACK].border > hls[HLS_L])&&( infoP->ParamTable[LT_BLACK].enabled == TRUE)){
					p = BLK8;
				}else{
					p = WHT8;
				}
			}else{
				A_long H = (A_long)( (hls[HLS_H]) >>16);
				//赤
				if ( (H>=RANGE_RED2)||((H>=RANGE_RED0)&&(H<RANGE_RED1) )){
					if ( (infoP->ParamTable[LT_RED].border > hls[HLS_L])&&( infoP->ParamTable[LT_RED].enabled == TRUE)){
						p = infoP->ParamTable[LT_RED].color;
					}else{
						p = WHT8;
					}
				}else if ( (H>=TARGET_YELLOW0)&&(H<TARGET_YELLOW1) ){
					if ( (infoP->ParamTable[LT_YELLOW].border > hls[HLS_L])&&( infoP->ParamTable[LT_YELLOW].enabled == TRUE)){
						p = infoP->ParamTable[LT_YELLOW].color;
					}else{
						p = WHT8;
					}
				}else if ( (H>=TARGET_GREEN0)&&(H<TARGET_GREEN1) ){
					if ( (infoP->ParamTable[LT_GREEN].border > hls[HLS_L])&&(infoP->ParamTable[LT_GREEN].enabled == TRUE)){
						p = infoP->ParamTable[LT_GREEN].color;
					}else{
						p = WHT8;
					}
				}else if ( (H>=TARGET_BLUE0)&&(H<TARGET_BLUE1) ){
					if ( (infoP->ParamTable[LT_BLUE].border > hls[HLS_L])&&(infoP->ParamTable[LT_BLUE].enabled == TRUE)){
						p = infoP->ParamTable[LT_BLUE].color;;
					}else{
						p = WHT8;
					}
				}else if ( (H>=TARGET_VIOLET0)&&(H<TARGET_VIOLET1) ){
					if ( (infoP->ParamTable[LT_VIOLET].border > hls[HLS_L])&&( infoP->ParamTable[LT_VIOLET].enabled == TRUE)){
						p = infoP->ParamTable[LT_VIOLET].color;;
					}else{
						p = WHT8;
					}
				}else{
					p = WHT8;
				}
			}
			
		}
		*outP = p;


	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo16 *	infoP		= reinterpret_cast<ParamInfo16*>(refcon);
	if (infoP){
		PF_Pixel16 p = *outP;
		PF_FpShort op = (PF_FpShort)p.alpha/PF_MAX_CHAN16;
		if(op>1) op=1;

		if (op<=0){
			p = WHT16;
		}else{
			if ( op<1) {
				PF_FpShort op1 = 1- op;
				p.blue	= RoundShortFpLong(p.blue * op + PF_MAX_CHAN16*op1);
				p.green	= RoundShortFpLong(p.green* op + PF_MAX_CHAN16*op1);
				p.blue	= RoundShortFpLong(p.blue * op + PF_MAX_CHAN16*op1);
			}
			PF_HLS_Pixel hls;
			ERR(infoP->cs->RGBtoHLS(infoP->effect_ref,&p,hls));

			if ( hls[HLS_L] >=TARGET_WHITE){
					p = WHT16;	
			}else if ( hls[HLS_L] < TARGET_BLACK){
				if ( infoP->ParamTable[LT_BLACK].enabled == TRUE){
					p = BLK16;	
				}else{
					p = WHT16;	
				}
			}else if ( hls[HLS_S] < TARGET_GRAY){
				if ( (infoP->ParamTable[LT_BLACK].border > hls[HLS_L])&&( infoP->ParamTable[LT_BLACK].enabled == TRUE)){
					p = BLK16;
				}else{
					p = WHT16;
				}
			}else{
				A_long H = (A_long)( (hls[HLS_H]) >>16);
				//赤
				if ( (H>=RANGE_RED2)||((H>=RANGE_RED0)&&(H<RANGE_RED1) )){
					if ( (infoP->ParamTable[LT_RED].border > hls[HLS_L])&&( infoP->ParamTable[LT_RED].enabled == TRUE)){
						p = infoP->ParamTable[LT_RED].color;
					}else{
						p = WHT16;
					}
				}else if ( (H>=TARGET_YELLOW0)&&(H<TARGET_YELLOW1) ){
					if ( (infoP->ParamTable[LT_YELLOW].border > hls[HLS_L])&&( infoP->ParamTable[LT_YELLOW].enabled == TRUE)){
						p = infoP->ParamTable[LT_YELLOW].color;
					}else{
						p = WHT16;
					}
				}else if ( (H>=TARGET_GREEN0)&&(H<TARGET_GREEN1) ){
					if ( (infoP->ParamTable[LT_GREEN].border > hls[HLS_L])&&(infoP->ParamTable[LT_GREEN].enabled == TRUE)){
						p = infoP->ParamTable[LT_GREEN].color;
					}else{
						p = WHT16;
					}
				}else if ( (H>=TARGET_BLUE0)&&(H<TARGET_BLUE1) ){
					if ( (infoP->ParamTable[LT_BLUE].border > hls[HLS_L])&&(infoP->ParamTable[LT_BLUE].enabled == TRUE)){
						p = infoP->ParamTable[LT_BLUE].color;;
					}else{
						p = WHT16;
					}
				}else if ( (H>=TARGET_VIOLET0)&&(H<TARGET_VIOLET1) ){
					if ( (infoP->ParamTable[LT_VIOLET].border > hls[HLS_L])&&( infoP->ParamTable[LT_VIOLET].enabled == TRUE)){
						p = infoP->ParamTable[LT_VIOLET].color;;
					}else{
						p = WHT16;
					}
				}else{
					p = WHT16;
				}
			}
			
		}
		*outP = p;


	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	infoP		= reinterpret_cast<ParamInfo32*>(refcon);
	if (infoP){
		PF_PixelFloat p = *outP;
		PF_FpShort op = (PF_FpShort)p.alpha;
		if(op>1) op=1;

		if (op<=0){
			p = WHT32;
		}else{
			if ( op<1) {
				PF_FpShort op1 = 1- op;
				p.blue	= RoundFpShortDouble(p.blue * op + op1);
				p.green	= RoundFpShortDouble(p.green* op + op1);
				p.blue	= RoundFpShortDouble(p.blue * op + op1);
			}
			PF_HLS_Pixel hls;
			ERR(infoP->cs->RGBtoHLS(infoP->effect_ref,&p,hls));

			if ( hls[HLS_L] >=TARGET_WHITE){
					p = WHT32;	
			}else if ( hls[HLS_L] < TARGET_BLACK){
				if ( infoP->ParamTable[LT_BLACK].enabled == TRUE){
					p = BLK32;	
				}else{
					p = WHT32;	
				}
			}else if ( hls[HLS_S] < TARGET_GRAY){
				if ( (infoP->ParamTable[LT_BLACK].border > hls[HLS_L])&&( infoP->ParamTable[LT_BLACK].enabled == TRUE)){
					p = BLK32;
				}else{
					p = WHT32;
				}
			}else{
				A_long H = (A_long)( (hls[HLS_H]) >>16);
				//赤
				if ( (H>=RANGE_RED2)||((H>=RANGE_RED0)&&(H<RANGE_RED1) )){
					if ( (infoP->ParamTable[LT_RED].border > hls[HLS_L])&&( infoP->ParamTable[LT_RED].enabled == TRUE)){
						p = infoP->ParamTable[LT_RED].color;
					}else{
						p = WHT32;
					}
				}else if ( (H>=TARGET_YELLOW0)&&(H<TARGET_YELLOW1) ){
					if ( (infoP->ParamTable[LT_YELLOW].border > hls[HLS_L])&&( infoP->ParamTable[LT_YELLOW].enabled == TRUE)){
						p = infoP->ParamTable[LT_YELLOW].color;
					}else{
						p = WHT32;
					}
				}else if ( (H>=TARGET_GREEN0)&&(H<TARGET_GREEN1) ){
					if ( (infoP->ParamTable[LT_GREEN].border > hls[HLS_L])&&(infoP->ParamTable[LT_GREEN].enabled == TRUE)){
						p = infoP->ParamTable[LT_GREEN].color;
					}else{
						p = WHT32;
					}
				}else if ( (H>=TARGET_BLUE0)&&(H<TARGET_BLUE1) ){
					if ( (infoP->ParamTable[LT_BLUE].border > hls[HLS_L])&&(infoP->ParamTable[LT_BLUE].enabled == TRUE)){
						p = infoP->ParamTable[LT_BLUE].color;;
					}else{
						p = WHT32;
					}
				}else if ( (H>=TARGET_VIOLET0)&&(H<TARGET_VIOLET1) ){
					if ( (infoP->ParamTable[LT_VIOLET].border > hls[HLS_L])&&( infoP->ParamTable[LT_VIOLET].enabled == TRUE)){
						p = infoP->ParamTable[LT_VIOLET].color;;
					}else{
						p = WHT32;
					}
				}else{
					p = WHT32;
				}
			}
			
		}
		*outP = p;


	}
	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	for ( A_long i=0; i<6; i++){
		A_long idx = enabled_black + i*3;
		
		PF_Boolean b;
		PF_Fixed f;
		PF_Pixel p;
		ERR(ae->GetCHECKBOX(idx+0,&b));
		ERR(ae->GetFIXED_PER2ONE(idx+1,&f));
		ERR(ae->GetCOLOR(idx+2,&p));
		if (!err){
			infoP->ParamTable[i].enabled	= b;
			infoP->ParamTable[i].border		= f;
			infoP->ParamTable[i].color		= p;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_Err	err2 = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ParamInfo32 info32;
		for (A_long i=0; i<LT_COUNT; i++){
			info32.ParamTable[i].enabled = infoP->ParamTable[i].enabled;
			info32.ParamTable[i].border = infoP->ParamTable[i].border;
			info32.ParamTable[i].color = CONV8TO32( infoP->ParamTable[i].color);
		}
		info32.cs = NULL;
		info32.effect_ref = ae->in_data->effect_ref;
		
		ERR(AEFX_AcquireSuite(
			ae->in_data, 
			ae->out_data,
			kPFColorCallbacksFloatSuite,
			kPFColorCallbacksFloatSuiteVersion1,
			"Couldn't load suite.",
			(void**)&(info32.cs)));
		if (!err){
			if (info32.cs != NULL){
				ERR(ae->iterate32((refconType)&info32,FilterImage32));
		
				ERR2(AEFX_ReleaseSuite(
					ae->in_data,
					ae->out_data,
					kPFColorCallbacksFloatSuite, 
					kPFColorCallbacksFloatSuiteVersion1, 
					"Couldn't release suite."));
			}
		}
		break;
	case PF_PixelFormat_ARGB64:
		ParamInfo16 info16;
		for (A_long i=0; i<LT_COUNT; i++){
			info16.ParamTable[i].enabled = infoP->ParamTable[i].enabled;
			info16.ParamTable[i].border = infoP->ParamTable[i].border;
			info16.ParamTable[i].color = CONV8TO16( infoP->ParamTable[i].color);
		}
		info16.cs = NULL;
		info16.effect_ref = ae->in_data->effect_ref;
		
		ERR(AEFX_AcquireSuite(
			ae->in_data, 
			ae->out_data,
			kPFColorCallbacks16Suite,
			kPFColorCallbacks16SuiteVersion1,
			"Couldn't load suite.",
			(void**)&(info16.cs)));
		if (!err){
			if (info16.cs != NULL){
				ERR(ae->iterate16((refconType)&info16,FilterImage16));
		
				ERR2(AEFX_ReleaseSuite(
					ae->in_data,
					ae->out_data,
					kPFColorCallbacks16Suite, 
					kPFColorCallbacks16SuiteVersion1, 
					"Couldn't release suite."));
			}
		}
		break;
	case PF_PixelFormat_ARGB32:
		ParamInfo8 info8;
		for (A_long i=0; i<LT_COUNT; i++){
			info8.ParamTable[i].enabled = infoP->ParamTable[i].enabled;
			info8.ParamTable[i].border = infoP->ParamTable[i].border;
			info8.ParamTable[i].color = infoP->ParamTable[i].color;
		}
		info8.cs = NULL;
		info8.effect_ref = ae->in_data->effect_ref;
		
		ERR(AEFX_AcquireSuite(
			ae->in_data, 
			ae->out_data,
			kPFColorCallbacksSuite,
			kPFColorCallbacksSuiteVersion1,
			"Couldn't load suite.",
			(void**)&(info8.cs)));
		if (!err){
			if (info8.cs != NULL){
				ERR(ae->iterate8((refconType)&info8,FilterImage8));
		
				ERR2(AEFX_ReleaseSuite(
					ae->in_data,
					ae->out_data,
					kPFColorCallbacksSuite, 
					kPFColorCallbacksSuiteVersion1, 
					"Couldn't release suite."));
			}
		}
		break;
	}
	return err;
}

//-----------------------------------------------------------------------------

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
		ERR(ae.CopyInToOut());
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

//-----------------------------------------------------------------------------
#include "Fs_Entry.h"