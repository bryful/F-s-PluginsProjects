//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "ShitsuKanBlur.h"


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
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_COL_TOPICS0, ID_COLOR_TOPICS0);
	//----------------------------------------------------------------
	//Color0
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR0,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR0,
		0xFF,
		0x00,
		0x00,
		ID_COL_COLOR0
	);
	//----------------------------------------------------------------
	//Color1
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR1,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR1,
		0x00,
		0xFF,
		0x00,
		ID_COL_COLOR1
	);
	//----------------------------------------------------------------
	//Color2
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR2,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR2,
		0x00,
		0x00,
		0xFF,
		ID_COL_COLOR2
	);
	//----------------------------------------------------------------
	//Color3
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR3,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR3
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR3,
		0xFF,
		0xFF,
		0x00,
		ID_COL_COLOR3
	);
	//----------------------------------------------------------------
	//Color4
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR4,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR4
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR4,
		0xFF,
		0x00,
		0x00,
		ID_COL_COLOR4
	);
	//----------------------------------------------------------------
	//Color5
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR5,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR5
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR5,
		0x00,
		0xFF,
		0x00,
		ID_COL_COLOR5
	);
	//----------------------------------------------------------------
	//Color6
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR6,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR6
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR6,
		0x00,
		0x00,
		0xFF,
		ID_COL_COLOR6
	);
	//----------------------------------------------------------------
	//Color7
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR7,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR7
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR7,
		0x00,
		0xFF,
		0xFF,
		ID_COL_COLOR7
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_COLOR_TOPICS_END0);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_COL_TOPICS1, ID_COLOR_TOPICS1);
	//----------------------------------------------------------------
	//Color0
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX0,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX0,
		0xFF,
		0x00,
		0x00,
		ID_COL_COLOR_EX0
	);
	//----------------------------------------------------------------
	//Color1
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX1,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX1,
		0x00,
		0xFF,
		0x00,
		ID_COL_COLOR_EX1
	);
	//----------------------------------------------------------------
	//Color2
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX2,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX2,
		0x00,
		0x00,
		0xFF,
		ID_COL_COLOR_EX2
	);
	//----------------------------------------------------------------
	//Color3
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX3,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX3
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX3,
		0xFF,
		0xFF,
		0x00,
		ID_COL_COLOR_EX3
	);
	//----------------------------------------------------------------
	//Color4
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX4,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX4
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX4,
		0xFF,
		0x00,
		0x00,
		ID_COL_COLOR_EX4
	);
	//----------------------------------------------------------------
	//Color5
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX5,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX5
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX5,
		0x00,
		0xFF,
		0x00,
		ID_COL_COLOR_EX5
	);
	//----------------------------------------------------------------
	//Color6
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX6,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX6
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX6,
		0x00,
		0x00,
		0xFF,
		ID_COL_COLOR_EX6
	);
	//----------------------------------------------------------------
	//Color7
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_COLOR_EX7,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_COLOR_EX7
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_COLOR_EX7,
		0x00,
		0xFF,
		0xFF,
		ID_COL_COLOR_EX7
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_COLOR_TOPICS_END1);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ANGLE, 0, ID_ANGLE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_HI_TOPICS, ID_HI_TOPICS);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(STR_FLT_HI_LENGTH,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		200,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		50,			//スライダーの最大値
		10,			//デフォルトの値
		1,				//数値表示に関するフラグ 
		0,
		0,
		ID_FLT_HI_LENGTH
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(STR_FLT_HI_BLUR,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		20,				//デフォルトの値
		ID_ADD_HI_BLUR
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_HILIGHT_LV,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,					//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		50,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_HILIGHT_LV
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_HILIGHT_R,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_HILIGHT_R
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_HILIGHT_G,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_HILIGHT_G
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_HILIGHT_B,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_HILIGHT_B
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_HI_TOPICS_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_SW_TOPICS, ID_SW_TOPICS);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(STR_FLT_SW_LENGTH,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		200,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		10,			//デフォルトの値
		1,				//数値表示に関するフラグ 
		0,
		0,
		ID_FLT_SW_LENGTH
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(STR_FLT_SW_BLUR,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		20,				//デフォルトの値
		ID_ADD_SW_BLUR
	);	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_SHADOW_LV,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		-50,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_SHADOW_LV
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_SHADOW_R,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_SHADOW_R
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_SHADOW_G,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_SHADOW_G
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLT_SHADOW_B,	//Name
		-200,					//VALID_MIN
		200,					//VALID_MAX
		-100,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_SHADOW_B
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_SW_TOPICS_END);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_NOISE,	//Name
		0,					//VALID_MIN
		50,					//VALID_MAX
		0,						//SLIDER_MIN
		5,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		1,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLT_NOISE
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_EDGE1,
		STR_CBX_EDGE2,
		FALSE,
		0,
		ID_CBX_EDGE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_PUP_DRAW,
		STR_PUP_COUNT,	//メニューの数
		STR_PUP_DFLT,	//デフォルト
		STR_PUP_ITEMS,
		ID_PUP_DRAW
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COL_MASKCOLOR,
		0xFF,
		0xFF,
		0xFF,
		ID_COL_MASKCOLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CBX_WHITE,
		STR_CBX_ON,
		FALSE,
		0,
		ID_CBX_WHITE
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
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	
	PF_Boolean b = FALSE;
	int cm = 0;
	infoP->Color_Max = cm;
	infoP->EXColor_Max = cm;
	int idx = ID_CBX_COLOR0;
	for (int i = 0; i < 8; i++) {
		
		ERR(ae->GetCHECKBOX(idx, &b));
		idx++;
		if (b == TRUE) {
			ERR(ae->GetCOLOR(idx, &infoP->Color[cm]));
			cm++;
		}
		idx++;
	}
	infoP->Color_Max = cm;
	cm = 0;
	idx = ID_CBX_COLOR_EX0;
	for (int i = 0; i < 8; i++) {

		ERR(ae->GetCHECKBOX(idx, &b));
		idx++;
		if (b == TRUE) {
			ERR(ae->GetCOLOR(idx, &infoP->EXColor[cm]));
			cm++;
		}
		idx++;
	}
	infoP->EXColor_Max = cm;

	infoP->TagetCount = 0;
	
	ERR(ae->GetANGLE(ID_ANGLE, &infoP->angle));
	
	ERR(ae->GetFIXED(ID_FLT_HI_LENGTH, &infoP->hi_length));
	ERR(ae->GetADD(ID_ADD_HI_BLUR, &infoP->hi_blur));

	ERR(ae->GetFLOAT(ID_FLT_HILIGHT_LV,&infoP->hi_lv));
	infoP->hi_lv /= 100.0;
	ERR(ae->GetFLOAT(ID_FLT_HILIGHT_R, &infoP->hi_r));
	infoP->hi_r /= 100.0;
	ERR(ae->GetFLOAT(ID_FLT_HILIGHT_G, &infoP->hi_g));
	infoP->hi_g /= 100.0;
	ERR(ae->GetFLOAT(ID_FLT_HILIGHT_B, &infoP->hi_b));
	infoP->hi_b /= 100.0;

	ERR(ae->GetFIXED(ID_FLT_SW_LENGTH, &infoP->sw_length));
	ERR(ae->GetADD(ID_ADD_SW_BLUR, &infoP->sw_blur));

	ERR(ae->GetFLOAT(ID_FLT_SHADOW_LV, &infoP->sw_lv));
	infoP->sw_lv /= 100.0;
	ERR(ae->GetFLOAT(ID_FLT_SHADOW_R, &infoP->sw_r));
	infoP->sw_r /= 100.0;
	ERR(ae->GetFLOAT(ID_FLT_SHADOW_G, &infoP->sw_g));
	infoP->sw_g /= 100.0;
	ERR(ae->GetFLOAT(ID_FLT_SHADOW_B, &infoP->sw_b));
	infoP->sw_b /= 100.0;

	ERR(ae->GetFLOAT(ID_FLT_NOISE, &infoP->noise));
	infoP->noise /= 100.0;

	ERR(ae->GetCHECKBOX(ID_CBX_EDGE, &infoP->edge));

	ERR(ae->GetPOPUP(ID_PUP_DRAW, &infoP->draw));

	ERR(ae->GetCOLOR(ID_COL_MASKCOLOR, &infoP->MaskColor));
	ERR(ae->GetCHECKBOX(ID_CBX_WHITE, &infoP->white));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	switch (ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		SitsuKanSub32(ae, infoP);
		break;
	case PF_PixelFormat_ARGB64:
		SitsuKanSub16(ae, infoP);
		break;
	case PF_PixelFormat_ARGB32:
		SitsuKanSub8(ae, infoP);
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
