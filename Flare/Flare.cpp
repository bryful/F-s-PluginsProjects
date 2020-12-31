//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "Flare.h"
#include <stdio.h>
#include "Flare_def.h"



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

	//mode
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_MODE1,
		STR_MODE1_COUNT,	//メニューの数
		STR_MODE1_DEFL,	//デフォルト
		STR_MODE1_ITEMS,
		ID_MODE
	);
	//baseOn
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_BASE_ON,
		STR_ON,
		TRUE,
		0,
		ID_BASE_ON
	);
	//baseColor
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_BASE_COLOR,
		0xF4,//F4FF95
		0xFF,
		0x95,
		ID_BASE_COLOR
	);
	//baseOpacity
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_BASE_OPACITY,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_BASE_OPACITY
	);

	char num[255] = { "\0" };
	for (int i = 0; i < PCOUNT; i++)
	{
#pragma region topic
//topic
		SPRINTF(STR_TOPIC, i + 1);
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_START_COLLAPSED;
		PF_ADD_TOPIC(num, PIDX(i)+ ID_TOPIC);
#pragma endregion

#pragma region enabled
		//enabled
		SPRINTF(STR_ENABLED, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FI_Enabled[i],
			0,
			PIDX(i)+ ID_ENABLED
		);
#pragma endregion

#pragma region rev
		//rev
		SPRINTF(STR_REV, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_REV_COUNT,	//メニューの数
			FI_REV[i],	//デフォルト
			STR_REV_ITEMS,
			PIDX(i) + ID_REV
		);
#pragma endregion

#pragma region bordertopic
		//borderTopic
		SPRINTF(STR_BORDER_TOPIC, i + 1);
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_START_COLLAPSED;
		PF_ADD_TOPIC(num, PIDX(i) + ID_BORDER_TOPIC);
#pragma endregion

#pragma region border
//border
		SPRINTF(STR_BORDER, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FI_BORDER[i],
			0,
			PIDX(i) + ID_BORDER
		);
#pragma endregion

#pragma region inside
//inside
		SPRINTF(STR_INSIDE, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			20,				//数値入力する場合の最大値
			0,				//スライダーの最小値 
			10,				//スライダーの最大値
			FI_INSIDE[i],				//デフォルトの値
			PIDX(i) + ID_INSIDE
		);
#pragma endregion

#pragma region outside
		//outside
		SPRINTF(STR_OUTSIDE, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			20,				//数値入力する場合の最大値
			0,				//スライダーの最小値 
			20,				//スライダーの最大値
			FI_OUTSIDE[i],				//デフォルトの値
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
		SPRINTF(STR_MAX, i + 1);
		//----------------------------------------------------------------
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			100,			//数値入力する場合の最大値
			0,				//スライダーの最小値 
			20,				//スライダーの最大値
			FI_MAX[i],				//デフォルトの値
			PIDX(i) + ID_MAX
		);
#pragma endregion

#pragma region blur

		//blur
		SPRINTF(STR_BLUR, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_SLIDER(num,	//パラメータの名前
			0, 				//数値入力する場合の最小値
			100,			//数値入力する場合の最大値
			0,				//スライダーの最小値 
			50,				//スライダーの最大値
			FI_BLUR[i],		//デフォルトの値
			PIDX(i) + ID_BLUR
		);
#pragma endregion

#pragma region color
		//color
		SPRINTF(STR_COLOR, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(num,
			FI_COLOR[i].red,
			FI_COLOR[i].green,
			FI_COLOR[i].blue,
			PIDX(i) + ID_COLOR
		);
#pragma endregion



#pragma region blend
		//blend
		SPRINTF(STR_BLEND, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_POPUP(num,
			STR_BLEND_COUNT,	//メニューの数
			FI_BLEND[i],	//デフォルト
			STR_BLEND_ITEMS,
			PIDX(i) + ID_BLEND
		);
#pragma endregion

#pragma region opacity
		//opacity
		SPRINTF(STR_OPACITY, i + 1);
		AEFX_CLR_STRUCT(def);
		PF_ADD_FLOAT_SLIDER(num,	//Name
			0,						//VALID_MIN
			100,					//VALID_MAX
			0,						//SLIDER_MIN
			100,					//SLIDER_MAX
			1,						//CURVE_TOLERANCE
			FI_OPACITY[i],			//DFLT
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


	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));
	ERR(ae->GetCHECKBOX(ID_BASE_ON, &infoP->BaseOn));
	ERR(ae->GetCOLOR(ID_BASE_COLOR, &infoP->BaseColor));
	ERR(ae->GetFLOAT(ID_BASE_OPACITY, &infoP->BaseOpacity));
	infoP->BaseOpacity /= 100;
	if (infoP->BaseOpacity < 0) infoP->BaseOpacity = 0;
	else if (infoP->BaseOpacity > 1)infoP->BaseOpacity = 1;

	for (int i = 0; i < PCOUNT; i++)
	{
		ERR(ae->GetCHECKBOX(PIDX(i)+ ID_ENABLED, &infoP->flareInfo[i].enabled));
		ERR(ae->GetPOPUP(PIDX(i) + ID_REV, &infoP->flareInfo[i].rev));
		ERR(ae->GetCHECKBOX(PIDX(i) + ID_BORDER, &infoP->flareInfo[i].border));
		ERR(ae->GetADD(PIDX(i) + ID_INSIDE, &infoP->flareInfo[i].inside));
		infoP->flareInfo[i].inside = ae->downScaleNoClip(infoP->flareInfo[i].inside);
		ERR(ae->GetADD(PIDX(i) + ID_OUTSIDE, &infoP->flareInfo[i].outside));
		infoP->flareInfo[i].outside = ae->downScaleNoClip(infoP->flareInfo[i].outside);
		ERR(ae->GetADD(PIDX(i) + ID_MAX, &infoP->flareInfo[i].max));
		infoP->flareInfo[i].max = ae->downScaleNoClip(infoP->flareInfo[i].max);
		ERR(ae->GetADD(PIDX(i) + ID_BLUR, &infoP->flareInfo[i].blur));
		infoP->flareInfo[i].blur = ae->downScaleNoClip(infoP->flareInfo[i].blur);
		ERR(ae->GetCOLOR(PIDX(i) + ID_COLOR, &infoP->flareInfo[i].color));
		ERR(ae->GetPOPUP(PIDX(i) + ID_BLEND, &infoP->flareInfo[i].blend));
		ERR(ae->GetFLOAT(PIDX(i) + ID_OPACITY, &infoP->flareInfo[i].opacitry));
		infoP->flareInfo[i].opacitry /= 100;
		if (infoP->flareInfo[i].opacitry < 0) infoP->flareInfo[i].opacitry = 0;
		else if (infoP->flareInfo[i].opacitry > 1)infoP->flareInfo[i].opacitry = 1;

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err ParamInfoTo16(ParamInfo *infoP, ParamInfo16 *info16P)
{
	PF_Err	err = PF_Err_NONE;

	info16P->mode = infoP->mode;
	info16P->BaseOn = infoP->BaseOn;
	info16P->BaseOpacity = infoP->BaseOpacity;
	info16P->BaseColor = CONV8TO16(infoP->BaseColor);

	for (A_long i = 0; i < PCOUNT; i++)
	{
		info16P->flareInfo[i].blend = infoP->flareInfo[i].blend;
		info16P->flareInfo[i].blur = infoP->flareInfo[i].blur;
		info16P->flareInfo[i].border = infoP->flareInfo[i].border;
		info16P->flareInfo[i].bufH = infoP->flareInfo[i].bufH;
		info16P->flareInfo[i].color = CONV8TO16(infoP->flareInfo[i].color);
		info16P->flareInfo[i].enabled = infoP->flareInfo[i].enabled;
		info16P->flareInfo[i].inside = infoP->flareInfo[i].inside;
		info16P->flareInfo[i].max = infoP->flareInfo[i].max;
		info16P->flareInfo[i].opacitry = infoP->flareInfo[i].opacitry;
		info16P->flareInfo[i].outside = infoP->flareInfo[i].outside;
		info16P->flareInfo[i].rev = infoP->flareInfo[i].rev;

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



	ParamInfo16 info16;

	ParamInfoTo16(infoP, &info16);

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		break;
	case PF_PixelFormat_ARGB64:
		ERR(Exec16(ae, &info16));
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
