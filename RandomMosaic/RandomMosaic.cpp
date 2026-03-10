//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "RandomMosaic.h"

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err			err = PF_Err_NONE;

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddFloatSlider(	// R
		STR_MASTER,		//Name
		0,				//VALID_MIN
		500,			//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		0,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_MASTER
	);
	// ----------------------------------------------------------------
	// *******************************************************	
	// *******************************************************	
	cs.AddTopic(STR_MOSAIC_TOPIC, ID_MOSAIC_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddCheckBox(	// noise frame
		STR_MOSAIC_ENABLED,
		"on",
		FALSE,
		ID_MOSAIC_ENABLED
	);
	cs.AddFloatSlider(	// R
		STR_MOSAIC_AMOUNT,			//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		50,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_MOSAIC_AMOUNT
	);
	cs.AddSlider(	// 
		STR_MOSAIC_WIDTH,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		100,			//スライダーの最大値
		32,			//デフォルトの値
		ID_MOSAIC_WIDTH
	);
	cs.AddSlider(	// 
		STR_MOSAIC_HEIGHT,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		100,			//スライダーの最大値
		32,			//デフォルトの値
		ID_MOSAIC_HEIGHT
	);
	cs.AddFloatSlider(	// R
		STR_MOSAIC_VALUE,	//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		50,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_MOSAIC_VALUE
	);
	cs.AddCheckBox(	// noise frame
		STR_MOSAIC_ISCOLOR,
		"on",
		FALSE,
		ID_MOSAIC_ISCOLOR
	);
	cs.EndTopic(ID_MOSAIC_TOPIC_END);
	// *******************************************************	
	// *******************************************************	
	cs.AddTopic(STR_CBLOCK_TOPIC, ID_CBLOCK_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddCheckBox(	// noise frame
		STR_CBLOCK_ENABLED,
		"on",
		FALSE,
		ID_CBLOCK_ENABLED
	); 
	cs.AddSlider(	// 
		STR_CBLOCK_COUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		10000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		50,			//デフォルトの値
		ID_CBLOCK_COUNT
	);
	cs.AddSlider(	// 
		STR_CBLOCK_WIDTH,	//パラメータの名前
		8, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		8,				//スライダーの最小値 
		1000,			//スライダーの最大値
		500,			//デフォルトの値
		ID_CBLOCK_WIDTH
	);
	cs.AddSlider(	// 
		STR_CBLOCK_HEIGHT,	//パラメータの名前
		8, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		8,				//スライダーの最小値 
		1000,				//スライダーの最大値
		100,				//デフォルトの値
		ID_CBLOCK_HEIGHT
	);
	cs.AddFloatSlider(	// R
		STR_CBLOCK_VALUE,			//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		100,			//SLIDER_MAX
		35,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_CBLOCK_VALUE
	);
	cs.AddCheckBox(	// noise frame
		STR_CBLOCK_ALPHA,
		"on",
		FALSE,
		ID_CBLOCK_ALPHA
	);
	cs.EndTopic(ID_CBLOCK_TOPIC_END);
	// *******************************************************	
	cs.AddTopic(STR_SHIFT_TOPIC, ID_SHIFT_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddCheckBox(	// noise frame
		STR_SHIFT_ENABLED,
		"on",
		FALSE,
		ID_SHIFT_ENABLED
	);
	cs.AddSlider(	// 
		STR_SHIFT_COUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		10000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		100,				//デフォルトの値
		ID_SHIFT_COUNT
	);
	cs.AddSlider(	// 
		STR_SHIFT_WIDTH,	//パラメータの名前
		8, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		8,				//スライダーの最小値 
		500,			//スライダーの最大値
		128,				//デフォルトの値
		ID_SHIFT_WIDTH
	);
	cs.AddSlider(	// 
		STR_SHIFT_HEIGHT,	//パラメータの名前
		8, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		8,				//スライダーの最小値 
		500,				//スライダーの最大値
		64,				//デフォルトの値
		ID_SHIFT_HEIGHT
	);
	cs.AddSlider(	// 
		STR_SHIFT_OFFSET_X,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		128,			//スライダーの最大値
		32,				//デフォルトの値
		ID_SHIFT_OFFSET_X
	);
	cs.AddSlider(	// 
		STR_SHIFT_OFFSET_Y,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		128,				//スライダーの最大値
		8,				//デフォルトの値
		ID_SHIFT_OFFSET_Y
	);
	cs.EndTopic(ID_SHIFT_TOPIC_END);
	// *******************************************************	
	cs.AddTopic(STR_BLOCK_TOPIC, ID_BLOCK_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddCheckBox(	// noise frame
		STR_BLOCK_ENABLED,
		"on",
		FALSE,
		ID_BLOCK_ENABLED
	);
	cs.AddSlider(	// 
		STR_BLOCK_COUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		10000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		10,				//デフォルトの値
		ID_BLOCK_COUNT
	);
	cs.AddSlider(	// 
		STR_BLOCK_WIDTH,	//パラメータの名前
		8, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		8,				//スライダーの最小値 
		1000,			//スライダーの最大値
		256,				//デフォルトの値
		ID_BLOCK_WIDTH
	);
	cs.AddSlider(	// 
		STR_BLOCK_HEIGHT,	//パラメータの名前
		8, 				//数値入力する場合の最小値
		4000,			//数値入力する場合の最大値
		8,				//スライダーの最小値 
		1000,			//スライダーの最大値
		128,				//デフォルトの値
		ID_BLOCK_HEIGHT
	);
	cs.AddFloatSlider(	// R
		STR_BLOCK_NOISE,			//Name
		0,				//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		20,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_BLOCK_NOISE
	);
	cs.AddCheckBox(	// noise frame
		STR_BLOCK_NOISE_ISCOLOR,
		"on",
		FALSE,
		ID_BLOCK_NOISE_ISCOLOR
	);
	cs.EndTopic(ID_BLOCK_TOPIC_END);
	// *******************************************************	
	cs.AddTopic(STR_EMBOSS_TOPIC, ID_EMBOSS_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddCheckBox(	// noise frame
		STR_EMBOSS_ENABLED,
		"on",
		FALSE,
		ID_EMBOSS_ENABLED
	);
	cs.AddFloatSlider(	// R
		STR_EMBOSS_DISTANCE,			//Name
		-1000,				//VALID_MIN
		1000,				//VALID_MAX
		-100,				//SLIDER_MIN
		100,				//SLIDER_MAX
		2,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_EMBOSS_DISTANCE
	);
	cs.AddCheckBox(	// noise frame
		STR_EMBOSS_ISCOLOR,
		"on",
		FALSE,
		ID_EMBOSS_ISCOLOR
	);
	cs.AddCheckBox(	// noise frame
		STR_EMBOSS_ALPHA,
		"on",
		FALSE,
		ID_EMBOSS_ALPHA
	);
	cs.EndTopic(ID_EMBOSS_TOPIC_END);
	// **************************************************************
	cs.AddTopic(STR_NOISE_TOPIC, ID_NOISE_TOPIC//,
		//PF_ParamFlag_START_COLLAPSED,
		//PF_PUI_DISABLED
	);
	cs.AddCheckBox(	// noise frame
		STR_NOISE_ENABLED,
		"on",
		FALSE,
		ID_NOISE_ENABLED
	);
	cs.AddFloatSlider(	// R
		STR_NOISE_AMOUNT,			//Name
		0,				//VALID_MIN
		300,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		1,				//DFLT
		1,				//PREC 小数点以下の桁数
		1,				//DISP 1で％表示
		FALSE,			//WANT_PHASE
		ID_NOISE_AMOUNT

	);
	cs.AddCheckBox(	// noise frame
		STR_NOISE_ISCOLOR,
		"on",
		FALSE,
		ID_NOISE_ISCOLOR
	);
	cs.EndTopic(ID_NOISE_TOPIC_END);
	// *******************************************************	
	cs.AddCheckBox(	// noise frame
		STR_AUTO_SEED,
		"on",
		TRUE,
		ID_AUTO_SEED,
		PF_ParamFlag_SUPERVISE
	);
	cs.AddSlider(	// 
		STR_SEED,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-1000,			//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED
	);

	cs.Finalize();
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

	/*
	try{
		NF_AE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
			// ボタンが押されたときの処理
			if (extraP->param_index == ID_BUTTON)
			{
				A_char scriptCode[2048] = { '\0' };
				PF_SPRINTF(scriptCode, FS_ABOUT_DIALOG,
					NF_NAME,
					MAJOR_VERSION,
					MINOR_VERSION,
					__DATE__,
					NF_DESCRIPTION);

				ERR(ae.suitesP->UtilitySuite5()->AEGP_ExecuteScript(ae.ae_plugin_idP->my_id, scriptCode, TRUE, NULL, NULL));
				out_data->out_flags |= PF_OutFlag_REFRESH_UI;
			}

			ERR(ae.GetNewEffectStreamAll());
			if (!err) {
				A_Boolean hide_themB[ID_NUM_PARAMS];
				//とりあえず全てのパラメータを非表示にする
				for (A_long i = 1; i < ID_NUM_PARAMS; i++) hide_themB[i] = TRUE;
				// 常時表示はここでFALSEにする
				hide_themB[ID_MODE] = FALSE;
				hide_themB[ID_AUTO_SEED] = FALSE;
				hide_themB[ID_SEED] = FALSE;
				A_long mode;
				ERR(ae.GetPOPUP(ID_MODE, &mode));
				switch (mode) {
					case MODE_NOISE:
						hide_themB[ID_NOISE_TOPIC] = FALSE;
						hide_themB[ID_NOISE_VALUE] = FALSE;
						hide_themB[ID_NOISE_ISCOLOR] = FALSE;
						break;
					case MODE_RECT:
						hide_themB[ID_RECT_TOPIC] = FALSE;
						for (int i = 0; i < 4; i++) hide_themB[ID_RECT_POS1 + i] = FALSE;
						hide_themB[ID_RECT_COLOR] = FALSE;
						break;
					case MODE_LINE:
						hide_themB[ID_LINE_TOPIC] = FALSE;
						for (int i = 0; i < 4; i++) {
							hide_themB[ID_LINE_POS1 + i*2] = FALSE;
							hide_themB[ID_LINE_W1 + i*2] = FALSE;
						}
						hide_themB[ID_LINE_COLOR] = FALSE;
						break;

				}
				// パラメータの表示/非表示を切り替える
				for (A_long i = 1; i < ID_NUM_PARAMS; i++)
					ERR(ae.SetDynamicStreamFlag(i, AEGP_DynStreamFlag_HIDDEN, hide_themB[i]));
				if (!err && in_data->appl_id != 'PrMr') {
					
					PF_Boolean b = FALSE;
					ERR(ae.GetCHECKBOX(ID_AUTO_SEED, &b));
					ERR(ae.UI_DISABLE(ID_SEED, !b));
					
				}
				if (!err) {
					out_data->out_flags |= PF_OutFlag_REFRESH_UI;
				}

			}
		}
	}catch ( PF_Err & errP){
		err = errP;
	}
	*/
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
	PF_Err 	err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	NF_AE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		//PF_ParamDef def;
		//AEFX_CLR_STRUCT(def);
		//ERR(ae.checkout_param(ID_AUTO_SEED,&def));
		PF_Boolean auto_seedB = FALSE;
		ae.GetCHECKBOX(ID_AUTO_SEED, &auto_seedB);
		ERR(ae.SetOutFlag_NON_PARAM_VARY(auto_seedB));
		//ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		//ERR(ae.checkin_param(&def));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT_float(ID_MASTER, &infoP->master));
	infoP->master /= 100;

	ERR(ae->GetCHECKBOX(ID_MOSAIC_ENABLED, &infoP->mEnabled));
	ERR(ae->GetFLOAT_float(ID_MOSAIC_AMOUNT, &infoP->mAmount));
	infoP->mAmount /= 100;
	ERR(ae->GetADD(ID_MOSAIC_WIDTH, &infoP->mWidth));
	ERR(ae->GetADD(ID_MOSAIC_HEIGHT, &infoP->mHeight));
	ERR(ae->GetFLOAT_float(ID_MOSAIC_VALUE, &infoP->mValue));
	infoP->mValue /= 100;
	ERR(ae->GetCHECKBOX(ID_MOSAIC_ISCOLOR, &infoP->mIsColor));

	ERR(ae->GetCHECKBOX(ID_CBLOCK_ENABLED, &infoP->cbEnabled));
	ERR(ae->GetADD(ID_CBLOCK_COUNT, &infoP->cbCount));
	ERR(ae->GetADD(ID_CBLOCK_WIDTH, &infoP->cbWidth));
	ERR(ae->GetADD(ID_CBLOCK_HEIGHT, &infoP->cbHeight));
	ERR(ae->GetFLOAT_float(ID_CBLOCK_VALUE, &infoP->cbValue));
	infoP->cbValue /= 100;
	ERR(ae->GetCHECKBOX(ID_CBLOCK_ALPHA, &infoP->cbAlpha));

	ERR(ae->GetCHECKBOX(ID_SHIFT_ENABLED, &infoP->rsEnabled));
	ERR(ae->GetADD(ID_SHIFT_COUNT, &infoP->rsCount));
	ERR(ae->GetADD(ID_SHIFT_WIDTH, &infoP->rsWidth));
	ERR(ae->GetADD(ID_SHIFT_HEIGHT, &infoP->rsHeight));
	ERR(ae->GetADD(ID_SHIFT_OFFSET_X, &infoP->rsOffsetX));
	ERR(ae->GetADD(ID_SHIFT_OFFSET_Y, &infoP->rsOffsetY));

	ERR(ae->GetCHECKBOX(ID_BLOCK_ENABLED, &infoP->bmEnabled));
	ERR(ae->GetADD(ID_BLOCK_COUNT, &infoP->bmCount));
	ERR(ae->GetADD(ID_BLOCK_WIDTH, &infoP->bmWidth));
	ERR(ae->GetADD(ID_BLOCK_HEIGHT, &infoP->bmHeight));
	ERR(ae->GetFLOAT_float(ID_BLOCK_NOISE, &infoP->bmNoise));
	infoP->bmNoise /= 100;
	ERR(ae->GetCHECKBOX(ID_BLOCK_NOISE_ISCOLOR, &infoP->bmNoiseIsColor));

	ERR(ae->GetCHECKBOX(ID_EMBOSS_ENABLED, &infoP->ebEnabled));
	ERR(ae->GetFLOAT_float(ID_EMBOSS_DISTANCE, &infoP->ebDistance));
	ERR(ae->GetCHECKBOX(ID_EMBOSS_ISCOLOR, &infoP->ebIsColor));
	ERR(ae->GetCHECKBOX(ID_EMBOSS_ALPHA, &infoP->ebIsAlpha));

	ERR(ae->GetCHECKBOX(ID_NOISE_ENABLED, &infoP->noiseEnabled));
	ERR(ae->GetFLOAT_float(ID_NOISE_AMOUNT, &infoP->noiseAmount));
	infoP->noiseAmount /= 100;
	ERR(ae->GetCHECKBOX(ID_NOISE_ISCOLOR, &infoP->noiseIsColor));


	ERR(ae->GetCHECKBOX(ID_AUTO_SEED, &infoP->autoSeed));
	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	infoP->mAmount = (float)infoP->mAmount * infoP->master;
	infoP->cbCount = (A_long)((float)infoP->cbCount * infoP->master + 0.5f);
	infoP->rsCount = (A_long)((float)infoP->rsCount * infoP->master + 0.5f);
	infoP->bmCount = (A_long)((float)infoP->bmCount * infoP->master + 0.5f);
	infoP->bmNoise = infoP->bmNoise * infoP->master;
	infoP->ebDistance = infoP->ebDistance * infoP->master;
	infoP->noiseAmount = infoP->noiseAmount * infoP->master;


	infoP->mWidth = AE_CLAMP(ae->downScale(infoP->mWidth), 1, ae->output->width);
	infoP->mHeight = AE_CLAMP(ae->downScale(infoP->mHeight), 1, ae->output->height);

	infoP->cbWidth = AE_CLAMP(ae->downScale(infoP->cbWidth), 1, ae->output->width);
	infoP->cbHeight = AE_CLAMP(ae->downScale(infoP->cbHeight), 1, ae->output->height);

	infoP->rsWidth = AE_CLAMP(ae->downScale(infoP->rsWidth),1,ae->output->width);
	infoP->rsHeight = AE_CLAMP(ae->downScale(infoP->rsHeight), 1, ae->output->height);
	infoP->rsOffsetX = ae->downScale(infoP->rsOffsetX);
	if (infoP->rsOffsetX + infoP->rsWidth >= ae->output->width) {
		infoP->rsOffsetX = ae->output->width - infoP->rsOffsetX;
	}
	infoP->rsOffsetY = ae->downScale(infoP->rsOffsetY);
	if (infoP->rsOffsetY + infoP->rsHeight >= ae->output->height) {
		infoP->rsOffsetY = ae->output->height - infoP->rsOffsetY;
	}
	infoP->bmWidth = AE_CLAMP(ae->downScale(infoP->bmWidth), 1, ae->output->width);
	infoP->bmHeight = AE_CLAMP(ae->downScale(infoP->bmHeight), 1, ae->output->height);

	infoP->ebDistance = (float)ae->downScale(infoP->ebDistance);

	if (infoP->autoSeed == TRUE) {
		infoP->seed = infoP->seed + ae->frame()*10;
	}

	if (infoP->mAmount > 0 && infoP->mEnabled) {
		ERR(MosaicPatMain(
			ae->output,
			ae->pixelFormat(),
			infoP->mAmount,
			infoP->mWidth,
			infoP->mHeight,
			infoP->mValue,
			infoP->mIsColor,
			infoP->seed
		));
	}
	if (infoP->cbCount > 0 && infoP->cbValue>0 && infoP->cbEnabled) {
		ERR(ColorBlockMain(
			ae->output,
			ae->pixelFormat(),
			infoP->cbCount,
			infoP->cbWidth,
			infoP->cbHeight,
			infoP->cbValue,
			infoP->cbAlpha,
			infoP->seed
		));
	}


	if (infoP->rsCount > 0 && infoP->rsEnabled) {
		ERR(RandomShiftMain(
			ae->output,
			ae->pixelFormat(),
			infoP->rsCount,
			infoP->rsWidth,
			infoP->rsHeight,
			infoP->rsOffsetX,
			infoP->rsOffsetY,
			infoP->seed
		));
	}
	if (infoP->bmCount > 0 && infoP->bmEnabled) {
		ERR(BlockMosaicMain(
			ae->output,
			ae->pixelFormat(),
			infoP->bmCount,
			infoP->bmWidth,
			infoP->bmHeight,
			infoP->bmNoise,
			infoP->bmNoiseIsColor,
			infoP->seed
		));
	}
	if (infoP->ebEnabled && infoP->ebDistance!=0) {
		ERR(ColorEmbossHorMain(
			ae->pixelFormat(),
			ae->suitesP,
			ae->output,
			infoP->ebDistance,
			infoP->ebIsColor,
			infoP->ebIsAlpha
			));
	}
	if (infoP->noiseEnabled && infoP->noiseAmount > 0) {
		ERR(NoiseExec(
			ae->in_data,
			ae->output,
			ae->pixelFormat(),
			ae->suitesP,
			infoP->noiseAmount,
			infoP->noiseIsColor,
			infoP->seed
		));
	}

	return err;
}


//-----------------------------------------------------------------------------------
DllExport
PF_Err PluginDataEntryFunction2(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB2 inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT_EXT2(
		inPtr,
		inPluginDataCallBackPtr,
		NF_NAME, // Name
		NF_MATCHNAME, // Match Name
		NF_CATEGORY, // Category
		AE_RESERVED_INFO, // Reserved Info
		"EffectMain",	// Entry point
		"https://github.com/bryful");	// support URL

	return result;
}
DllExport	PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	void* extraP)
{
	PF_Err			err = PF_Err_NONE,
					err2 = PF_Err_NONE;

	try
	{
		NF_AE ae;
		switch (cmd) {
		case PF_Cmd_ABOUT:
			err = ae.About(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = ae.GlobalSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETDOWN:
			err = ae.GlobalSetdown(in_data);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_SEQUENCE_SETUP:
			break;
		case PF_Cmd_SEQUENCE_SETDOWN:
			break;
		case PF_Cmd_SEQUENCE_RESETUP:
			break;
		case PF_Cmd_FRAME_SETUP:
			//err = ae.FrameSetup(in_data, out_data, params, output, ID_NUM_PARAMS);
			break;
		case PF_Cmd_RENDER:
			err = ae.Render(in_data, out_data, params, output, ID_NUM_PARAMS);
			if (!err) {
				ParamInfo info;
				ERR(GetParams(&ae, &info));
				ERR(Exec(&ae, &info));
			}
			break;
		case PF_Cmd_SMART_PRE_RENDER:
			err = ae.PreRender(in_data, out_data, reinterpret_cast<PF_PreRenderExtra*>(extraP),sizeof(ParamInfo), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ae.SetHostPreRenderData();
					ERR(GetParams(&ae, infoP));
					ERR(ae.UnSetPreRenderData());
					ae.UnlockPreRenderData();
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		case PF_Cmd_SMART_RENDER:
			err = ae.SmartRender(in_data, out_data, reinterpret_cast<PF_SmartRenderExtra*>(extraP), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ERR(Exec(&ae, infoP));
					ERR2(ae.UnsetSmartRender());
					ae.UnlockPreRenderData();
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		case PF_Cmd_COMPLETELY_GENERAL:
		{
			AEGP_SuiteHandler suites(in_data->pica_basicP);

			suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
				"%s",
				reinterpret_cast<A_char*>(extraP));
			break;
		}
		case PF_Cmd_DO_DIALOG:
		{
			//err = PopDialog(in_data,out_data,params,output);
			break;
		}
		case PF_Cmd_USER_CHANGED_PARAM:
		{
			err = HandleChangedParam(in_data,
				out_data,
				params,
				output,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));

			break;
		}
		case PF_Cmd_QUERY_DYNAMIC_FLAGS:
			err = QueryDynamicFlags(in_data,
				out_data,
				params,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}
	if (err != PF_Err_NONE) {
		AEGP_SuiteHandler suites(in_data->pica_basicP);

		suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
			"%d", cmd
			);
	}
	return err;
}

