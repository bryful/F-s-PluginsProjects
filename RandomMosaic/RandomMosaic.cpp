//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "RandomMosaic.h"


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

//-----------------------------------------------------------------------------------
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
static PF_Err ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_Y,//パラメータの名前
					0, 				//数値入力する場合の最小値
					2048,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					512,			//スライダーの最大値
					0,					//デフォルトの値
					ID_Y
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SIZEMAX,//パラメータの名前
					3, 				//数値入力する場合の最小値
					1024,			//数値入力する場合の最大値
					12,				//スライダーの最小値 
					64,				//スライダーの最大値
					24,				//デフォルトの値
					ID_SIZEMAX
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SIZEMIN,//パラメータの名前
					3, 				//数値入力する場合の最小値
					1024,			//数値入力する場合の最大値
					6,				//スライダーの最小値 
					64,			//スライダーの最大値
					6,				//デフォルトの値
					ID_SIZEMIN
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_ASPECT,//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,				//デフォルトの値
					ID_ASPECT
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_RANDOMCOLOR,//パラメータの名前
					0, 				//数値入力する場合の最小値
					255,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					16,			//スライダーの最大値
					0,				//デフォルトの値
					ID_RANDCOLOR
					);

	out_data->num_params = ID_NUM_PARAMS;

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
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_Y,&infoP->value));
	
	ERR(ae->GetADD(ID_SIZEMAX,&infoP->sizeMax));
	if (!err) infoP->sizeMax = ae->downScale(infoP->sizeMax);
	
	ERR(ae->GetADD(ID_SIZEMIN,&infoP->sizeMin));
	if (!err) {
		infoP->sizeMin = ae->downScale(infoP->sizeMin);
		if ( infoP->sizeMin>infoP->sizeMax){
			swapLong(&infoP->sizeMin, &infoP->sizeMax);
		}
	}
	ERR(ae->GetADD(ID_ASPECT,&infoP->aspect));

	ERR(ae->GetADD(ID_RANDCOLOR,&infoP->randomColor));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	if ( (infoP->value>0) ) {
		FsFlatParam fp;

		A_long prValue = infoP->randomColor;
		FsPixelRand	pr;
		pr.a = pr.b = pr.g = pr.r = prValue;

		//value回繰り返す
		for(A_long i=0;i<infoP->value;i++ ){
			//サイズ
			fp.sizeX = F_RAND2(infoP->sizeMin,infoP->sizeMax); //サイズを適当に変える
			fp.sizeY = fp.sizeX;
			if (infoP->aspect>0){
				A_long temp = (infoP->sizeMax - infoP->sizeMin) * infoP->aspect / 100;
				fp.sizeX += F_RAND2(-temp,temp);
				fp.sizeY += F_RAND2(-temp,temp);
				if (fp.sizeX<2) fp.sizeX = 2;
				if (fp.sizeY<2) fp.sizeY = 2;
			}else{
				F_RAND();
				F_RAND();
			}
			//位置
			fp.x = F_RAND2(-fp.sizeX,ae->out->width());
			fp.y = F_RAND2(-fp.sizeY,ae->out->height());
			//明るさ
			if ( prValue>0){
				pr.b = pr.g = pr.r =
				pr.a =F_RAND2(-prValue,prValue);
			}else{
				F_RAND();
			}
			ae->out->flat8(&fp,&pr);
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec16(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	if ( (infoP->value>0) ) {
		FsFlatParam fp;

		A_long prValue = FS_CONVERT8TO16(infoP->randomColor);
		FsPixelRand	pr;
		pr.a = pr.b = pr.g = pr.r = prValue;

		//value回繰り返す
		for(A_long i=0;i<infoP->value;i++ ){
			//サイズ
			fp.sizeX = F_RAND2(infoP->sizeMin,infoP->sizeMax); //サイズを適当に変える
			fp.sizeY = fp.sizeX;
			if (infoP->aspect>0){
				A_long temp = (infoP->sizeMax - infoP->sizeMin) * infoP->aspect / 100;
				fp.sizeX += F_RAND2(-temp,temp);
				fp.sizeY += F_RAND2(-temp,temp);
				if (fp.sizeX<2) fp.sizeX = 2;
				if (fp.sizeY<2) fp.sizeY = 2;
			}else{
				F_RAND();
				F_RAND();
			}
			//位置
			fp.x = F_RAND2(-fp.sizeX,ae->out->width());
			fp.y = F_RAND2(-fp.sizeY,ae->out->height());
			//明るさ
			if ( prValue>0){
				pr.b = pr.g = pr.r =
				pr.a = F_RAND2(-prValue,prValue);
			}else{
				F_RAND();
			}
			ae->out->flat16(&fp,&pr);
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec32(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	if ( (infoP->value>0) ) {
		FsFlatParam fp;

		A_long prValue = FS_CONVERT8TO16(infoP->randomColor);
		FsPixelRandFloat pr;

		//value回繰り返す
		for(A_long i=0;i<infoP->value;i++ ){
			//サイズ
			fp.sizeX = F_RAND2(infoP->sizeMin,infoP->sizeMax); //サイズを適当に変える
			fp.sizeY = fp.sizeX;
			if (infoP->aspect>0){
				A_long temp = (infoP->sizeMax - infoP->sizeMin) * infoP->aspect / 100;
				fp.sizeX += F_RAND2(-temp,temp);
				fp.sizeY += F_RAND2(-temp,temp);
				if (fp.sizeX<2) fp.sizeX = 2;
				if (fp.sizeY<2) fp.sizeY = 2;
			}else{
				F_RAND();
				F_RAND();
			}
			//位置
			fp.x = F_RAND2(-fp.sizeX,ae->out->width());
			fp.y = F_RAND2(-fp.sizeY,ae->out->height());
			//明るさ
			if ( prValue>0){
				PF_FpShort rr = (PF_FpShort)F_RAND2(-prValue,prValue)/PF_MAX_CHAN16;
				pr.b = pr.g = pr.r =
				pr.a = rr;
			}else{
				F_RAND();
			}
			ae->out->flat32(&fp,&pr);
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
	ERR(ae->CopyInToOut());
	if (infoP->value>0){
		F_SRAND(ae->frame());
		switch(ae->pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			err = Exec32(ae,infoP);
			break;
		case PF_PixelFormat_ARGB64:
			err =Exec16(ae,infoP);
			break;
		case PF_PixelFormat_ARGB32:
			err =Exec8(ae,infoP);
			break;
		}
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
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
		ERR(ae.CopyInToOut());
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
//-----------------------------------------------------------------------------------
static PF_Err 
RespondtoAEGP ( 	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extraP)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extraP));

	return err;
}

//-----------------------------------------------------------------------------------
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
