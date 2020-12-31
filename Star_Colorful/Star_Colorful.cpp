//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Star_Colorful.h"


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
	PF_ADD_TOPIC(STR_TOPIC0, ID_TOPIC0);
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_TARGET_KIND,
		STR_TARGET_COUNT,	//メニューの数
		STR_TARGET_DFLT,	//デフォルト
		STR_TARGET_ITEMS,
		ID_TAGET_KIND
	);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_BORDER,	//Name
		50,						//VALID_MIN
		100,						//VALID_MAX
		50,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_BORDER
		);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TAGET_SOFT,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TAGET_SOFT
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COLOR,
		0xFF,
		0x00,
		0x00,
		ID_TARGET_COLOR
	);	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_MASK,
		STR_ON,
		FALSE,
		0,
		ID_TARGET_MASK
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC0_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_TOPICA, ID_TOPICA);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LENGTH,//Name
		0,						//VALID_MIN
		600,					//VALID_MAX
		0,						//SLIDER_MIN
		200,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LENGTH
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_OPACITY,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		20,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OPACITY
	);

	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ROT, 0, ID_ROT);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AUTOROLLING,
		STR_ON,
		FALSE,
		0,
		ID_AUTOROLLING
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_ROLLINGSPEED,//Name
		-720,						//VALID_MIN
		720,						//VALID_MAX
		-180,						//SLIDER_MIN
		180,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		90,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_ROLLINGDPEED
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPICA_END);


	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_TOPIC1, ID_TOPIC1);
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_COLOR_KIND,
		STR_COLOR_COUNT,	//メニューの数
		STR_COLOR_DFLT,	//デフォルト
		STR_COLOR_ITEMS,
		ID_COLOR_KIND
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COLOR,
		0xFF,
		0xFF,
		0x00,
		ID_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_BRIGHT,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		20,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		5,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_BRIGHTNESS
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_RAINBOW_OFFSET,	//Name
						-30000,						//VALID_MIN
						+30000,						//VALID_MAX
						-1,						//SLIDER_MIN
						1,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_RAINBOW_OFFSET
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_RAINBOW_DELTA,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						5,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						1,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_RAINBOW_DELTA
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC1_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_TOPIC2, ID_TOPIC2);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_VER_LEN,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		150,			//スライダーの最大値
		70,				//デフォルトの値
		ID_VER_LEN
	);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_VER_ROT, 0, ID_VER_ROT);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_VER_OPACITY,	//Name
						0,							//VALID_MIN
						100,						//VALID_MAX
						0,							//SLIDER_MIN
						100,						//SLIDER_MAX
						0,							//CURVE_TOLERANCE
						100,						//DFLT
						1,							//PREC
						0,							//DISP
						0,							//WANT_PHASE
						ID_VER_OPACITY
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_HOR_LEN,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		150,			//スライダーの最大値
		70,				//デフォルトの値
		ID_HOR_LEN
	);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_HOR_ROT, 90, ID_HOR_ROT);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HOR_OPACITY,	//Name
		0,							//VALID_MIN
		100,						//VALID_MAX
		0,							//SLIDER_MIN
		100,						//SLIDER_MAX
		0,							//CURVE_TOLERANCE
		100,							//DFLT
		1,							//PREC
		0,							//DISP
		0,							//WANT_PHASE
		ID_HOR_OPACITY
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_DIA_LEN,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		150,			//スライダーの最大値
		50,				//デフォルトの値
		ID_DIA_LEN
	);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_DIA_ROT, 45, ID_DIA_ROT);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_DIA_ROT2, -45, ID_DIA_ROT2);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_DIA_OPACITY,	//Name
		0,							//VALID_MIN
		100,						//VALID_MAX
		0,							//SLIDER_MIN
		100,						//SLIDER_MAX
		0,							//CURVE_TOLERANCE
		75,							//DFLT
		1,							//PREC
		0,							//DISP
		0,							//WANT_PHASE
		ID_DIA_OPACITY
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC2_END);

	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_BLEND,
		STR_BLEND_COUNT,	//メニューの数
		STR_BLEND_DFLT,	//デフォルト
		STR_BLEND_ITEMS,
		ID_BLEND
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

	ERR(ae->GetPOPUP(ID_TAGET_KIND, &infoP->target_kind));
	ERR(ae->GetFLOAT(ID_TAGET_BORDER, &infoP->target_border));
	infoP->target_border /= 100;
	ERR(ae->GetFLOAT(ID_TAGET_SOFT, &infoP->target_softness));
	infoP->target_softness /= 100;

	ERR(ae->GetCOLOR(ID_TARGET_COLOR, &infoP->target_color));
	ERR(ae->GetCHECKBOX(ID_TARGET_MASK, &infoP->target_maskDraw));

	ERR(ae->GetANGLE_FLT(ID_ROT, &infoP->rot));
	infoP->rot = CRot::RoundRot(infoP->rot);


	ERR(ae->GetCHECKBOX(ID_AUTOROLLING, &infoP->autoRolling));
	ERR(ae->GetFLOAT(ID_ROLLINGDPEED, &infoP->rollingSpeed));


	ERR(ae->GetFLOAT(ID_LENGTH,&infoP->length));
	infoP->length /= 100;
	ERR(ae->GetFLOAT(ID_OPACITY,&infoP->opacity));
	infoP->opacity /= 100;
	ERR(ae->GetFLOAT(ID_BRIGHTNESS,&infoP->brigthness));
	infoP->brigthness /= 100;

	ERR(ae->GetPOPUP(ID_COLOR_KIND, &infoP->color_kind));
	ERR(ae->GetCOLOR(ID_COLOR, &infoP->color));


	ERR(ae->GetFLOAT(ID_RAINBOW_OFFSET, &infoP->rainbowOffset));
	ERR(ae->GetFLOAT(ID_RAINBOW_DELTA, &infoP->rainbowDelta));
	infoP->rainbowDelta /= 100;
	infoP->rainbowDelta = ae->downSaleValue()* infoP->rainbowDelta;
	if (infoP->rainbowDelta == 0) infoP->rainbowDelta = 0.001;


	ERR(ae->GetADD(ID_VER_LEN,&infoP->verLength));
	infoP->verLength = ae->downScale(infoP->verLength);

	ERR(ae->GetANGLE_FLT(ID_VER_ROT, &infoP->verRot));
	infoP->verRot = CRot::RoundRot(infoP->verRot);

	ERR(ae->GetFLOAT(ID_VER_OPACITY,&infoP->verOpacity));
	infoP->verOpacity /= 100;

	ERR(ae->GetADD(ID_HOR_LEN, &infoP->horLength));
	infoP->horLength = ae->downScale(infoP->horLength);
	ERR(ae->GetANGLE_FLT(ID_HOR_ROT, &infoP->horRot));
	infoP->horRot = CRot::RoundRot(infoP->horRot);

	ERR(ae->GetFLOAT(ID_HOR_OPACITY, &infoP->horOpacity));
	infoP->horOpacity /= 100;

	ERR(ae->GetADD(ID_DIA_LEN, &infoP->diaLength));
	infoP->diaLength = ae->downScale(infoP->diaLength);
	ERR(ae->GetANGLE_FLT(ID_DIA_ROT, &infoP->diaRot));
	infoP->diaRot = CRot::RoundRot(infoP->diaRot);
	ERR(ae->GetANGLE_FLT(ID_DIA_ROT2, &infoP->diaRot2));
	infoP->diaRot2 = CRot::RoundRot(infoP->diaRot2);

	ERR(ae->GetFLOAT(ID_DIA_OPACITY, &infoP->diaOpacity));
	infoP->diaOpacity /= 100;

	ERR(ae->GetPOPUP(ID_BLEND, &infoP->blend));


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
		ERR(StarExec32(ae, infoP));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(StarExec16(ae, infoP));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(StarExec8(ae, infoP));
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
