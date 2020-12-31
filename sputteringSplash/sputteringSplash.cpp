//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "sputteringSplash.h"


//-----------------------------------------------------------------------------
static PF_Err 
ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;
	
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	UI_SEED,		//パラメータの名前
					0, 				//数値入力する場合の最小値
					32000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					144,			//スライダーの最大値
					0,				//デフォルトの値
					ID_SEED
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_VALUE,		//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					50,				//デフォルトの値
					1,				//表示される小数の桁数
					0,
					0,
					ID_Y
					);
	//-----------------
	//ポップアップ
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(	UI_DIR1, 
					UI_DIR_COUNT,	//メニューの数
					UI_DIR_DFLT,	//デフォルト
					UI_DIR2,
					ID_DIR
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	UI_OPA_RND,		//パラメータの名前
					0, 				//数値入力する場合の最小値
					5,				//数値入力する場合の最大値
					0,				//スライダーの最小値 
					2,				//スライダーの最大値
					1,				//デフォルトの値
					2,				//表示される小数の桁数
					0,
					0,
					ID_OPACITY_RAND
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	UI_POINT_VALUE,		//パラメータの名前
					1, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					50,			//スライダーの最大値
					10,				//デフォルトの値
					ID_POINT_VALUE
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	UI_POINT_LENGTH,		//パラメータの名前
					1, 			//数値入力する場合の最小値
					1000,			//数値入力する場合の最大値
					5,				//スライダーの最小値 
					200,			//スライダーの最大値
					15,				//デフォルトの値
					ID_POINT_LENGTH
					);
	//-----------------
	//ポップアップ
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(	UI_SIZE1, 
					UI_SIZE_COUNT,	//メニューの数
					UI_SIZE_DFLT,	//デフォルト
					UI_SIZE2,
					ID_SIZE
					);

	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_COLOR1, 
					0xD6,	// Red D6E6FF
					0xE6,	//Green
					0xFF,	//Blue
					ID_COLOR1
					);
	
	//-----------------
	//トピック表示の開始
	AEFX_CLR_STRUCT(def);	
	def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//
	PF_ADD_TOPIC(	UI_COLOR_TOPIC,
					ID_COLOR_TOPIC
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	UI_COLOR_MAX,		//パラメータの名前
					1, 				//数値入力する場合の最小値
					UI_COLOR_MAX_V,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					UI_COLOR_MAX_V,			//スライダーの最大値
					20,				//デフォルトの値
					ID_COLOR_MAX
					);

	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_COLOR2, 
					0x99,	// Red99C7FF
					0xC7,	//Green
					0xFF,	//Blue
					ID_COLOR2
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_COLOR3, 
					0x52,	// Red52A3FF
					0xA3,	//Green
					0xFF,	//Blue
					ID_COLOR3
					);
	//-----------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	UI_COLOR4, 
					0x04,	// Red048D
					0x8D,	//Green
					0xE3,	//Blue
					ID_COLOR4
					);
	//-----------------
	//トピック表示の終わり
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_COLOR_TOPIC_END);
	//-----------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(UI_ORG1,
					UI_ORG2,
					FALSE,
					0,
					ID_ORG
					);

	//-----------------
	out_data->num_params = ID_NUM_PARAMS;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	ERR(ae->GetADD(ID_SEED,&infoP->seed));
	ERR(ae->GetFIXED_PER2ONE(ID_Y,&infoP->value));
	ERR(ae->GetPOPUP(ID_DIR,&infoP->dir));
	ERR(ae->GetFIXED(ID_OPACITY_RAND,&infoP->opa));

	ERR(ae->GetADD(ID_POINT_VALUE,&infoP->point_value));
	ERR(ae->GetADD(ID_POINT_LENGTH,&infoP->point_length));
	if (!err){
		infoP->point_length = ae->downScale(infoP->point_length);
		if (infoP->point_length<2)  infoP->point_length = 2;
	}
	ERR(ae->GetPOPUP(ID_SIZE,&infoP->size));
	ERR(ae->GetADD(ID_COLOR_MAX,&infoP->color_max));

	ERR(ae->GetCOLOR(ID_COLOR1,&infoP->colors[0]));
	ERR(ae->GetCOLOR(ID_COLOR2,&infoP->colors[1]));
	ERR(ae->GetCOLOR(ID_COLOR3,&infoP->colors[2]));
	ERR(ae->GetCOLOR(ID_COLOR4,&infoP->colors[3]));

	ERR(ae->GetCHECKBOX(ID_ORG,&infoP->org));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if (infoP->org == TRUE) {
		ERR( ae->CopyInToOut());
	}else{
		ERR(ae->out->clear());
	}
	if (infoP->value>0){
		CFsBuffer buf = ae->NewBuffer((SPD_RAND_MAX+3)*sizeof(A_u_char));// + SPD_RAND_MAX
		if (buf.alive()==FALSE){
			ae->out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
			return err;
		}
		CFsBuffer buf1 = ae->NewBuffer(ae->out->width() * (ae->out->height()+8) *sizeof(A_u_char));// + SPD_RAND_MAX
		if (buf1.alive()==FALSE){
			buf.Dispose();
			ae->out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
			return err;
		}
		infoP->sputRandTable = buf.bufA_u_char();
		SetupSputData(infoP->size,infoP->sputRandTable);
		infoP->buf = buf1.bufA_u_char();

		F_SRAND(infoP->seed);	
		switch(ae->pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			ERR(MainRender32(ae,infoP));
			break;
		case PF_PixelFormat_ARGB64:
			ERR(MainRender16(ae,infoP));
			break;
		case PF_PixelFormat_ARGB32:
			ERR(MainRender8(ae,infoP));
			break;
		}
		buf.Dispose();
		buf1.Dispose();
	}
	return err;
}

//-----------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------
