//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "CellLineEraser.h"


//-----------------------------------------------------------------------------------

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

	// ----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_Use,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		10,		//数値入力する場合の最大値
		0,		//スライダーの最小値 
		10,		//スライダーの最大値
		0,		//デフォルトの値
		ID_Color_COUNT
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color1,
					0x00,
					0x00,
					0x00,
		ID_Color1
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color2,
		0xFF,
		0x00,
		0x00,
		ID_Color2
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color3,
		0x00,
		0xFF,
		0x00,
		ID_Color3
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color4,
		0x00,
		0x00,
		0xFF,
		ID_Color4
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color5,
		0x80,
		0x00,
		0x00,
		ID_Color5
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color6,
		0x00,
		0x80,
		0x00,
		ID_Color6
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color7,
		0x00,
		0x00,
		0x80,
		ID_Color7
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color8,
		0x40,
		0x00,
		0x00,
		ID_Color1
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color9,
		0x00,
		0x40,
		0x00,
		ID_Color9
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_Color10,
		0x00,
		0x00,
		0x40,
		ID_Color10
	);
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_KEEP_PIXELS,
		"on",
		FALSE,
		0,
		ID_KEEP_PIXELS
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_FillUnknownColors,
		"on",
		FALSE,
		0,
		ID_Fill_Color
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_FillColor,
		0xFF,
		0x00,
		0xFF,
		ID_Fill_Color
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MakeWhiteTransparent,
		"on",
		FALSE,
		0,
		ID_MakeWhiteTransparent
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
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

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
static PF_Err GetParams(CFsAE *ae, FillInfo*infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_Color_COUNT, &infoP->target_count));

	for (int i = 0; i < 10; i++)
	{
		PF_Pixel p;
		ERR(ae->GetCOLOR(ID_Color1+i, &p));
		if (err== PF_Err_NONE) {
			infoP->targets[i] = p;
		}

	}

	ERR(ae->GetCHECKBOX(ID_KEEP_PIXELS,&infoP->KeepPixel));
	ERR(ae->GetCHECKBOX(ID_FillUnknownColors, &infoP->FillUnknownColors));
	ERR(ae->GetCOLOR(ID_Fill_Color, &infoP->GiveUpColor8));
	infoP->GiveUpColor16 = CONV8TO16(infoP->GiveUpColor8);
	infoP->GiveUpColor32 = CONV8TO32(infoP->GiveUpColor8);

	ERR(ae->GetCHECKBOX(ID_MakeWhiteTransparent, &infoP->MakeWhiteTransparent));


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , FillInfo *infoP)
{
	PF_InData *in_data;
	in_data = ae->in_data;
	PF_Err	err = PF_Err_NONE;


	ERR(CellLineEraserSub(ae, infoP));
	
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
		FillInfo info;
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
	CFsAE ae(in_data,out_data,extraP,sizeof(FillInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		FillInfo *infoP = reinterpret_cast<FillInfo*>(ae.LockPreRenderData());
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
		FillInfo *infoP = reinterpret_cast<FillInfo*>(ae.LockPreRenderData());
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

