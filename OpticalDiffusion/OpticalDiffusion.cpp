//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "OpticalDiffusion.h"

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
	PF_ADD_CHECKBOX(STR_EXTRACT_ENABLED,
					"",
					TRUE,
					0,
					ID_EXTRACT_ENABLED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_EXTRACT_TOPIC, ID_EXTRACT_TOPIC);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_EXTRACT_BLACK_POINT,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					128,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					128,			//スライダーの最大値
					64,				//デフォルトの値
					ID_EXTRACT_BLACK_POINT
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_EXTRACT_WHITE_POINT,	//パラメータの名前
					100, 		//数値入力する場合の最小値
					255,			//数値入力する場合の最大値
					128,				//スライダーの最小値 
					255,			//スライダーの最大値
					128,				//デフォルトの値
					ID_EXTRACT_WHITE_POINT
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_EXTRACT_BLACK_SOFTNESS,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					255,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					128,			//スライダーの最大値
					128,				//デフォルトの値
					ID_EXTRACT_BLACK_SOFTNESS
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_EXTRACT_WHITE_SOFTNESS,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					255,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					64,			//スライダーの最大値
					64,				//デフォルトの値
					ID_EXTRACT_WHITE_SOFTNESS
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_EXTRACT_INVERT,
					"",
					TRUE,
					0,
					ID_EXTRACT_INVERT
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_EXTRACT_TOPIC_END);
	//**********************************
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_EXTRACT_COLOR_TOPIC, ID_EXTRACT_COLOR_TOPIC);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_EXTRACT_COLOR_COUNT,	//パラメータの名前
					0, 						//数値入力する場合の最小値
					8,						//数値入力する場合の最大値
					0,						//スライダーの最小値 
					8,						//スライダーの最大値
					0,						//デフォルトの値
					ID_EXTRACT_COL_COUNT
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_EXTRACT_COLOR_RANGE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					10,				//スライダーの最大値
					0.5,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_EXTRACT_COLOR_RANGE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL1, 
					0xFF,
					0x00,
					0x00,
					ID_EXTRACT_COL1
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL2, 
					0x00,
					0xFF,
					0x00,
					ID_EXTRACT_COL2
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL3, 
					0x00,
					0x00,
					0xFF,
					ID_EXTRACT_COL3
					);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL4, 
					0xFF,
					0xFF,
					0x00,
					ID_EXTRACT_COL4
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL5, 
					0xFF,
					0x00,
					0xFF,
					ID_EXTRACT_COL5
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL6, 
					0x00,
					0xFF,
					0xFF,
					ID_EXTRACT_COL6
					);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL7, 
					0xFF,
					0x80,
					0x00,
					ID_EXTRACT_COL7
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_EXTRACT_COL8, 
					0x00,
					0x80,
					0xFF,
					ID_EXTRACT_COL8
					);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_EXTRACT_COLOR_TOPIC_END);
	//**********************************
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_MINIMAX_1ST,	//パラメータの名前
					-100, 		//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-25,				//スライダーの最小値 
					25,			//スライダーの最大値
					-3,				//デフォルトの値
					ID_MINIMAX_1ST
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_MINIMAX_2ND,	//パラメータの名前
					-100, 		//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-25,				//スライダーの最小値 
					25,			//スライダーの最大値
					3,				//デフォルトの値
					ID_MINIMAX_2ND
					);
	//----------------------------------------------------------------
	PF_ADD_FLOAT_SLIDER(STR_BLUR,	//Name
						0,							//VALID_MIN
						255,						//VALID_MAX
						0,							//SLIDER_MIN
						50,							//SLIDER_MAX
						0,							//CURVE_TOLERANCE
						30,							//DFLT
						1,							//PREC
						0,							//DISP
						0,							//WANT_PHASE
						ID_BLUR
						);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_BLEND_MODE, 
						STR_BLEND_COUNT,	//メニューの数
						STR_BLEND_DFLT,	//デフォルト
						STR_BLEND_ITEMS,
						ID_BLEND_MODE
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_BLEND_OPACITY,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					70,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_BLEND_OPACITY
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_NOISE_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					10,				//スライダーの最大値
					1,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_NOISE_VALUE
					);
	//**********************************
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
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	PF_InData *in_data;
	in_data= ae->in_data;

	ERR(ae->GetCHECKBOX(ID_EXTRACT_ENABLED,&infoP->extractEnabled));
	ERR(ae->GetADD(ID_EXTRACT_BLACK_POINT,&infoP->blackPoint));
	ERR(ae->GetADD(ID_EXTRACT_WHITE_POINT,&infoP->whitePoint));
	ERR(ae->GetADD(ID_EXTRACT_BLACK_SOFTNESS,&infoP->blackSoftness));
	ERR(ae->GetADD(ID_EXTRACT_WHITE_SOFTNESS,&infoP->whiteSoftness));
	ERR(ae->GetCHECKBOX(ID_EXTRACT_INVERT,&infoP->invert));

	if (infoP->blackPoint>infoP->whitePoint){
		A_long tmp = infoP->blackPoint - infoP->whitePoint;
		infoP->blackPoint -= tmp;
		if (infoP->blackPoint<0) infoP->blackPoint = 0;
		infoP->blackSoftness -= tmp;
		if (infoP->blackSoftness<0) infoP->blackSoftness = 0;
	}

	ERR(ae->GetFIXED(ID_EXTRACT_COLOR_RANGE,&infoP->extract_color_Range));
	ERR(ae->GetADD(ID_EXTRACT_COL_COUNT,&infoP->extract_color_Count));
	PF_Pixel p;
	PF_PixelFloat p32;
	A_long cnt = infoP->extract_color_Count;
	for (int i=0; i<cnt; i++){
		ERR(ae->GetCOLOR(ID_EXTRACT_COL1+i,&p));
		p32 = CONV8TO32(p);
		infoP->extract_colors[i] = RGBtoHLS(p32);
	}

	ERR(ae->GetADD(ID_MINIMAX_1ST,&infoP->minimax1));
	ERR(ae->GetADD(ID_MINIMAX_2ND,&infoP->minimax2));
	ERR(ae->GetFLOAT(ID_BLUR,&infoP->blur));

	ERR(ae->GetPOPUP(ID_BLEND_MODE,&infoP->blendMode));
	ERR(ae->GetFIXED2FpShort(ID_BLEND_OPACITY,&infoP->blendOpacity));
	infoP->blendOpacity /= 100;
	if (infoP->blendOpacity<0) infoP->blendOpacity = 0;
	else if (infoP->blendOpacity>1) infoP->blendOpacity = 1;

	ERR(ae->GetFIXED2FpShort(ID_NOISE_VALUE,&infoP->noiseValue));
	infoP->noiseValue /= 100;
	if (infoP->noiseValue<0) infoP->noiseValue = 0;

	if (infoP->blendOpacity<=0){
		infoP->noiseValue = 0;
	}else if (infoP->blendOpacity<0.25){
		infoP->noiseValue = (PF_FpShort)(infoP->noiseValue * infoP->blendOpacity /0.25); 
	}

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if (infoP->blendOpacity <=0){
		ERR(ae->CopyInToOut());
		return err;
	}
	bool b = false;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(DF32(ae,infoP));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(DF16(ae,infoP));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(DF8(ae,infoP));
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
