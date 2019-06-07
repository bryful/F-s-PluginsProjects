//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Flare.h"
#include <stdio.h>


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

	int count = 0;

	char num[255] = { "\0" };
	for (int i = 0; i < PCOUNT; i++)
	{

#pragma region topic
//topic
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d",STR_TOPIC, i + 1);
#else
		sprintf(num, "%s%d", STR_TOPIC, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_TOPIC(num, PIDX(0)+ ID_TOPIC);
#pragma endregion

#pragma region enabled


		//enabled
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_ENABLED, i + 1);
#else
		sprintf(num, "%s%d", STR_ENABLED, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FALSE,
			0,
			PIDX(i)+ ID_ENABLED
		);
#pragma endregion

#pragma region mode
//mode
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_MODE, i + 1);
#else
		sprintf(num, "%s%d", STR_MODE, i + 1);
#endif
		//ポップアップメニュー
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_MODE_COUNT,	//メニューの数
			STR_MODE_DEFL,	//デフォルト
			STR_MODE_ITEMS,
			PIDX(i) + ID_MODE
		);
#pragma endregion

#pragma region bordertopic
		//borderTopic
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_BORDER_TOPIC, i + 1);
#else
		sprintf(num, "%s%d", STR_BORDER_TOPIC, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_TOPIC(num, PIDX(0) + ID_BORDER_TOPIC);
#pragma endregion

#pragma region border
//border
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_BORDER, i + 1);
#else
		sprintf(num, "%s%d", STR_TARGET, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FALSE,
			0,
			PIDX(i) + ID_BORDER
		);
#pragma endregion

#pragma region inside
//inside
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_INSIDE, i + 1);
#else
		sprintf(num, "%s%d", STR_INSIDE, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			20,				//数値入力する場合の最大値
			0,				//スライダーの最小値 
			10,				//スライダーの最大値
			2,				//デフォルトの値
			PIDX(i) + ID_INSIDE
		);
#pragma endregion

#pragma region outside
		//outside
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_OUTSIDE, i + 1);
#else
		sprintf(num, "%s%d", STR_OUTSIDE, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			20,				//数値入力する場合の最大値
			0,				//スライダーの最小値 
			20,				//スライダーの最大値
			2,				//デフォルトの値
			PIDX(i) + ID_OUTSIDE
		);
#pragma endregion

#pragma region borderend

		//ID_BORDER_TOPIC_END
		AEFX_CLR_STRUCT(def);
		PF_END_TOPIC(PIDX(i) + ID_BORDER_TOPIC_END);
#pragma endregion

#pragma region max
		//max
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_MAX, i + 1);
#else
		sprintf(num, "%s%d", STR_MAX, i + 1);
#endif
		//----------------------------------------------------------------
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			-50, 				//数値入力する場合の最小値
			50,				//数値入力する場合の最大値
			-10,				//スライダーの最小値 
			10,				//スライダーの最大値
			0,				//デフォルトの値
			PIDX(i) + ID_MAX
		);
#pragma endregion

#pragma region blur

		//blur
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_BLUR, i + 1);
#else
		sprintf(num, "%s%d", STR_BLUR, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			100,			//数値入力する場合の最大値
			0,				//スライダーの最小値 
			50,				//スライダーの最大値
			20,				//デフォルトの値
			PIDX(i) + ID_BLUR
		);
#pragma endregion

#pragma region color
		//color
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_COLOR, i + 1);
#else
		sprintf(num, "%s%d", STR_COLOR, i + 1);
#endif
		//色の指定
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(num,
			0xFF,
			0x00,
			0x00,
			PIDX(i) + ID_COLOR
		);
#pragma endregion

#pragma region rev
		//rev
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_REV, i + 1);
#else
		sprintf(num, "%s%d", STR_REV, i + 1);
#endif
		//ポップアップメニュー
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_REV_COUNT,	//メニューの数
			STR_REV_DEFL,	//デフォルト
			STR_REV_ITEMS,
			PIDX(i) + ID_REV
		);
#pragma endregion

#pragma region blend
		//blend
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_BLEND, i + 1);
#else
		sprintf(num, "%s%d", STR_BLEND, i + 1);
#endif
		//ポップアップメニュー
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_BLEND_COUNT,	//メニューの数
			STR_BLEND_DFLT,	//デフォルト
			STR_BLEND_ITEMS,
			PIDX(i) + ID_BLEND
		);
#pragma endregion

#pragma region opacity
		//opacity
#ifdef AE_OS_WIN
		sprintf_s(num, "%s%d", STR_OPACITY, i + 1);
#else
		sprintf(num, "%s%d", STR_OPACITY, i + 1);
#endif
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			0,						//VALID_MIN
			100,					//VALID_MAX
			0,						//SLIDER_MIN
			100,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			100,					//DFLT
			1,						//PREC
			0,						//DISP
			0,						//WANT_PHASE
			PIDX(i) + ID_OPACITY
		);
#pragma endregion

#pragma region topicend
		//topicend
		AEFX_CLR_STRUCT(def);
		PF_END_TOPIC(PIDX(i) + ID_TOPIC_END);
#pragma endregion
	}
	//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;

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



	for (int i = 0; i < PCOUNT; i++)
	{
		ERR(ae->GetCHECKBOX(PIDX(i)+ ID_ENABLED, &infoP->flareInfo[i].enabled));
		ERR(ae->GetPOPUP(PIDX(i) + ID_MODE, &infoP->flareInfo[i].mode));
		ERR(ae->GetCHECKBOX(PIDX(i) + ID_BORDER, &infoP->flareInfo[i].border));
		ERR(ae->GetADD(PIDX(i) + ID_INSIDE, &infoP->flareInfo[i].inside));
		ERR(ae->GetADD(PIDX(i) + ID_OUTSIDE, &infoP->flareInfo[i].outside));
		ERR(ae->GetADD(PIDX(i) + ID_MAX, &infoP->flareInfo[i].max));
		ERR(ae->GetADD(PIDX(i) + ID_BLUR, &infoP->flareInfo[i].blur));
		ERR(ae->GetCOLOR(PIDX(i) + ID_COLOR, &infoP->flareInfo[i].color));
		ERR(ae->GetPOPUP(PIDX(i) + ID_REV, &infoP->flareInfo[i].rev));
		ERR(ae->GetPOPUP(PIDX(i) + ID_BLEND, &infoP->flareInfo[i].blend));
		ERR(ae->GetFLOAT(PIDX(i) + ID_OPACITY, &infoP->flareInfo[i].opacitry));
		infoP->flareInfo[i].opacitry /= 100;
		if (infoP->flareInfo[i].opacitry < 0) infoP->flareInfo[i].opacitry = 0;
		else if (infoP->flareInfo[i].opacitry > 1)infoP->flareInfo[i].opacitry = 1;

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


	ae->out->clear();

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		break;
	case PF_PixelFormat_ARGB64:
		break;
	case PF_PixelFormat_ARGB32:
		ERR(Exec08(ae, infoP));
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
