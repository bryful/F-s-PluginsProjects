//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "grayToWaveLine.h"


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
	PF_ADD_SLIDER(STR_SXCOUNT,	//パラメータの名前
		50, 					//数値入力する場合の最小値
		2000,					//数値入力する場合の最大値
		100,					//スライダーの最小値 
		500,					//スライダーの最大値
		300,					//デフォルトの値
		ID_SXCount
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_SYCOUNT,	//パラメータの名前
		1, 					//数値入力する場合の最小値
		1000,					//数値入力する場合の最大値
		1,					//スライダーの最小値 
		50,					//スライダーの最大値
		20,					//デフォルトの値
		ID_SYCount
	);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_DRAW_POS,
		25,	// X
		15,	// Y
		0,	// Flag
		ID_DRAW_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_GRAPH_XSCALE,	//パラメータの名前
		1, 					//数値入力する場合の最小値
		100,				//数値入力する場合の最大値
		2,					//スライダーの最小値 
		50,					//スライダーの最大値
		5,					//デフォルトの値
		ID_GRAPH_XSCALE
	);	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_GRAPH_YMAX,	//パラメータの名前
		-1000, 					//数値入力する場合の最小値
		1000,					//数値入力する場合の最大値
		-200,					//スライダーの最小値 
		200,					//スライダーの最大値
		100,					//デフォルトの値
		ID_GRAPH_YMAX
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_TILT,	//パラメータの名前
		-1000, 					//数値入力する場合の最小値
		1000,					//数値入力する場合の最大値
		-200,					//スライダーの最小値 
		200,					//スライダーの最大値
		0,					//デフォルトの値
		ID_TILT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_X_OFFSET,	//パラメータの名前
		-1000, 					//数値入力する場合の最小値
		1000,					//数値入力する場合の最大値
		-500,					//スライダーの最小値 
		500,					//スライダーの最大値
		0,					//デフォルトの値
		ID_X_OFFSET
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_Y_OFFSET,	//パラメータの名前
		-1000, 					//数値入力する場合の最小値
		1000,					//数値入力する場合の最大値
		-500,					//スライダーの最小値 
		500,					//スライダーの最大値
		0,					//デフォルトの値
		ID_Y_OFFSET
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LINE_HEIGHT,	//パラメータの名前
		1, 					//数値入力する場合の最小値
		20,					//数値入力する場合の最大値
		1,					//スライダーの最小値 
		6,					//スライダーの最大値
		3,					//デフォルトの値
		ID_LINEHEIGHT
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_GRAPH_COLOR,
		0xFF,
		0xFF,
		0xFF,
		ID_GRAPH_COLOR
	);	
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_BASE_COLOR,
		0x80,
		0x80,
		0x80,
		ID_BASE_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(STR_MODE,
		STR_MODE_COUNT,	//メニューの数
		STR_MODE_DFLT,	//デフォルト
		STR_MODE_ITEM,
		ID_MODE
	);	out_data->num_params = 	ID_NUM_PARAMS;

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
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	//ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_SXCount, &infoP->SXCount));
	if (infoP->SXCount <= 0) infoP->SXCount = 1;
	ERR(ae->GetADD(ID_SYCount, &infoP->SYCount));
	if (infoP->SYCount <= 0) infoP->SYCount = 1;
	PF_FixedPoint	pnt;
	ERR(ae->GetFIXEDPOINT(ID_DRAW_POS, &pnt));
	if (!err) {
		infoP->Draw_X = (pnt.x >>16L);
		infoP->Draw_Y = (pnt.y >>16L);
	}
	ERR(ae->GetADD(ID_GRAPH_XSCALE, &infoP->Graph_XScale));
	ERR(ae->GetADD(ID_GRAPH_YMAX, &infoP->Graph_YMax));

	ERR(ae->GetADD(ID_TILT, &infoP->Graph_Tilt));

	ERR(ae->GetADD(ID_X_OFFSET, &infoP->Draw_X_Offset));
	ERR(ae->GetADD(ID_Y_OFFSET, &infoP->Draw_Y_Offset));

	ERR(ae->GetADD(ID_LINEHEIGHT, &infoP->LineHeight));
	ERR(ae->GetCOLOR(ID_GRAPH_COLOR, &infoP->Graph_Color));
	ERR(ae->GetCOLOR(ID_BASE_COLOR, &infoP->Base_Color));
	ERR(ae->GetPOPUP(ID_MODE, &infoP->mode));

	return err;
}
//-------------------------------------------------------------------------------------------------
static ParamInfo16 ToParamInfo16(ParamInfo* infoP)
{
	ParamInfo16 ret;
	ret.SXCount = infoP->SXCount;
	ret.SYCount = infoP->SYCount;
	ret.Draw_X = infoP->Draw_X;
	ret.Draw_Y = infoP->Draw_Y;
	ret.Graph_XScale = infoP->Graph_XScale;
	ret.Graph_YMax = infoP->Graph_YMax;
	ret.Graph_Tilt = infoP->Graph_Tilt;

	ret.Graph_Color = CONV8TO16( infoP->Graph_Color);
	ret.Base_Color = CONV8TO16(infoP->Base_Color);

	ret.LineHeight = infoP->LineHeight;
	ret.Draw_X_Offset = infoP->Draw_X_Offset;
	ret.Draw_Y_Offset = infoP->Draw_Y_Offset;

	ret.SampleDataH = infoP->SampleDataH;
	ret.SampleData = infoP->SampleData;
	ret.mode = infoP->mode;

	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ae->out->clear();


	if (infoP->SXCount > ae->out->width()) infoP->SXCount = ae->out->width();
	if (infoP->SYCount > ae->out->height()) infoP->SYCount = ae->out->height();


	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ae->CopyInToOut();
		break;
	case PF_PixelFormat_ARGB64:
		ParamInfo16 info16 = ToParamInfo16(infoP);
		ERR(exec16(ae, &info16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(exec8(ae, infoP));
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
