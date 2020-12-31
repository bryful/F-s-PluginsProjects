
//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "TouchDrawCenter.h"


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
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SEED,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					32000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					144,			//スライダーの最大値
					0,				//デフォルトの値
					ID_SEED
					);
	
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POINT(STR_CENTER,			/*"New Center"*/
				50,	// X
				50,	// Y
				0,	// Flag
				ID_CENTER
				);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_MAX,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_MAX
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_RND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_RND
					);
	//----------------------------------------------------------------
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_Y,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					3000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					30,				//デフォルトの値
					ID_Y
					);
	
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR, 
					0x00,
					0x00,
					0x00,
					ID_COLOR
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_OPACITY,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_OPACITY
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_OPACITY_RND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_OPACITY_RND
					);
	//----------------------------------------------------------------
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_POINT_COUNT,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					10,			//スライダーの最大値
					5,				//デフォルトの値
					ID_POINT_COUNT
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_POINT_LENGTH,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					30,			//スライダーの最大値
					10,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_POINT_LENGTH
					);
	
	
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_ORG_CB1,
					STR_ORG_CB2,
					TRUE,
					0,
					ID_ORG
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
	PF_Err 	err 	= PF_Err_NONE,
			err2 	= PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err CalcParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;
	//タッチ線の上下左右の発生回数を計算
	infoP->value_left =
	infoP->value_right =
	infoP->value_top =
	infoP->value_bottom = infoP->value;
	if (infoP->value>0){
		//まず上下から
		A_long c = ae->out->height()/2; //２分の一にしておく
		A_long p = (A_long)infoP->centeY;
		A_long vv=0;
		if ( (c/2)==(p/2)){
			//大体同じなら数を一緒とする。
		}else if (p<=0){
			infoP->value_bottom += infoP->value_top;
			infoP->value_top = 0;
		}else if ( p>=ae->out->height()-1){
			infoP->value_top += infoP->value_bottom;
			infoP->value_bottom = 0;
		}else if (p>c){
			vv = infoP->value_bottom * (p-c) /(ae->out->height() - c);
			infoP->value_bottom -= vv;
			infoP->value_top += vv;
		}else{
			vv = infoP->value_top * (c-p) /c;
			infoP->value_top -= vv;
			infoP->value_bottom += vv;
		}
		//まず左右から
		c = ae->out->width()/2; //２分の一にしておく
		p = (A_long)infoP->centeX;
		vv=0;
		if ( (c/2)==(p/2)){
			//大体同じなら数を一緒とする。
		}else if (p<=0){
			infoP->value_right += infoP->value_left;
			infoP->value_left = 0;
		}else if ( p>=ae->out->width()-1){
			infoP->value_left += infoP->value_right;
			infoP->value_right = 0;
		}else if (p>c){
			vv = infoP->value_right * (p-c) /(ae->out->width() - c);
			infoP->value_right -= vv;
			infoP->value_left += vv;
		}else{
			vv = infoP->value_left * (c-p) /c;
			infoP->value_left -= vv;
			infoP->value_right += vv;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_SEED,&infoP->seed));
	if (!err) if (infoP->seed<0) infoP->seed =0;
	PF_FixedPoint pt;
	ERR(ae->GetFIXEDPOINT(ID_CENTER,&pt));
	if (!err){
		infoP->centeX = (PF_FpShort)pt.x/(1L<<16);
		infoP->centeY = (PF_FpShort)pt.y/(1L<<16);
	}
	PF_FpShort s;
	ERR(ae->GetFIXED2FpShort(ID_LENGTH_MAX,&s));
	if (!err) {
		infoP->length_max = s/100;
		if (infoP->length_max<0) infoP->length_max=0; 
	}
	ERR(ae->GetFIXED2FpShort(ID_LENGTH_RND,&s));
	if (!err) {
		infoP->length_rnd = s/100;
		if (infoP->length_rnd<0) infoP->length_rnd=0; 
	}
	
	ERR(ae->GetADD(ID_Y,&infoP->value));
	if (!err) if (infoP->value<0) infoP->value=0;

	ERR(ae->GetCOLOR(ID_COLOR,&infoP->color));
	
	ERR(ae->GetFIXED2FpShort(ID_OPACITY,&s));
	if (!err) {
		infoP->opacity = s/100;
		if (infoP->opacity<0) infoP->opacity=0; 
	}
	ERR(ae->GetFIXED2FpShort(ID_OPACITY_RND,&s));
	if (!err) {
		infoP->opacity_rnd = s/100;
		if (infoP->opacity_rnd<0) infoP->opacity_rnd=0; 
	}
	ERR(ae->GetADD(ID_POINT_COUNT,&infoP->point_count));
	if (!err) if (infoP->point_count<0) infoP->point_count=0;

	ERR(ae->GetFIXED2FpShort(ID_POINT_LENGTH,&s));
	if (!err) {
		infoP->point_length = s * ae->in_data->downsample_x.num /ae->in_data->downsample_x.den;
		if (infoP->point_length<0) infoP->point_length=0; 
	}
	ERR(ae->GetCHECKBOX(ID_ORG,&infoP->org));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	
	ERR(ae->out->clear());
	if ( (infoP->length_max <=0)||(infoP->value<=0)||(infoP->opacity<=0)){
		if ( infoP->org) ERR(ae->CopyInToOut());
		return err;
	}
	/*
	infoP->centeX -= ae->output->origin_x;
	infoP->centeY -= ae->output->origin_y;
	*/
	
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		drawTouch32(ae->out,infoP);
		AlphaBlur32(ae->out);
		copyImageInfo32 ci32;
		ci32.col = CONV8TO32( infoP->color);
		ci32.org = infoP->org;
		ERR(ae->iterate32((refconType)&ci32,CopyImage32));
		break;
	case PF_PixelFormat_ARGB64:
		drawTouch16(ae->out,infoP);
		AlphaBlur16(ae->out);
		copyImageInfo16 ci16;
		ci16.col = CONV8TO16( infoP->color);
		ci16.org = infoP->org;
		ERR(ae->iterate16((refconType)&ci16,CopyImage16));
		break;
	case PF_PixelFormat_ARGB32:
		drawTouch8(ae->out,infoP);
		AlphaBlur8(ae->out);
		copyImageInfo8 ci8;
		ci8.col = infoP->color;
		ci8.org = infoP->org;
		ERR(ae->iterate8((refconType)&ci8,CopyImage8));
		break;
	}
	
	return err;

}
//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
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
		CalcParams(&ae,&info);
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
			CalcParams(&ae,infoP);
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


#include "Fs_Entry.h"
