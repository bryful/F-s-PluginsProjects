//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "CellGrad.h"


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
	PF_ADD_TOPIC(STR_TARGET_TOPIC, ID_TARGET_TOPIC);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_TARGET_COUNT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		8,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		8,			//スライダーの最大値
		0,				//デフォルトの値
		ID_TARGET_COUNT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_TARGET_LEVEL,	//Name
		0,						//VALID_MIN
		20,						//VALID_MAX
		0,						//SLIDER_MIN
		10,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_TARGET_LEVEL
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL1,
		0xFF,
		0xFF,
		0xFF,
		ID_TARGET_COL1
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL2,
		0xFF,
		0x00,
		0x00,
		ID_TARGET_COL2
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL3,
		0x00,
		0xFF,
		0x00,
		ID_TARGET_COL3
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL4,
		0x00,
		0x00,
		0xFF,
		ID_TARGET_COL4
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL5,
		0xFF,
		0xFF,
		0x00,
		ID_TARGET_COL5
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL6,
		0xFF,
		0x00,
		0xFF,
		ID_TARGET_COL6
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL7,
		0x80,
		0x80,
		0x00,
		ID_TARGET_COL7
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_TARGET_COL8,
		0x80,
		0x00,
		0x80,
		ID_TARGET_COL8
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TARGET_TOPIC_END);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ANGLE, 0, ID_ANGLE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_START_OVER,	//Name
		-200,						//VALID_MIN
		400,						//VALID_MAX
		0,						//SLIDER_MIN
		200,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_START_OVER
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LAST_OVER,	//Name
		-200,						//VALID_MIN
		400,						//VALID_MAX
		0,						//SLIDER_MIN
		200,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LAST_OVER
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_START_COL,
		0xFF,
		0xFF,
		0xFF,
		ID_START_COL
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_LAST_COL,
		0x00,
		0x00,
		0x00,
		ID_LAST_COL
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_GUIDE_SHOW,
		STR_ON,
		FALSE,
		0,
		ID_GUIDE_SHOW
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_GUIDE_COLOR,
		0xFF,
		0x00,
		0x00,
		ID_GUIDE_COL
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
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_TARGET_COUNT, &infoP->targetCount));
	PF_FpLong lv;
	ERR(ae->GetFLOAT(ID_TARGET_LEVEL, &lv));
	if (!err)
	{
		infoP->tagetLevel = RoundByteFpLong(PF_MAX_CHAN8*lv/100 +0.5) ;
	}

	ERR(ae->GetCOLOR(ID_TARGET_COL1, &infoP->targetCol[0]));
	ERR(ae->GetCOLOR(ID_TARGET_COL2, &infoP->targetCol[1]));
	ERR(ae->GetCOLOR(ID_TARGET_COL3, &infoP->targetCol[2]));
	ERR(ae->GetCOLOR(ID_TARGET_COL4, &infoP->targetCol[3]));
	ERR(ae->GetCOLOR(ID_TARGET_COL5, &infoP->targetCol[4]));
	ERR(ae->GetCOLOR(ID_TARGET_COL6, &infoP->targetCol[5]));
	ERR(ae->GetCOLOR(ID_TARGET_COL7, &infoP->targetCol[6]));
	ERR(ae->GetCOLOR(ID_TARGET_COL8, &infoP->targetCol[7]));

	PF_Fixed a;
	ERR(ae->GetANGLE(ID_ANGLE, &a));
	if (!err)
	{
		a %= (360L << 16);
		if (a < 0) a += (360L << 16);
		infoP->angle = (PF_FpLong)a / (PF_FpLong)(1L << 16);
	}
	ERR(ae->GetFLOAT(ID_START_OVER, &infoP->startOver));
	if (!err)
	{
		infoP->startOver /= 100;
	}
	ERR(ae->GetFLOAT(ID_LAST_OVER, &infoP->lastOver));
	if (!err)
	{
		infoP->lastOver /= 100;
	}


	ERR(ae->GetCOLOR(ID_START_COL, &infoP->startCol));
	ERR(ae->GetCOLOR(ID_LAST_COL, &infoP->lastCol));
	
	ERR(ae->GetCHECKBOX(ID_GUIDE_SHOW, &infoP->guideShow));
	ERR(ae->GetCOLOR(ID_GUIDE_COL, &infoP->guideCol));

	return err;
}
//-------------------------------------------------------------------------------------------------
static LineInfo RotC(PF_InData* in_data, LineInfo li)
{

}

//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	FWorldGrad fw(ae->output, ae->in_data, ae->out->pixelFormat());

	if (infoP->targetCount > 0) {
		fw.SelectedPixel(infoP->targetCol, infoP->targetCount, infoP->tagetLevel);
	}
	fw.GradMask();
	fw.Min(3);
	A_LRect rct = fw.scanRect();

	FRectGrad rg(ae->in_data,rct,infoP->angle,infoP->startOver,infoP->lastOver);
	FGrad gra(ae->output, ae->in_data, ae->pixelFormat());
	gra.exec(rg.start(),rg.last(),infoP->startCol,infoP->lastCol);


	if (infoP->guideShow == TRUE)
	{
		ae->out->Line(
			rct.left, rct.top,
			rct.right, rct.top,
			infoP->guideCol
		);
		ae->out->Line(
			rct.right, rct.top,
			rct.right, rct.bottom,
			infoP->guideCol
		);
		ae->out->Line(
			rct.right, rct.bottom,
			rct.left, rct.bottom,
			infoP->guideCol
		);
		ae->out->Line(
			rct.left, rct.bottom,
			rct.left, rct.top,
			infoP->guideCol
		);

		ae->out->Line(
			(A_long)rg.start().x, (A_long)rg.start().y,
			(A_long)rg.last().x, (A_long)rg.last().y,
			infoP->guideCol
		);
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
