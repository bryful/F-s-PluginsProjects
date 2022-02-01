//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "PluginSkeleton.h"


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
	PF_ADD_FLOAT_SLIDER(STR_R,	//Name
						-2,						//VALID_MIN
						2,						//VALID_MAX
						-1,						//SLIDER_MIN
						1,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_R
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_G,	//Name
						-2,						//VALID_MIN
						2,						//VALID_MAX
						-1,						//SLIDER_MIN
						1,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_G
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_B,	//Name
						-2,						//VALID_MIN
						2,						//VALID_MAX
						-1,						//SLIDER_MIN
						1,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_B
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_NOISE,	//Name
						0,							//VALID_MIN
						1,						//VALID_MAX
						0,							//SLIDER_MIN
						1,							//SLIDER_MAX
						0,							//CURVE_TOLERANCE
						0,							//DFLT
						1,							//PREC
						0,							//DISP
						0,							//WANT_PHASE
						ID_FLOAT_SLIDER
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_NOISE_FRAME1,
					STR_NOISE_FRAME2,
					TRUE,
					0,
					ID_NOISE_FRAME
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(	STR_NOISE_OFFSET,	//パラメータの名前
					-30000, 		//数値入力する場合の最小値
					30000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					300,			//スライダーの最大値
					10,				//デフォルトの値
					ID_NOISE_OFFSET
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_HIDDEN_ON1,
					STR_HIDDEN_ON2,
					FALSE,
					0,
					ID_HIDDEN_ON
					);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	def.flags 	= 	PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC(STR_TOPIC, ID_TOPIC);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COLOR, 
					0xFF,
					0xFF,
					0xFF,
					ID_COLOR
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_INTERP;	//キーフレームの自動保管を停止する
	PF_ADD_SLIDER(	STR_ADD_SLIDER,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					10,				//デフォルトの値
					ID_ADD_SLIDER
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FIXED(	STR_FIXED_SLIDER,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_FIXED_SLIDER
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_FLOAT_SLIDER,	//Name
						-10000,							//VALID_MIN
						10000,						//VALID_MAX
						0,							//SLIDER_MIN
						1000,						//SLIDER_MAX
						1,							//CURVE_TOLERANCE
						500,						//DFLT
						1,							//PREC
						0,							//DISP
						0,							//WANT_PHASE
						ID_FLOAT_SLIDER
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CHECKBOX1,
					STR_CHECKBOX2,
					FALSE,
					0,
					ID_CHECKBOX
					);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ANGLE,0,ID_ANGLE); 

	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_POPUP, 
						STR_POPUP_COUNT,	//メニューの数
						STR_POPUP_DFLT,	//デフォルト
						STR_POPUP_ITEMS,
						ID_POPUP
						);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POINT(STR_POINT,			/*"New Center"*/
				50,	// X
				50,	// Y
				0,	// Flag
				ID_POINT
				);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_END);

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

	try{
		CFsAE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
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
				for ( A_long i=1; i<ID_NUM_PARAMS; i++) 
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

	PF_Pixel32 c;
	c = CONV8TO32(*outP);
	
	c.red	+= (PF_FpShort)niP->r;
	c.green	+= (PF_FpShort)niP->g;
	c.blue	+= (PF_FpShort)niP->b;

	if ( niP->noise>0){
		A_long v = (A_long)(PF_MAX_CHAN16 * niP->noise);
		v = F_RAND2(-v,v);
		PF_FpShort vv = (PF_FpShort)v / PF_MAX_CHAN16;
		c.red	+= vv;
		c.green	+= vv;
		c.blue	+= vv;
	}


	PF_Pixel c8	= CONV32TO8(c);
	outP->alpha	= inP->alpha;
	outP->red	= c8.red;
	outP->green	= c8.green;
	outP->blue	= c8.blue;

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

	PF_Pixel32 c;
	c = CONV16TO32(*outP);
	c.red	+= (PF_FpShort)niP->r;
	c.green	+= (PF_FpShort)niP->g;
	c.blue	+= (PF_FpShort)niP->b;

	if ( niP->noise>0){
		A_long v = (A_long)(PF_MAX_CHAN16 * niP->noise);
		v = F_RAND2(-v,v);
		PF_FpShort vv = (PF_FpShort)v / PF_MAX_CHAN16;
		c.red	+= vv;
		c.green	+= vv;
		c.blue	+= vv;
	}


	PF_Pixel16 c16	= CONV32TO16(c);
	outP->red	= c16.red;
	outP->green	= c16.green;
	outP->blue	= c16.blue;

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

	PF_PixelFloat c;
	c.red	= (PF_FpShort)(outP->red	+ niP->r);
	c.green	= (PF_FpShort)(outP->green	+ niP->g);
	c.blue	= (PF_FpShort)(outP->blue	+ niP->b);

	if ( niP->noise>0){
		A_long v = (A_long)(PF_MAX_CHAN16 * niP->noise);
		v = F_RAND2(-v,v);
		PF_FpShort vv = (PF_FpShort)v / PF_MAX_CHAN16;
		c.red	+= vv;
		c.green	+= vv;
		c.blue	+= vv;
	}
	
	outP->red	= c.red;
	outP->green	= c.green;
	outP->blue	= c.blue;

	return err;
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
		ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
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
