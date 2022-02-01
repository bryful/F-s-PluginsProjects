//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "TouchDraw.h"



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
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_TARGET_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					25,			//スライダーの最大値
					5,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_TARGET_VALUE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	def.flags 	= 	PF_ParamFlag_START_COLLAPSED;
	PF_ADD_TOPIC(STR_TARGET_TOPIC, ID_TARGRT_TOPIC);
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_TARGET_MODE1, 
						3,	//メニューの数
						2,	//デフォルト
						STR_TARGET_MODE2,
						ID_TARGET_MODE
						);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COLOR, 
					0x00,
					0x00,
					0x00,
					ID_TARGET_COLOR
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_COLOR_RANGE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					25,				//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_COLOR_RANGE
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_DELTA_RANGE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					90,				//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_DELTA_RANGE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TARGRT_TOPIC_END);
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
	PF_ADD_FIXED(	STR_LENGTH_I_MAX,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_I_MAX
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_I_RND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_I_RND
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_O_MAX,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					10,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_O_MAX
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_O_RND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_O_RND
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
					100,			//数値入力する場合の最大値
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
					100,			//数値入力する場合の最大値
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
					2,				//デフォルトの値
					ID_POINT_COUNT
					);
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_POINT_LENGTH,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					10,			//スライダーの最大値
					1,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_POINT_LENGTH
					);
	
	//----------------------------------------------------------------
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_BLOCK_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_BLOCK_VALUE
					);
	//----------------------------------------------------------------
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_BLOCK_SIZE,	//パラメータの名前
					8, 				//数値入力する場合の最小値
					128,			//数値入力する場合の最大値
					8,				//スライダーの最小値 
					64,			//スライダーの最大値
					2,				//デフォルトの値
					ID_BLOCK_SIZE
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
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_SEED,&infoP->seed));
	ERR(ae->GetFIXED2FpShort_P1(ID_TARGET_VALUE,&infoP->target_value));
	ERR(ae->GetPOPUP(ID_TARGET_MODE,&infoP->target_mode));
	ERR(ae->GetCOLOR(ID_TARGET_COLOR,&infoP->target_color));
	ERR(ae->GetFIXED_PER2ONE(ID_COLOR_RANGE,&infoP->color_range));
	ERR(ae->GetFIXED_PER2ONE(ID_DELTA_RANGE,&infoP->delta_range));
	ERR(ae->GetFIXEDPOINT(ID_CENTER,&infoP->center));
	ERR(ae->GetFIXED_PER2ONE(ID_LENGTH_I_MAX,&infoP->length_i_max));
	ERR(ae->GetFIXED_PER2ONE(ID_LENGTH_I_RND,&infoP->length_i_rnd));
	ERR(ae->GetFIXED_PER2ONE(ID_LENGTH_O_MAX,&infoP->length_o_max));
	ERR(ae->GetFIXED_PER2ONE(ID_LENGTH_O_RND,&infoP->length_o_rnd));

	ERR(ae->GetCOLOR(ID_COLOR,&infoP->color));
	ERR(ae->GetFIXED_PER2ONE(ID_OPACITY,&infoP->opacity));
	ERR(ae->GetFIXED_PER2ONE(ID_OPACITY_RND,&infoP->opacity_rnd));

	ERR(ae->GetADD(ID_POINT_COUNT,&infoP->point_count));
	ERR(ae->GetFIXED(ID_POINT_LENGTH,&infoP->point_length));
	if (!err) {
		infoP->point_length = ae->downScaleNoClip(infoP->point_length);
		infoP->point_length += (1L<<16);
	}
	infoP->target_count = 0;
	ERR(ae->GetFIXED_PER2ONE(ID_BLOCK_VALUE,&infoP->block_value));
	ERR(ae->GetADD(ID_BLOCK_SIZE,&infoP->block_size));
	if (!err) {
		infoP->block_size = ae->downScaleNoClip(infoP->block_size);
		if ( infoP->block_size<2) infoP->block_size = 2;
	}
	A_long pW = ae->in_data->width;
	A_long pH = ae->in_data->height;
	if ((pW % infoP->block_size)!=0) { pW = pW/infoP->block_size +1;}else{pW = pW/infoP->block_size;}
	if ((pH % infoP->block_size)!=0) { pH = pH/infoP->block_size +1;}else{pH = pH/infoP->block_size;}
	infoP->block_count = pW * pH * infoP->block_value >> 16;

	ERR(ae->GetCHECKBOX(ID_ORG,&infoP->org));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	
	ae->out->clear();
	switch (infoP->target_mode)
	{
	case 1:
		FindTarget(ae,infoP->target_color,infoP->color_range);
		break;
	case 2:
		FindTargetDeltaBright(ae,infoP->delta_range);
		break;
	case 3:
		FindTargetDeltaAlpha(ae,infoP->delta_range);
		break;
	}

	F_SRAND(infoP->seed);	
	blockDraw(ae,infoP->block_size,infoP->block_count);
	
	F_SRAND(infoP->seed);	
	switch(ae->pixelFormat()){
	case PF_PixelFormat_ARGB128:
		drawTouch32(ae->out,infoP);
		break;
	case PF_PixelFormat_ARGB64:
		drawTouch16(ae->out,infoP);
		break;
	case PF_PixelFormat_ARGB32:
		drawTouch8(ae->out,infoP);
		break;
	}
	greenBlur(ae);
	
	copyAlpha(ae,infoP->color,infoP->org);
	
	return err;
}


//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (
	PF_InData			*in_data,
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