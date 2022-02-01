//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "colorThreshold.h"

#include <stdio.h>
#include <Math.h>
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
	PF_ADD_FLOAT_SLIDER(STR_ALPHA_LV,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						25,						//SLIDER_MIN
						75,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_ALPHA_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HS_LV,	//Name
						0,						//VALID_MIN
						10,						//VALID_MAX
						0,						//SLIDER_MIN
						1,						//SLIDER_MAX
						0,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_HS_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_L_LV,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						25,						//SLIDER_MIN
						75,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_L_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_TOPIC_NAIN_COL, ID_TOPIC_NAIN_COL);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MAIN_COL1_ON,
					STR_ON,
					TRUE,
					0,
					ID_MAIN_COL1_ON
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_MAIN_COL1, 
					10,	// Red
					10,	//Green
					10,	//Blue
					ID_MAIN_COL1
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MAIN_COL1_HS,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						50,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MAIN_COL1_HS
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MAIN_COL1_LV,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						50,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MAIN_COL1_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MAIN_COL2_ON,
					STR_ON,
					FALSE,
					0,
					ID_MAIN_COL2_ON
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_MAIN_COL2, 
					PF_MAX_CHAN8-1,	// Red
					PF_MAX_CHAN8-1,	//Green
					PF_MAX_CHAN8-1,	//Blue
					ID_MAIN_COL2
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MAIN_COL2_HS,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						10,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						1,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MAIN_COL2_HS
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MAIN_COL2_LV,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						100,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MAIN_COL2_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MAIN_COL3_ON,
					STR_ON,
					FALSE,
					0,
					ID_MAIN_COL3_ON
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_MAIN_COL3, 
					0,	// Red
					0,	//Green
					0,	//Blue
					ID_MAIN_COL3
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MAIN_COL3_HS,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						10,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						1,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MAIN_COL3_HS
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MAIN_COL3_LV,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						50,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MAIN_COL3_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_NAIN_COL_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_TOPIC_SUB_COL, ID_TOPIC_SUB_COL);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SUB_COL_COUNT,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					COUNT_OF_SUB,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					COUNT_OF_SUB,			//スライダーの最大値
					0,				//デフォルトの値
					ID_SUB_COL_COUNT
					);
	//----------------------------------------------------------------
	char num[255] = {"\0"};
	for (A_long i=0; i< COUNT_OF_SUB; i++){ 
#ifdef AE_OS_WIN
		sprintf_s(num,"UserColor_%d",i+1);
#else
		sprintf(num,"UserColor_%d",i+1);
#endif
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		PF_ADD_COLOR(	num, 
						0,	// Red
						0,	//Green
						0,	//Blue
						ID_SUB_COL(i)
						);
	}
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_SUB_COL_END);
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
static A_long findColor8(PF_Pixel *tbl, A_long cnt,PF_Pixel p)
{
	A_long ret = -1;
	if (cnt<=0) return ret;

	for (A_long i= 0; i<cnt; i++)
	{
		PF_Pixel tp = tbl[i];
		if ( (tp.red == p.red)&&(tp.green == p.green)&&(tp.blue == p.blue))
		{
			ret = i;
			break;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_ALPHA_LV,&infoP->alpha_level));
	infoP->alpha_level /= 100;
	infoP->alpha_border  = RoundByteFpLong((double)PF_MAX_CHAN8 * infoP->alpha_level+0.5);
	
	ERR(ae->GetFLOAT(ID_HS_LV,&infoP->hs_level));
	infoP->hs_level /= 100;
	ERR(ae->GetFLOAT(ID_L_LV,&infoP->l_level));
	infoP->l_level /= 100;

	
	PF_Pixel c;
	A_long idx = 0;
	PF_FpLong d;


	//ピクセルテーブルの初期化
	PF_Pixel bl = {0,0,0,0};
	PF_Pixel16 bl16 = {0,0,0,0};
	PF_Pixel32 bl32 = {0,0,0,0};
	for ( int i=0; i<COUNT_OF_MAIN;i++) {
		infoP->main_col[i] = bl;
		infoP->main_col16[i] = bl16;
		infoP->main_col32[i] = bl32;
	}
	for ( int i=0; i<COUNT_OF_SUB;i++) {
		infoP->sub_col[i] = bl;
		infoP->sub_col16[i] = bl16;
		infoP->sub_col32[i] = bl32;
	}
	idx = 0;
	for ( int i=0; i<COUNT_OF_MAIN; i++){
		PF_Boolean m;
		ERR(ae->GetCHECKBOX(ID_MAIN_COL_ON(i) ,&m));
		if ((!err)&&(m==TRUE)){
			ERR(ae->GetCOLOR(ID_MAIN_COL(i) ,&c));
			if (!err) {
				infoP->main_col[idx] = c;
				ERR(ae->GetFLOAT(ID_MAIN_COL_HS(i),&d));
				if(!err){
					infoP->main_hs[idx] = d /100;
					ERR(ae->GetFLOAT(ID_MAIN_COL_L(i),&d));
					if (!err) {
						infoP->main_l[idx] = d /100;
						idx++;
					}
				}
			}
		}
	}
	infoP->main_col_count = idx;
	if (infoP->main_col_count>0){
		for (int i=0; i<infoP->main_col_count; i++) {
			infoP->main_col16[i] =CONV8TO16(infoP->main_col[i]);
			infoP->main_col32[i] =CONV8TO32(infoP->main_col[i]);
			A_long a = (i<<2);
			infoP->main_col[i].alpha   = (A_u_char)a;
			infoP->main_col16[i].alpha   = (A_u_short)a;
			infoP->main_col32[i].alpha   = (PF_FpShort)((PF_FpShort)a / CT_FLOAT);
		}
	}

	ERR(ae->GetADD(ID_SUB_COL_COUNT,&infoP->sub_col_count));
	if(!err) {
		if (infoP->sub_col_count>0){
			if (infoP->sub_col_count>COUNT_OF_SUB)infoP->sub_col_count=COUNT_OF_SUB; 
			idx = 0;
			for ( A_long i=0; i<infoP->sub_col_count; i++){
				ERR(ae->GetCOLOR(ID_SUB_COL(i),&c));
				if(!err) {
					if (findColor8(infoP->sub_col,idx,c)<0){
						infoP->sub_col[idx] = c;
						idx++;
					}
				}
			}
			infoP->sub_col_count = idx;
		}
	}
	if (infoP->sub_col_count>0){
		for (int i=0; i<infoP->sub_col_count; i++) {
			infoP->sub_col[i].alpha		= (A_u_char)((i+COUNT_OF_MAIN) <<2);
			infoP->sub_col16[i] =CONV8TO16(infoP->sub_col[i]);
			infoP->sub_col32[i] =CONV8TO32(infoP->sub_col[i]);
			A_long a = ((i + COUNT_OF_MAIN) <<2);
			infoP->sub_col[i].alpha   = (A_u_char)a;
			infoP->sub_col16[i].alpha   = (A_u_short)a;
			infoP->sub_col32[i].alpha   = (PF_FpShort)((PF_FpShort)a / CT_FLOAT);
		}
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
	infoP->out = ae->out;
	A_long cnt = infoP->main_col_count + infoP->sub_col_count;  
	if (cnt<=0) return err;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,AlphaThreshold32));
		ERR(ae->iterate32((refconType)infoP,hslMatch32));
		if (!err){
			if (infoP->main_col_count>0){
				ERR(ae->iterate32((refconType)infoP,line_search32));
			}
		}
		if (!err){
			for ( A_long i=0; i<RETRY; i++){
				infoP->finFlag = TRUE;
				ERR(ae->iterate32((refconType)infoP,use_search32));
				ERR(ae->iterate32((refconType)infoP,use_searchSub32));
				if (infoP->finFlag)break;
			}
		}
		ERR(ae->iterate32((refconType)infoP,AlphaFix32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,AlphaThreshold16));
		ERR(ae->iterate16((refconType)infoP,hslMatch16));
		if (!err){
			if (infoP->main_col_count>0){
				ERR(ae->iterate16((refconType)infoP,line_search16));
			}
		}
		if (!err){
			for ( A_long i=0; i<RETRY; i++){
				infoP->finFlag = TRUE;
				ERR(ae->iterate16((refconType)infoP,use_search16));
				ERR(ae->iterate16((refconType)infoP,use_searchSub16));
				if (infoP->finFlag)break;
			}
		}
		ERR(ae->iterate16((refconType)infoP,AlphaFix16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,AlphaThreshold8));
		ERR(ae->iterate8((refconType)infoP,hslMatch8));
		if (!err){
			if (infoP->main_col_count>0){
				ERR(ae->iterate8((refconType)infoP,line_search8));
			}
		}
		
		if(!err){
			for ( A_long i=0; i<RETRY; i++){
				infoP->finFlag = TRUE;
				ERR(ae->iterate8((refconType)infoP,use_search8));
				ERR(ae->iterate8((refconType)infoP,use_searchSub8));
				if (infoP->finFlag)break;
			}
		}
		
		ERR(ae->iterate8((refconType)infoP,AlphaFix8));
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
