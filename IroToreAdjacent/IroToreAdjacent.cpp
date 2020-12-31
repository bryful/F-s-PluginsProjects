//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "IroToreAdjacent.h"


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
	PF_ADD_CHECKBOX(STR_LINE_ONLY,
					STR_ON,
					FALSE,
					0,
					ID_LINE_ONLY
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_NEW_COLOR, 
					0x80,
					0x00,
					0x00,
					ID_NEW_COLOR
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_Y,	//パラメータの名前
					0, 			//数値入力する場合の最小値
					50,			//数値入力する場合の最大値
					0,			//スライダーの最小値 
					10,			//スライダーの最大値
					6,			//デフォルトの値
					ID_Y
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_LINE_MINMAX,	//パラメータの名前
					-100, 		//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					-10,				//スライダーの最小値 
					10,			//スライダーの最大値
					-2,				//デフォルトの値
					ID_LINE_MINMAX
					);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_LINE_BLUR,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					50,			//数値入力する場合の最大値
					00,				//スライダーの最小値 
					10,			//スライダーの最大値
					10,				//デフォルトの値
					ID_LINE_BLUR
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MN_COLOR1_ON,
					STR_ON,
					TRUE,
					0,
					ID_MN_COLOR1_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_MN_COLOR1, 
					0x0F,
					0x0F,
					0x0F,
					ID_MN_COLOR1
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MN_COLOR2_ON,
					STR_ON,
					TRUE,
					0,
					ID_MN_COLOR2_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_MN_COLOR2, 
					0x0A,
					0x0A,
					0x0A,
					ID_MN_COLOR2
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LEVEL,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						10,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_LEVEL
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AD_COLOR1_ON,
					STR_ON,
					FALSE,
					0,
					ID_AD_COLOR1_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_AD_COLOR1, 
					0xFF,
					0x00,
					0x00,
					ID_AD_COLOR1
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AD_COLOR2_ON,
					STR_ON,
					FALSE,
					0,
					ID_AD_COLOR2_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_AD_COLOR2, 
					0xFF,
					0x00,
					0x00,
					ID_AD_COLOR2
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AD_COLOR3_ON,
					STR_ON,
					FALSE,
					0,
					ID_AD_COLOR3_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_AD_COLOR3, 
					0xFF,
					0x00,
					0x00,
					ID_AD_COLOR3
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AD_COLOR4_ON,
					STR_ON,
					FALSE,
					0,
					ID_AD_COLOR4_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_AD_COLOR4, 
					0xFF,
					0x00,
					0x00,
					ID_AD_COLOR4
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AD_COLOR5_ON,
					STR_ON,
					FALSE,
					0,
					ID_AD_COLOR5_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_AD_COLOR5, 
					0xFF,
					0x00,
					0x00,
					ID_AD_COLOR5
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_AD_COLOR6_ON,
					STR_ON,
					FALSE,
					0,
					ID_AD_COLOR6_ON
					);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_AD_COLOR6, 
					0xFF,
					0x00,
					0x00,
					ID_AD_COLOR6
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
	PF_Err				err					= PF_Err_NONE;
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
static PF_Boolean FindMnColor(ParamInfo *infoP, PF_Pixel p)
{
	PF_Boolean ret = FALSE;
	if (infoP->mnColorMax >0 ) {
		for ( int i=0; i<infoP->mnColorMax; i++){
			if (
				(p.blue == infoP->mnColor[i].blue)
				&&(p.green == infoP->mnColor[i].green)
				&&(p.red == infoP->mnColor[i].red)){
			
				ret = TRUE;
				break;
			}

		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Boolean FindAdColor(ParamInfo *infoP, PF_Pixel p)
{
	PF_Boolean ret = FALSE;
	if (infoP->adColorMax >0 ) {
		for ( int i=0; i<infoP->adColorMax; i++){
			if (
				(p.blue == infoP->adColor[i].blue)
				&&(p.green == infoP->adColor[i].green)
				&&(p.red == infoP->adColor[i].red)){
			
				ret = TRUE;
				break;
			}

		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;



	ERR(ae->GetCHECKBOX(ID_LINE_ONLY,&infoP->lineOnly));

	ERR(ae->GetCOLOR(ID_NEW_COLOR,&infoP->newColor));
	infoP->newColor.alpha = TARGET_NEW8;

	ERR(ae->GetADD(ID_Y,&infoP->value));
	ERR(ae->GetADD(ID_LINE_MINMAX,&infoP->minmax));
	ERR(ae->GetADD(ID_LINE_BLUR,&infoP->blur));


	PF_Boolean on = FALSE;
	int idx = 0;
	PF_Pixel p = {0,0,0,0};
	infoP->mnColorMax = 0;
	ERR(ae->GetCHECKBOX(ID_MN_COLOR1_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_MN_COLOR1,&p));
		infoP->mnColor[idx] = p;
		idx++;
	}
	ERR(ae->GetCHECKBOX(ID_MN_COLOR2_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_MN_COLOR2,&p));
		if ( FindMnColor(infoP,p) == FALSE) { 
			infoP->mnColor[idx] = p;
			idx++;
		}
	}
	infoP->mnColorMax = idx;

	double lv = 0;
	ERR(ae->GetFLOAT(ID_LEVEL,&lv));
	infoP->level = (A_long)((double)PF_MAX_CHAN8 * lv / 100.0 + 0.5);	

	idx = 0;
	infoP->adColorMax = 0;

	ERR(ae->GetCHECKBOX(ID_AD_COLOR1_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_AD_COLOR1,&p));
		if ( FindMnColor(infoP,p)==FALSE) { 
			infoP->adColor[idx] = p;
			idx++;
		}		
	}
	ERR(ae->GetCHECKBOX(ID_AD_COLOR2_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_AD_COLOR2,&p));
		if ( FindMnColor(infoP,p)==FALSE) { 
			infoP->adColor[idx] = p;
			idx++;
		}		
	}
	ERR(ae->GetCHECKBOX(ID_AD_COLOR3_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_AD_COLOR3,&p));
		if ( FindMnColor(infoP,p)==FALSE) { 
			if ( FindAdColor(infoP,p)==FALSE) { 
				infoP->adColor[idx] = p;
				idx++;
			}
		}		
	}
	ERR(ae->GetCHECKBOX(ID_AD_COLOR4_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_AD_COLOR4,&p));
		if ( FindMnColor(infoP,p)==FALSE) { 
			if ( FindAdColor(infoP,p)==FALSE) { 
				infoP->adColor[idx] = p;
				idx++;
			}
		}		
	}
	ERR(ae->GetCHECKBOX(ID_AD_COLOR5_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_AD_COLOR5,&p));
		if ( FindMnColor(infoP,p)==FALSE) { 
			if ( FindAdColor(infoP,p)==FALSE) { 
				infoP->adColor[idx] = p;
				idx++;
			}
		}		
	}
	ERR(ae->GetCHECKBOX(ID_AD_COLOR6_ON,&on));
	if (on==TRUE) {
		ERR(ae->GetCOLOR(ID_AD_COLOR6,&p));
		if ( FindMnColor(infoP,p)==FALSE) { 
			if ( FindAdColor(infoP,p)==FALSE) { 
				infoP->adColor[idx] = p;
				idx++;
			}
		}		
	}
	infoP->adColorMax = idx;
	return err;
}
//###########################################################################

static ParamInfo16 
	infoTo16 (ParamInfo *infoP)
{
	ParamInfo16 ret;
	ret.lineOnly = infoP->lineOnly;
	ret.newColor	= CONV8TO16(infoP->newColor);
	ret.newColor.alpha = TARGET_NEW16;
	
	ret.mnColorMax = infoP->mnColorMax;
	if (infoP->mnColorMax>0){
		for (int i=0;i<infoP->mnColorMax;i++){
			ret.mnColor[i] = infoP->mnColor[i];
		}
	}
	ret.adColorMax = infoP->adColorMax;
	if (infoP->adColorMax>0){
		for (int i=0;i<infoP->adColorMax;i++){
			ret.adColor[i] = infoP->adColor[i];
		}
	}


	ret.value = infoP->value;
	ret.level = infoP->level;
	ret.minmax = infoP->minmax;
	ret.blur = infoP->blur;
	ret.mnColor16 = CONV8TO16(infoP->mnColor[0]);
	ret.mnColor16.alpha = TARGET_MAIN8;
	return ret;

}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	if ((infoP->mnColorMax<=0)||(infoP->adColorMax<=0)||(infoP->value<=0))
	{
		return err;
	}
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//ParamInfo32 info32;

		//info32 = infoTo32(infoP);
		//ERR(irotoreExec32(ae,&info32));
		break;
	case PF_PixelFormat_ARGB64:
		ParamInfo16 info16;

		info16 = infoTo16(infoP);
		ERR(irotoreExec16(ae,&info16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(irotoreExec8(ae,infoP));
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
