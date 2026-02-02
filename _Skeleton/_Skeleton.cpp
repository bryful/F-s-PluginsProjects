//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "_Skeleton.h"

#ifndef PF_CLAMP
#define PF_CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (MIN) : ((VAL) > (MAX) ? (MAX) : (VAL)))
#endif
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

	CParamsSetup cs(in_data, out_data);

	cs.AddFloatSilder(	// R
		STR_R,			//Name
		-2,				//VALID_MIN
		2,				//VALID_MAX
		-1,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_R
	);
	cs.AddFloatSilder(	// R
		STR_G,			//Name
		-2,				//VALID_MIN
		2,				//VALID_MAX
		-1,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_G
	);
	cs.AddFloatSilder(	// R
		STR_B,			//Name
		-2,				//VALID_MIN
		2,				//VALID_MAX
		-1,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_B
	);
	cs.AddFloatSilder(	// R
		STR_NOISE,			//Name
		0,				//VALID_MIN
		1,				//VALID_MAX
		0,				//SLIDER_MIN
		1,				//SLIDER_MAX
		0,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_NOISE
	);
		
	cs.AddCheckBox(	// noise frame
		STR_NOISE_FRAME1,
		STR_NOISE_FRAME2,
		TRUE,
		ID_NOISE_FRAME,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddSlider(	// noise offset
		STR_NOISE_OFFSET,	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		300,			//スライダーの最大値
		10,				//デフォルトの値
		ID_NOISE_OFFSET,
		PF_ParamFlag_NONE,
		PF_PUI_DISABLED
	);
	cs.AddCheckBox(
		STR_HIDDEN_ON1,
		STR_HIDDEN_ON2,
		FALSE,
		ID_HIDDEN_ON,
		PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP
	);
	cs.AddTopic(STR_TOPIC, ID_TOPIC, PF_ParamFlag_START_COLLAPSED);
	cs.AddColor(	// color
		STR_COLOR, 
		{ 0xFF, 0xFF, 0xFF, 0xFF },
		ID_COLOR,
		PF_ParamFlag_CANNOT_TIME_VARY//これをつけるとキーフレームが撃てなくなる
	);
	//cs.SetParamFlags();
	cs.AddSlider(	// add slider
		STR_ADD_SLIDER,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		10,				//デフォルトの値
		ID_ADD_SLIDER,
		PF_ParamFlag_CANNOT_TIME_VARY//これをつけるとキーフレームが撃てなくなる
	);
	cs.AddFixedSlider(		STR_FIXED_SLIDER,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,			//スライダーの最大値
		10,				//デフォルトの値
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		ID_FIXED_SLIDER,
		PF_ParamFlag_CANNOT_TIME_VARY//これをつけるとキーフレームが撃てなくなる
	);
	cs.AddFloatSilder(	// float slider
		STR_FLOAT_SLIDER,	//パラメータの名前
		-10000, 				//数値入力する場合の最小値
		10000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		500,				//デフォルトの値
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_FLOAT_SLIDER
	);
	cs.AddCheckBox(	// checkbox
		STR_CHECKBOX1,
		STR_CHECKBOX2,
		FALSE,
		ID_CHECKBOX
	);
	cs.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE
	);
	cs.AddPopup(		STR_POPUP,
		STR_POPUP_COUNT,
		STR_POPUP_DFLT,
		STR_POPUP_ITEMS,
		ID_POPUP
	);
	cs.AddPoint(
		STR_POINT,
		50, 
		50,
		FALSE,
		ID_POINT
	);
	cs.EndTopic(ID_TOPIC_END);
	cs.AddButton(	// button
		STR_BUTTON1,
		STR_BUTTON2,
		ID_BUTTON,
		PF_ParamFlag_SUPERVISE
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

	try{
		CFsAE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err) {
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
		}
		if((!err)&&(in_data->appl_id != 'PrMr')){
			//--------------------
			
			ERR(ae.GetNewEffectStreamAll());
			if (!err){
				A_Boolean hide_themB[ID_NUM_PARAMS];
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) hide_themB[i] =FALSE;
				PF_Boolean b;
				ERR(ae.GetCHECKBOX(ID_HIDDEN_ON,&b));
				if (b){
					for ( A_long i=ID_HIDDEN_ON+1; i<ID_NUM_PARAMS; i++)hide_themB[i] =TRUE;
				}
				for ( A_long i=1; i<ID_NUM_PARAMS-1; i++) 
					ERR(ae.SetDynamicStreamFlag(i,AEGP_DynStreamFlag_HIDDEN,hide_themB[i]));
			}
			//--------------------
			if (!err){
				PF_Boolean b = FALSE;
				ERR(ae.GetCHECKBOX(ID_NOISE_FRAME,&b));
				ERR(ae.UI_DISABLE(ID_NOISE_OFFSET, b));
			}
			//--------------------
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
		}
	}catch ( PF_Err & errP){
		err = errP;
	}
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
	CFsAE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_NOISE_FRAME,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	return err;
}
// *******************************************************************************
// -- - 1. 型解決用のトレイト(PixelTraits) -- -
template <typename T> struct PixelTraits;

template <> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char channel_type;
};

template <> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short channel_type;
};

template <> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort channel_type;
};
// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}

// --- 5. 共通フィルタテンプレート ---
template <typename T>
static PF_Err FilterImage(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	ParamInfo* niP = reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	// 色の加算処理
	PF_FpLong r = (PF_FpLong)inP->red + (niP->r * max_val);
	PF_FpLong g = (PF_FpLong)inP->green + (niP->g * max_val);
	PF_FpLong b = (PF_FpLong)inP->blue + (niP->b * max_val);

	// ノイズ処理
	if (niP->noise > 0) {
		// -1.0 ~ 1.0 のランダム値を生成してスケーリング
		PF_FpLong random_factor = (PF_FpLong)F_RAND2(-1000, 1000) / 1000.0;
		PF_FpLong vv = random_factor * niP->noise * max_val;
		r += vv;
		g += vv;
		b += vv;
	}

	// 出力処理 (PixelTraitsを使用して適切な型にキャスト)
	if (std::is_same<T, PF_PixelFloat>::value) {
		// 32bit float の場合はクランプ不要
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(r);
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(g);
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(b);
	}
	else {
		// 整数型の場合はクランプしてから適切な型にキャスト
		outP->red = static_cast<typename PixelTraits<T>::channel_type>(PF_CLAMP(r, 0, max_val));
		outP->green = static_cast<typename PixelTraits<T>::channel_type>(PF_CLAMP(g, 0, max_val));
		outP->blue = static_cast<typename PixelTraits<T>::channel_type>(PF_CLAMP(b, 0, max_val));
	}

	outP->alpha = inP->alpha; // アルファはそのまま維持

	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_R,&infoP->r));
	ERR(ae->GetFLOAT(ID_G,&infoP->g));
	ERR(ae->GetFLOAT(ID_B,&infoP->b));

	ERR(ae->GetFLOAT(ID_NOISE,&infoP->noise));
	ERR(ae->GetCHECKBOX(ID_NOISE_FRAME,&infoP->noise_frame));
	ERR(ae->GetADD(ID_NOISE_OFFSET,&infoP->noise_offset));


	ERR(ae->GetADD(ID_ADD_SLIDER,&infoP->add));
	ERR(ae->GetANGLE(ID_ANGLE,&infoP->agl));
	ERR(ae->GetCHECKBOX(ID_CHECKBOX,&infoP->cbx));
	ERR(ae->GetFLOAT(ID_FLOAT_SLIDER,&infoP->flt));
	ERR(ae->GetFIXED(ID_FIXED_SLIDER,&infoP->fxd));
	ERR(ae->GetFIXEDPOINT(ID_POINT,&infoP->pnt));
	ERR(ae->GetPOPUP(ID_POPUP,&infoP->pop));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if ( (infoP->r==0)&&(infoP->g==0)&&(infoP->b==0)&&(infoP->noise<=0))
	{
		return err;
	}
	if ( infoP->noise_frame == TRUE){
		infoP->noise_offset = 0;
		F_SRAND(ae->frame());
	}else{
		F_SRAND(infoP->noise_offset);
	}
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32(ae->input,(refconType)infoP, FilterImage<PF_PixelFloat>,ae->output));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16(ae->input, (refconType)infoP, FilterImage<PF_Pixel16>, ae->output));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8(ae->input, (refconType)infoP, FilterImage<PF_Pixel8>, ae->output));
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

#include "NF_Entry.h"

