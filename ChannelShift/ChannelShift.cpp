//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "ChannelShift.h"


/*
PF_Pixel (*getPixel8_sub)(getPixelPrm *prm,A_long x,A_long y);
PF_Pixel16 (*getPixel16_sub)(getPixelPrm *prm,A_long x,A_long y);
#if defined(SUPPORT_SMARTFX)
PF_PixelFloat (*getPixel32_sub)(getPixelPrm *prm,A_long x,A_long y);
#endif
*/

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (PF_InData		*in_data,
					PF_OutData		*out_data,
					PF_ParamDef		*params[],
					PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;
	PF_ADD_TOPIC(STR_RED_TOPIC, ID_RED_TOPIC);

	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(	STR_RED_ROT, 
					0,
					ID_RED_ROT
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_RED_LENGTH,	//パラメータの名前
					-30000, 				//数値入力する場合の最小値
					30000,	//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,		//スライダーの最大値
					0,		//デフォルトの値
					1,//数値表示に関するフラグ 
					0,
					0,
					ID_RED_LENGTH
					);
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_RED_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;
	PF_ADD_TOPIC(STR_GREEN_TOPIC, ID_GREEN_TOPIC);

	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(	STR_GREEN_ROT, 
					90.0,
					ID_GREEN_ROT
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_GREEN_LENGTH,	//パラメータの名前
					-30000, 				//数値入力する場合の最小値
					30000,	//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,		//スライダーの最大値
					0,		//デフォルトの値
					1,//数値表示に関するフラグ 
					0,
					0,
					ID_GREEN_LENGTH
					);
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_GREEN_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;
	PF_ADD_TOPIC(STR_BLUE_TOPIC, ID_BLUE_TOPIC);

	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(	STR_BLUE_ROT, 
					180.0,
					ID_BLUE_ROT
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_BLUE_LENGTH,	//パラメータの名前
					-30000, 				//数値入力する場合の最小値
					30000,	//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,		//スライダーの最大値
					0,		//デフォルトの値
					1,//数値表示に関するフラグ 
					0,
					0,
					ID_BLUE_LENGTH
					);
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_BLUE_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	//def.flags 	= 	PF_ParamFlag_START_COLLAPSED;
	PF_ADD_TOPIC(STR_ALPHA_TOPIC, ID_ALPHA_TOPIC);

	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(	STR_ALPHA_ROT, 
					270.0,
					ID_ALPHA_ROT
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_ALPHA_LENGTH,	//パラメータの名前
					-30000, 				//数値入力する場合の最小値
					30000,	//数値入力する場合の最大値
					-100,				//スライダーの最小値 
					100,		//スライダーの最大値
					0,		//デフォルトの値
					1,//数値表示に関するフラグ 
					0,
					0,
					ID_ALPHA_LENGTH
					);
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_ALPHA_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_EDGE_POP1, 
						4,	//メニューの数
						1,	//デフォルト
						STR_EDGE_POP2,
						ID_EDGE_POP
						);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}

//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
	channelShiftPrm		newPos;			
	if (infoP){
		newPos = addChannelShiftPrm(xL<<16,yL<<16,&infoP->cs);
		*outP = getPixel8_CS(&infoP->gp,&newPos);
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
	channelShiftPrm		newPos;			
	if (infoP){
		newPos = addChannelShiftPrm(xL<<16,yL<<16,&infoP->cs);
		*outP = getPixel16_CS(&infoP->gp,&newPos);
	}

	return err;
}
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err 
FilterImage32 (
	refconType		refcon, 
	A_long			xL, 
	A_long			yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);
	channelShiftPrm		newPos;			
	if (infoP){
		newPos = addChannelShiftPrm(xL<<16,yL<<16,&infoP->cs);
		*outP = getPixel32_CS(&infoP->gp,&newPos);
	}

	return err;
}
#endif
//-------------------------------------------------------------------------------------------------
static void PramSet(CFsAE *ae, ParamInfo *infoP)
{

	//---------------------------------------------------------------
	//移動距離をXYに分ける
	CRotCalc rt(ae->in_data);
	rt.SetRotLength(infoP->a_angle, infoP->a_length);
	infoP->cs.a= rt.pos();
	rt.SetRotLength(infoP->r_angle, infoP->r_length);
	infoP->cs.r= rt.pos();
	rt.SetRotLength(infoP->g_angle, infoP->g_length);
	infoP->cs.g= rt.pos();
	rt.SetRotLength(infoP->b_angle, infoP->b_length);
	infoP->cs.b= rt.pos();
	//---------------------------------------------------------------
	infoP->gp.data		= ae->input->data;
	infoP->gp.width		= ae->input->width;
	infoP->gp.height	= ae->input->height;
	infoP->gp.widthTrue	= ae->in->widthTrue();

	switch (infoP->edge_status){
	case edge_fill:
		getPixel8_sub = getPixel8_fill;
		getPixel16_sub = getPixel16_fill;
		#if defined(SUPPORT_SMARTFX)
		getPixel32_sub = getPixel32_fill;
		#endif
		break;
	case edge_rep:
		getPixel8_sub = getPixel8_rep;
		getPixel16_sub = getPixel16_rep;
		#if defined(SUPPORT_SMARTFX)
		getPixel32_sub = getPixel32_rep;
		#endif
		break;
	case edge_mirror:
		getPixel8_sub = getPixel8_mirror;
		getPixel16_sub = getPixel16_mirror;
		#if defined(SUPPORT_SMARTFX)
		getPixel32_sub = getPixel32_mirror;
		#endif
		break;
	case edge_none:
	default:
		getPixel8_sub = getPixel8_none;
		getPixel16_sub = getPixel16_none;
		#if defined(SUPPORT_SMARTFX)
		getPixel32_sub = getPixel32_none;
		#endif
		break;
	}
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
GetParams ( CFsAE *ae, ParamInfo * infoP)
{
	PF_Err			err = PF_Err_NONE;
	
	ERR(ae->GetANGLE(ID_RED_ROT,&infoP->r_angle));
	ERR(ae->GetFIXED(ID_RED_LENGTH,&infoP->r_length));
	if(!err) infoP->r_length = ae->downScaleNoClip(infoP->r_length); 

	ERR(ae->GetANGLE(ID_GREEN_ROT,&infoP->g_angle));
	ERR(ae->GetFIXED(ID_GREEN_LENGTH,&infoP->g_length));
	if(!err) infoP->g_length = ae->downScaleNoClip(infoP->g_length); 

	ERR(ae->GetANGLE(ID_BLUE_ROT,&infoP->b_angle));
	ERR(ae->GetFIXED(ID_BLUE_LENGTH,&infoP->b_length));
	if(!err) infoP->b_length = ae->downScaleNoClip(infoP->b_length); 

	ERR(ae->GetANGLE(ID_ALPHA_ROT,&infoP->a_angle));
	ERR(ae->GetFIXED(ID_ALPHA_LENGTH,&infoP->a_length));
	if(!err) infoP->a_length = ae->downScaleNoClip(infoP->a_length); 



	ERR(ae->GetPOPUP(ID_EDGE_POP,&infoP->edge_status));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	PramSet(ae,infoP);

	if ( (infoP->r_length==0)&&(infoP->g_length==0)&&(infoP->b_length==0)&&(infoP->a_length==0)){
		ERR(ae->CopyInToOut());
	}else{
		ERR(ae->out->clear());
		switch(ae->pixelFormat())
		{
		#if defined(SUPPORT_SMARTFX)
		case PF_PixelFormat_ARGB128:
			ae->iterate32_ORG((refconType)infoP,FilterImage32);
			break;
		#endif
		case PF_PixelFormat_ARGB64:
			ae->iterate16((refconType)infoP,FilterImage16);
			break;
		case PF_PixelFormat_ARGB32:
			ae->iterate8((refconType)infoP,FilterImage8);
			break;
		default:
			err = PF_Err_BAD_CALLBACK_PARAM;
		}
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
	PF_Err			err = PF_Err_NONE;
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);

	err =ae.resultErr();
	if (!err){
		ParamInfo info;
		ERR(GetParams(&ae, &info));
		ERR(Exec(&ae,&info));
	}else{
		err = PF_Err_BAD_CALLBACK_PARAM;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
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