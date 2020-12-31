//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "ScreenShakeMM.h"


/*
PF_Pixel (*getPixel8_sub)(getPixelPrm *prm,A_long x,A_long y);
PF_Pixel16 (*getPixel16_sub)(getPixelPrm *prm,A_long x,A_long y);
#if defined(SUPPORT_SMARTFX)
PF_PixelFloat (*getPixel32_sub)(getPixelPrm *prm,A_long x,A_long y);
#endif
*/
//-----------------------------------------------------------------------------------
static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.About(in_data,out_data,params,output);
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
GlobalSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetup(in_data,out_data,params,output);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetdown(
	PF_InData	*in_data)
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetdown(in_data);
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetdown (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


//-------------------------------------------------------------------------------------------------
static PF_Err SequenceResetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}



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

	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_MM,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					400,	//数値入力する場合の最大値
					0,		//スライダーの最小値 
					50,		//スライダーの最大値
					2,		//デフォルトの値
					1,		//数値表示に関するフラグ 
					0,
					0,
					ID_MM
					);

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(	STR_VALUE_RND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					10,				//デフォルトの値
					ID_VALUE_RND
					);
	
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_DIR,15.0,ID_DIR);

	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_DIR_RND,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					360,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					45,				//スライダーの最大値
					10,				//デフォルトの値
					ID_DIR_RND
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_RANDOMSEED,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					99,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					30,			//スライダーの最大値
					0,				//デフォルトの値
					ID_RANDOMSEED
					);

	AEFX_CLR_STRUCT(def);	
	PF_ADD_POPUP(		STR_EDGE_STATUS1, 
						4,	//メニューの数
						1,	//デフォルト
						STR_EDGE_STATUS2,
						ID_EDGE_STATUS
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_DPI,	//パラメータの名前
					10, 		//数値入力する場合の最小値
					400,		//数値入力する場合の最大値
					10,			//スライダーの最小値 
					192,		//スライダーの最大値
					144,		//デフォルトの値
					1,			//数値表示に関するフラグ 
					0,
					0,
					ID_DPI
					);
	//----------------------------------------------------------------
#ifdef TEST_MODE
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TEST_TIME_CB1,
					STR_TEST_TIME_CB2,
					FALSE,
					0,
					ID_TEST_TIME_CB
					);
#endif

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
	PF_Fixed			new_x, new_y;
					
	if (infoP){
		new_x = ((A_long)xL << 16) + infoP->shiftX;
		new_y = ((A_long)yL << 16) + infoP->shiftY;
		*outP = getPixel8(&infoP->gp,new_x,new_y);
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
	PF_Fixed			new_x, new_y;
					
	if (infoP){
		new_x = ((A_long)xL << 16) + infoP->shiftX;
		new_y = ((A_long)yL << 16) + infoP->shiftY;
		*outP = getPixel16(&infoP->gp,new_x,new_y);
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
	PF_Fixed			new_x, new_y;
					
	if (infoP){
		new_x = ((A_long)xL << 16) + infoP->shiftX;
		new_y = ((A_long)yL << 16) + infoP->shiftY;
		*outP = getPixel32(&infoP->gp,new_x,new_y);
	}

	return err;
}
#endif
//-------------------------------------------------------------------------------------------------
static void PramSet(CFsAE *ae, ParamInfo *infoP)
{

	//---------------------------------------------------------------
	//移動距離
	PF_Fixed valueMin	= infoP->value * (100 - infoP->value_rand) / 100;
	
	PF_Fixed valueFin	= F_RAND2( valueMin,infoP->value);
	//---------------------------------------------------------------
	//移動方向
	PF_Fixed dirFin = infoP->dir + F_RAND2((-1*infoP->dir_rand),infoP->dir_rand);
		if ( (ae->frame() % 2)==0 ) {
		dirFin += (180L<<16);
	}
	dirFin = RoundAngle360(dirFin);
	//---------------------------------------------------------------
	//移動距離をXYに分ける
	CRotCalc rt(ae->in_data);
	rt.SetRotLength(dirFin, valueFin);
	infoP->shiftX = rt.x();
	infoP->shiftY = rt.y();
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

	PF_Fixed v,dpi;

	ERR(ae->GetFIXED(ID_MM,&v));
	ERR(ae->GetFIXED(ID_DPI,&dpi));
	infoP->value = mm2px(v,dpi);
	if (!err) infoP->value = ae->downScaleNoClip(infoP->value);
	
	ERR(ae->GetADD(ID_VALUE_RND,&infoP->value_rand));
	
	ERR(ae->GetFIXED(ID_DIR,&infoP->dir));
	if (!err) {
		infoP->dir = RoundAngle360(infoP->dir);
	}
	ERR(ae->GetFIXED(ID_DIR_RND,&infoP->dir_rand));
	if (!err) infoP->dir_rand = infoP->dir_rand <<16;//PF_Fixedにあわせる
	ERR(ae->GetADD(ID_RANDOMSEED,&infoP->randomseed));
	ERR(ae->GetADD(ID_EDGE_STATUS,&infoP->edge_status));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	if ( (infoP->shiftX==0)&&(infoP->shiftY==0)){
		ERR(ae->CopyInToOut());
	}else{
		F_SRAND(ae->frame()+ infoP->randomseed);
		switch(ae->pixelFormat())
		{
		#if defined(SUPPORT_SMARTFX)
		case PF_PixelFormat_ARGB128:
			ae->iterate32((refconType)infoP,FilterImage32);
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
		PramSet(&ae,&info);
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
			PramSet(&ae,infoP);
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
//-------------------------------------------------------------------------------------------------
static PF_Err 
RespondtoAEGP ( 	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extra)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extra));

	return err;
}
//-------------------------------------------------------------------------------------------------
DllExport	PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP)
{
	PF_Err		err = PF_Err_NONE;
	
	try
	{
		CFsAE ae;
		switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_data);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceResetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
					err = Render(in_data,out_data,params,output);
					break;

#if defined(SUPPORT_SMARTFX)
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_data,out_data,reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(	in_data,out_data,reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
#endif
			case PF_Cmd_COMPLETELY_GENERAL:
				err = RespondtoAEGP(in_data,out_data,params,output,extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;		
			case PF_Cmd_USER_CHANGED_PARAM:
				err = HandleChangedParam(	in_data,
											out_data,
											params,
											output, 
											reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_data,
										out_data,
										params,
										reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
