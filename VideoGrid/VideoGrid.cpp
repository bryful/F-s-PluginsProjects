//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "VideoGrid.h"


//-------------------------------------------------------------------------------------------------
//About表示
static PF_Err About (	PF_InData		*in_data,
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
static PF_Err GlobalSetup (	PF_InData		*in_data,
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
static PF_Err ParamsSetup (	PF_InData		*in_data,
							PF_OutData		*out_data,
							PF_ParamDef		*params[],
							PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_GRIDSIZE,	//パラメータの名前
					2, 			//数値入力する場合の最小値
					128,		//数値入力する場合の最大値
					2,			//スライダーの最小値 
					32,			//スライダーの最大値
					4,	//デフォルトの値
					ID_GRIDSIZE
					);

	//2個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MODE1,STR_MODE2, FALSE,0, ID_MODE);

	//3個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_HILIGHT,	//パラメータの名前
				0, 				//数値入力する場合の最小値
				200,			//数値入力する場合の最大値
				0,			//スライダーの最小値 
				200,			//スライダーの最大値
				120,			//デフォルトの値
				1,				//数値表示に関するフラグ 
				0,
				0,
				ID_HILIGHT
				);
	//4個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_SHADOW,	//パラメータの名前
				0, 				//数値入力する場合の最小値
				200,	//数値入力する場合の最大値
				0,				//スライダーの最小値 
				200,		//スライダーの最大値
				75,		//デフォルトの値
				1,//数値表示に関するフラグ 
				0,
				0,
				ID_SHADOW
				);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_HEIGHT,	//パラメータの名前
					1, 			//数値入力する場合の最小値
					16,		//数値入力する場合の最大値
					1,			//スライダーの最小値 
					4,			//スライダーの最大値
					1,	//デフォルトの値
					ID_HEIGHT
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
	/*
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
	*/
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
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	/*
	CFsAE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_NOISE_FRAME,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	*/
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err
GetParams(CFsAE *ae,  ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	ERR(ae->GetADD(ID_GRIDSIZE,&infoP->gridSize));
	if (!err){
		infoP->gridSize = ae->downScale(infoP->gridSize);
		if (infoP->gridSize<2) infoP->gridSize =2;
	}
	ERR(ae->GetCHECKBOX(ID_MODE,&infoP->isMosaic));
	PF_Fixed f;
	ERR(ae->GetFIXED(ID_HILIGHT,&f));
	if (!err) infoP->hilight = (PF_FpShort)f /(6553600);
	ERR(ae->GetFIXED(ID_SHADOW,&f));
	if (!err) infoP->shadow = (PF_FpShort)f /(6553600);
	ERR(ae->GetADD(ID_HEIGHT,&infoP->height));
	if (!err){
		if (infoP->height > (infoP->gridSize/2))  infoP->height = infoP->gridSize/2;
		if (infoP->height<1) infoP->height = 1;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
GetGridParams(CFsAE *ae,  ParamInfo *infoP,gridParam *gp)
{
	PF_Err	err = PF_Err_NONE;
	gp->info.gridSize	= infoP->gridSize;
	gp->info.height		= infoP->height;
	gp->info.hilight	= infoP->hilight;
	gp->info.isMosaic	= infoP->isMosaic;
	gp->info.shadow		= infoP->shadow;
	gp->data	= ae->output->data;
	gp->width	= ae->output->width;
	gp->height	= ae->output->height;
	switch(ae->pixelFormat())
	{
		case PF_PixelFormat_ARGB128:
			gp->widthTrue	= ae->output->rowbytes/sizeof(PF_PixelFloat);
			break;
		case PF_PixelFormat_ARGB64:
			gp->widthTrue	= ae->output->rowbytes/sizeof(PF_Pixel16);
			break;
		case PF_PixelFormat_ARGB32:
			gp->widthTrue	= ae->output->rowbytes/sizeof(PF_Pixel);
			break;
	}

	//画像のサイズからループ回数を
	gp->loopX = gp->width / infoP->gridSize;
	if ( (gp->width % infoP->gridSize)!=0 ) gp->loopX += 1;
	gp->loopY = gp->height / infoP->gridSize;
	if ( (gp->height % infoP->gridSize)!=0 ) gp->loopY += 1;

	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err
GetParams(CFsAE *ae,  ParamInfo *infoP,gridParam *gp)
{
	PF_Err	err = PF_Err_NONE;
	err = GetParams(ae,infoP);
	if (!err) err = GetGridParams(ae,infoP,gp);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
Exec(CFsAE *ae, gridParam *gp)
{
	PF_Err	err = PF_Err_NONE;
	if ( (gp->info.isMosaic == FALSE)&&(gp->info.hilight==1)&&(gp->info.shadow==1))return err;		
	A_long	i,j;
	switch (ae->pixelFormat()) {
		case PF_PixelFormat_ARGB128:
			for (j=0;j<gp->loopY;j++){
				gp->y = j * gp->info.gridSize;
				for (i=0;i<gp->loopX;i++){
					gp->x = i * gp->info.gridSize;
					if (gp->info.isMosaic==TRUE) { BlockMosaic32(gp);}
					if (!( (gp->info.hilight ==1)&&(gp->info.shadow ==1))) BlockBevel32(gp);
				}
			}
			break;
		case PF_PixelFormat_ARGB64:
			for (j=0;j<gp->loopY;j++){
				gp->y = j * gp->info.gridSize;
				for (i=0;i<gp->loopX;i++){
					gp->x = i * gp->info.gridSize;
					if (gp->info.isMosaic==TRUE) { BlockMosaic16(gp);}
					if (!( (gp->info.hilight ==1)&&(gp->info.shadow ==1))) BlockBevel16(gp);
				}
			}
			break;
		case PF_PixelFormat_ARGB32:
			for (j=0;j<gp->loopY;j++){
				gp->y = j * gp->info.gridSize;
				for (i=0;i<gp->loopX;i++){
					gp->x = i * gp->info.gridSize;
					if (gp->info.isMosaic==TRUE) { BlockMosaic8(gp);}
					if (!( (gp->info.hilight ==1)&&(gp->info.shadow ==1))) BlockBevel8(gp);
				}
			}
			break;
		default:
			err = PF_Err_BAD_CALLBACK_PARAM;
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
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ERR(ae.CopyInToOut());
		ParamInfo info;
		gridParam gp;
		ERR(GetParams(&ae,&info,&gp));
		ERR(Exec(&ae,&gp));
	}
	return err;
}
#if defined(SUPPORT_SMARTFX)
//-----------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------
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
			ERR(ae.CopyInToOut());
			gridParam gp;
			ERR(GetGridParams(&ae,infoP,&gp));
			ERR(Exec(&ae,&gp));
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
