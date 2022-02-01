//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "RandomMosaic2nd.h"

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
	PF_ADD_FIXED(	STR_Y,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					0,			//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_Y
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_STRENGTH,//パラメータの名前
					0, 				//数値入力する場合の最小値
					255,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					128,			//スライダーの最大値
					10,					//デフォルトの値
					ID_STRENGTH
					);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SIZEX,//パラメータの名前
					1, 				//数値入力する場合の最小値
					256,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					256,			//スライダーの最大値
					8,					//デフォルトの値
					ID_SIZEX
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_SIZEY,//パラメータの名前
					1, 				//数値入力する場合の最小値
					256,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					256,			//スライダーの最大値
					8,					//デフォルトの値
					ID_SIZEY
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

	ERR(ae->GetFIXED(ID_Y,&infoP->value));
	if (!err)  infoP->value /= 100;
	
	ERR(ae->GetADD(ID_STRENGTH,&infoP->strangth));
	
	ERR(ae->GetADD(ID_SIZEX,&infoP->sizeX));
	if (!err) infoP->sizeX = ae->downScale(infoP->sizeX);
	ERR(ae->GetADD(ID_SIZEY,&infoP->sizeY));
	if (!err) infoP->sizeY = ae->downScale(infoP->sizeY);

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	if ( (infoP->value>0) ) {
		FsFlatParam fp;

		A_long prValue = infoP->strangth;
		FsPixelRand	pr;
		fp.sizeX = infoP->sizeX;
		fp.sizeY = infoP->sizeY;
		A_long cntX,cntY;
		cntX = ae->out->width() / infoP->sizeX;
		if ( (ae->out->width() % infoP->sizeX)!=0) cntX++;
		cntY = ae->out->height() / infoP->sizeY;
		if ( (ae->out->height() % infoP->sizeY)!=0) cntY++;
		A_long x,y;
		for( y=0; y<cntY;y++){
			fp.y = y * infoP->sizeY;
			for( x=0; x<cntX;x++){
				fp.x = x * infoP->sizeX;
				if ( F_RAND3(1L<<16)<= infoP->value){
					pr.a = pr.b = pr.g = pr.r = F_RAND2(-prValue, prValue);
					ERR(ae->out->flat8(&fp,&pr));
				}else{
					F_RAND();
				}
			}
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

		A_long prValue = FS_CONVERT8TO16(infoP->strangth);
		FsPixelRand	pr;
		fp.sizeX = infoP->sizeX;
		fp.sizeY = infoP->sizeY;
		A_long cntX,cntY;
		cntX = ae->out->width() / infoP->sizeX;
		if ( (ae->out->width() % infoP->sizeX)!=0) cntX++;
		cntY = ae->out->height() / infoP->sizeY;
		if ( (ae->out->height() % infoP->sizeY)!=0) cntY++;
		A_long x,y;
		for( y=0; y<cntY;y++){
			fp.y = y * infoP->sizeY;
			for( x=0; x<cntX;x++){
				fp.x = x * infoP->sizeX;
				if ( F_RAND3(1L<<16)<= infoP->value){
					pr.a = pr.b = pr.g = pr.r = F_RAND2(-prValue, prValue);
					ERR(ae->out->flat16(&fp,&pr));
				}else{
					F_RAND();
				}
			}
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

		A_long prValue = FS_CONVERT8TO16(infoP->strangth);
		FsPixelRandFloat	pr;
		fp.sizeX = infoP->sizeX;
		fp.sizeY = infoP->sizeY;
		A_long cntX,cntY;
		cntX = ae->out->width() / infoP->sizeX;
		if ( (ae->out->width() % infoP->sizeX)!=0) cntX++;
		cntY = ae->out->height() / infoP->sizeY;
		if ( (ae->out->height() % infoP->sizeY)!=0) cntY++;
		A_long x,y;
		for( y=0; y<cntY;y++){
			fp.y = y * infoP->sizeY;
			for( x=0; x<cntX;x++){
				fp.x = x * infoP->sizeX;
				if ( F_RAND3(1L<<16)<= infoP->value){
					pr.a = pr.b = pr.g = pr.r = (PF_FpShort)F_RAND2(-prValue, prValue) / PF_MAX_CHAN16;
					ERR(ae->out->flat32(&fp,&pr));
				}else{
					F_RAND();
				}
			}
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

/*
//-------------------------------------------------------------------------------------------------
PF_Err GetParams (	
						PF_InData			*in_data,
						PF_OutData		*out_data,
						PF_ParamDef		*params[],
						PF_LayerDef		*output,
						FsAEParams		*aeprm,
						MY_Params			*prm,
						mosaicPrm			*mprm
						)
{
	PF_Err		err 		= PF_Err_NONE;
	PF_EffectWorld 	*input	=	&params[MY_INPUT]->u.ld;	//入力画像のアドレス
	if ( (params==NULL)||(output==NULL)||(input==NULL) ) {
		return 0xFF;
	}

	//-----------------------
	aeprm->is16Bit	=  PF_WORLD_IS_DEEP(output);
	aeprm->inData		= (PF_Pixel *)input->data;
	aeprm->outData	= (PF_Pixel *)output->data;
	aeprm->inData16	= (PF_Pixel16 *)input->data;
	aeprm->outData16= (PF_Pixel16 *)output->data;
	aeprm->width	= input->width;
	aeprm->height	= input->height;
	if (aeprm->is16Bit) {
		aeprm->inWidth	= input->rowbytes/sizeof(PF_Pixel16);
		aeprm->outWidth	= output->rowbytes/sizeof(PF_Pixel16);
	}else{
		aeprm->inWidth	= input->rowbytes/sizeof(PF_Pixel);
		aeprm->outWidth	= output->rowbytes/sizeof(PF_Pixel);
	}
	aeprm->offsetInWidth  = aeprm->inWidth -input->width;
	aeprm->offsetOutWidth = aeprm->outWidth -input->width;
	if ( (aeprm->offsetInWidth <0)||( aeprm->offsetOutWidth <0) ) {
		err = 0xFF;
	}
	if ( input->height != output->height) {
		err = 0xFF;
	}
	//カレントフレームを求める画頭は０
	if ( (in_data->current_time>=0)&&(in_data->time_step>0) ) {
		aeprm->Frame	=(in_data->current_time/in_data->time_step); 
	}else{
		aeprm->Frame = 0;
	}
	mprm->height		= aeprm->height;
	mprm->inData		= aeprm->inData;
	mprm->inData16	= aeprm->inData16;
	mprm->outData		= aeprm->outData;
	mprm->outData16	= aeprm->outData16;
	mprm->inOffset	= aeprm->offsetInWidth;
	mprm->inWidth		= aeprm->inWidth;
	mprm->is16Bit		= aeprm->is16Bit;
	mprm->mode			= mosaicTopLeft;
	mprm->outOffset	= aeprm->offsetOutWidth;
	mprm->outWidth	= aeprm->outWidth;
	mprm->width			= aeprm->width;

	//-----------------------
	//モザイクの量
	prm->value	= params[MY_VALUE]->u.sd.value /100;
	prm->strangth = params[MY_STRENGTH]->u.sd.value;
	if (aeprm->is16Bit) {
		prm->strangth =  CONVERT8TO16(prm->strangth);	
	}
	mprm->randValue = prm->strangth;
	//モザイクの大きさ
	prm->sizeX	=params[MY_SIZEX]->u.sd.value * in_data->downsample_x.num / in_data->downsample_x.den;
	if (prm->sizeX<1) {prm->sizeX=1;}
	prm->sizeY	=params[MY_SIZEY]->u.sd.value * in_data->downsample_x.num / in_data->downsample_x.den;
	if (prm->sizeY<1) {prm->sizeY=1;}
	mprm->mosaicX = prm->sizeX;
	mprm->mosaicY = prm->sizeY;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Render (	PF_InData		*in_data,
						PF_OutData		*out_data,
						PF_ParamDef		*params[],
						PF_LayerDef		*output )
{
	long	i,j,w,h;
	FsAEParams	aprm;
	MY_Params		prm;
	mosaicPrm		mprm;
	
	PF_Err	err=PF_Err_NONE;
	PF_EffectWorld *input=&params[MY_INPUT]->u.ld;

	if  ( GetParams(in_data,out_data,params,output,&aprm,&prm,&mprm) != PF_Err_NONE ) {
		PF_SPRINTF(	out_data->return_msg, 
					"[%s] 画像サイズエラーです。御免なさい。",
					FS_NAME
					);
		return err;
	}
	
	//取りあえず複写
	PF_COPY(input, output, NULL, NULL);

	if ( prm.value<=0 ){
		return err;
	}
		
	srand(aprm.Frame);

	w = divCeil(aprm.width,  prm.sizeX);
	h = divCeil(aprm.height, prm.sizeY);

	
	if (prm.value>=(100L << 16)) {
		for (j=h-1 ; j>=0 ; j--){
			mprm.y=j*mprm.mosaicY;
			for (i=w-1 ; i>=0 ; i--){
				mprm.x=i*mprm.mosaicX;
				BlockMosaic(&mprm);
			}
		}
	}else{
		for (j=h-1 ; j>=0 ; j--){
			mprm.y=j*mprm.mosaicY;
			for (i=w-1 ; i>=0 ; i--){
				if ((1L<<16)*F_RAND()/F_RAND_MAX<= prm.value) {
					mprm.x=i*mprm.mosaicX;
					BlockMosaic(&mprm);
				}
			}
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
inline long divCeil(long v0, long v1)
{
	long temp = v0 / v1;
	if ( (v0%v1)!=0 ) temp+=1;
	return temp;

}
//-------------------------------------------------------------------------------------------------
DllExport PF_Err EntryPointFunc(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	switch (cmd) {
		case PF_Cmd_ABOUT:
			err = About(in_data,out_data,params,output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = GlobalSetup(in_data,out_data,params,output);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_data,out_data,params,output);
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
	}
	return err;
}//-------------------------------------------------------------------------------------------------
*/
