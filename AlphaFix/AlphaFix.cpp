//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010 and VS2012
*/
//-----------------------------------------------------------------------------------

#include "AlphaFix.h"


//-------------------------------------------------------------------------------------------------
// Aboutダイアログ
static PF_Err About (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.About(in_data,out_data,params,output);
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetup(in_data,out_data,params,output);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetdown(
	PF_InData	*in_data)
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetdown(in_data);
	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup (	PF_InData		*in_data,
								PF_OutData		*out_data,
								PF_ParamDef		*params[],
								PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


static PF_Err SequenceSetdown (	PF_InData		*in_data,
								PF_OutData		*out_data,
								PF_ParamDef		*params[],
								PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


static PF_Err SequenceResetup (	PF_InData		*in_data,
								PF_OutData		*out_data,
								PF_ParamDef		*params[],
								PF_LayerDef		*output )
{
	return PF_Err_NONE;
}
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
	err = cs.AddColor(
		STR_BASE_COLOR,
		{ 255,0,0,0 },
		ID_BASE_COLOR,
		PF_ParamFlag_NONE
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


// 型に応じた構造体を選択するヘルパー（Traits）
template<typename T> struct ParamTraits;
template<> struct ParamTraits<PF_Pixel8> { using ParamType = ParamInfo8; };
template<> struct ParamTraits<PF_Pixel16> { using ParamType = ParamInfo16; };
template<> struct ParamTraits<PF_PixelFloat> { using ParamType = ParamInfo32; };

template <typename T>
static PF_Err
FilterImageGeneric(
	void* refcon,
	A_long xL, A_long yL,
	T* inP, T* outP)
{
	using InfoType = typename ParamTraits<T>::ParamType;
	InfoType* niP = reinterpret_cast<InfoType*>(refcon);

	if (!niP) return PF_Err_BAD_CALLBACK_PARAM;

	if (inP->alpha <= 0) {
		// アルファが0なら背景色をそのまま出力
		*outP = niP->base_color;
	}
	else {
		if constexpr (std::is_same_v<T, PF_PixelFloat>) {
			// --- 32bit Float パス (Straight入力対応) ---
			float alphaF = inP->alpha;
			float invAlpha = 1.0f - alphaF;

			// 合成結果は不透明（背景と合成するため）
			outP->alpha = 1.0f;

			// 背景色 * (1 - Alpha) + 前景色 * Alpha
			outP->red = (niP->base_color.red * invAlpha) + (inP->red * alphaF);
			outP->green = (niP->base_color.green * invAlpha) + (inP->green * alphaF);
			outP->blue = (niP->base_color.blue * invAlpha) + (inP->blue * alphaF);
		}
		else {
			// --- 8/16bit 整数パス ---
			A_long max_chan = (std::is_same_v<T, PF_Pixel16>) ? PF_MAX_CHAN16 : PF_MAX_CHAN8;
			A_long alpha = static_cast<A_long>(inP->alpha);
			A_long invAlpha = max_chan - alpha;

			outP->alpha = static_cast<decltype(T::alpha)>(max_chan);

			// 丸め誤差を最小限にするため、最後に除算
			outP->red = static_cast<decltype(T::red)>((niP->base_color.red * invAlpha + inP->red * alpha) / max_chan);
			outP->green = static_cast<decltype(T::green)>((niP->base_color.green * invAlpha + inP->green * alpha) / max_chan);
			outP->blue = static_cast<decltype(T::blue)>((niP->base_color.blue * invAlpha + inP->blue * alpha) / max_chan);
		}
	}
	return PF_Err_NONE;
}
//-----------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo8 *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCOLOR(ID_BASE_COLOR,&infoP->base_color));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo8 *infoP)
{
	PF_Err	err = PF_Err_NONE;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ParamInfo32 info32;
		info32.base_color = CONV8TO32(infoP->base_color);
		ERR( ae->iterate32((refconType)&info32, FilterImageGeneric<PF_PixelFloat>));
		break;
	case PF_PixelFormat_ARGB64:
		ParamInfo16 info16;
		info16.base_color = CONV8TO16(infoP->base_color);
		ERR( ae->iterate16((refconType)&info16, FilterImageGeneric<PF_Pixel16>));
		break;
	case PF_PixelFormat_ARGB32:
		ERR( ae->iterate8((refconType)infoP, FilterImageGeneric<PF_Pixel>));
		break;
	}


	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err 
Render ( 
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_dataP,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ERR(ae.CopyInToOut());
		ParamInfo8 info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;
}
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_dataP,
	PF_OutData			*out_dataP,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_dataP,out_dataP,extraP,sizeof(ParamInfo8),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo8 *infoP = reinterpret_cast<ParamInfo8*>(ae.LockPreRenderData());
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
//-----------------------------------------------------------------------------------
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
		ParamInfo8 *infoP = reinterpret_cast<ParamInfo8*>(ae.LockPreRenderData());
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
//-----------------------------------------------------------------------------------
static PF_Err 
RespondtoAEGP ( 	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extraP)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extraP));

	return err;
}


//-----------------------------------------------------------------------------------
DllExport	PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP)
{
	PF_Err		err = PF_Err_NONE;
	
	try
	{
		CFsAE ae;
		switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_data);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceResetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
					err = Render(in_data,out_data,params,output);
					break;
#if defined(SUPPORT_SMARTFX)
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_data,out_data,reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(	in_data,out_data,reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
#endif
			case PF_Cmd_COMPLETELY_GENERAL:
				err = RespondtoAEGP(in_data,out_data,params,output,extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;		
			case PF_Cmd_USER_CHANGED_PARAM:
				err = HandleChangedParam(	in_data,
											out_data,
											params,
											output, 
											reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;

			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_data,
										out_data,
										params,
										reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------

