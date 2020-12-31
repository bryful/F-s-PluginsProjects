//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "GuideFrame.h"


//-------------------------------------------------------------------------------------------------
// Aboutダイアログ
static PF_Err About (
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
static PF_Err GlobalSetup (
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

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_COLOR, 
					0xff,
					0x00,
					0x00,
					ID_COLOR
					);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POINT(STR_TOPLEFT,			/*"New Center"*/ 
				25,	// X
				25,	// Y
				0,	// Flag
				ID_TOPLEFT
				);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);	
	PF_ADD_POINT(STR_BOTTOMRIGHT,			/*"New Center"*/ 
				75,	// X
				75,	// Y
				0,	// Flag
				ID_BOTTOMRIGHT
				);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_CHK1,
					STR_CHK2,
					FALSE,
					0,
					ID_CHK
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_SMOOTH1,
					STR_SMOOTH2,
					TRUE,
					0,
					ID_SMOOTH
					);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-------------------------------------------------------------------------------------------------
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
static void chkW8(CFsAE *ae)
{
	PF_Pixel *data;
	PF_Pixel c;
	PF_Pixel bl ={64,0,0,255};
	data = (PF_Pixel *)ae->out->data();
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long ow = ae->out->offsetWidth();
	A_long tw	= ae->out->widthTrue();

	A_long x0,x1,y0,y1;

	A_long i,j,t;
	//まず上から探す
	y0 = -1;
	t = 0;
	for (j=0;j<h;j++){
		for(i=0;i<w;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				y0 = j;
				break;
			}
			t++;
		}
		t+=ow;
		if (y0!=-1) break;

	}
	if (y0==-1) return;
	//下から探す
	y1 = -1;
	for (j = h-1;j>=0;j--){
		t = j * tw;
		for(i=0;i<w;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				y1 = j;
				break;
			}
			t++;
		}
		if (y1!=-1) break;
	}

	//まず左から探す
	x0 = -1;
	t = 0;
	for (j=0;j<w;j++){
		t = j;
		for(i=0;i<h;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				x0 = j;
				break;
			}
			t+=tw;
		}
		if (x0!=-1) break;

	}
	//まず右から探す
	x1 = -1;
	t = 0;
	for (j=w-1;j>=0;j--){
		t = j;
		for(i=0;i<h;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				x1 = j;
				break;
			}
			t+=tw;
		}
		if (x1!=-1) break;

	}
	if ( (x0<0)||(y0<0)||(x1<0)||(y1<0) ) return;

	t =0;
	for (j=y0;j<=y1;j++){
		t = j * tw + x0;
		for(i=x0;i<=x1;i++){
			c = data[t];
			if (  data[t].alpha==0) {
				data[t] = bl;
			}
			t++;
		}

	}

}
//-----------------------------------------------------------------------------------
static void chkW16(CFsAE *ae)
{
	PF_Pixel16 *data;
	PF_Pixel16 c;
	PF_Pixel16 bl ={FS_CONVERT8TO16(64),0,0,FS_CONVERT8TO16(255)};
	data = (PF_Pixel16 *)ae->out->data();
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long ow = ae->out->offsetWidth();
	A_long tw	= ae->out->widthTrue();

	A_long x0,x1,y0,y1;

	A_long i,j,t;
	//まず上から探す
	y0 = -1;
	t = 0;
	for (j=0;j<h;j++){
		for(i=0;i<w;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				y0 = j;
				break;
			}
			t++;
		}
		t+=ow;
		if (y0!=-1) break;

	}
	if (y0==-1) return;
	//下から探す
	y1 = -1;
	for (j = h-1;j>=0;j--){
		t = j * tw;
		for(i=0;i<w;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				y1 = j;
				break;
			}
			t++;
		}
		if (y1!=-1) break;
	}

	//まず左から探す
	x0 = -1;
	t = 0;
	for (j=0;j<w;j++){
		t = j;
		for(i=0;i<h;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				x0 = j;
				break;
			}
			t+=tw;
		}
		if (x0!=-1) break;

	}
	//まず右から探す
	x1 = -1;
	t = 0;
	for (j=w-1;j>=0;j--){
		t = j;
		for(i=0;i<h;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				x1 = j;
				break;
			}
			t+=tw;
		}
		if (x1!=-1) break;

	}
	if ( (x0<0)||(y0<0)||(x1<0)||(y1<0) ) return;

	t =0;
	for (j=y0;j<=y1;j++){
		t = j * tw + x0;
		for(i=x0;i<=x1;i++){
			c = data[t];
			if (  data[t].alpha==0) {
				data[t] = bl;
			}
			t++;
		}

	}

}
//-----------------------------------------------------------------------------------
static void chkW32(CFsAE *ae)
{
	PF_PixelFloat *data;
	PF_PixelFloat c;
	PF_PixelFloat bl ={ (PF_FpShort)64/(PF_FpShort)255 ,0,0,1};
	data = (PF_PixelFloat *)ae->out->data();
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long ow = ae->out->offsetWidth();
	A_long tw	= ae->out->widthTrue();

	A_long x0,x1,y0,y1;

	A_long i,j,t;
	//まず上から探す
	y0 = -1;
	t = 0;
	for (j=0;j<h;j++){
		for(i=0;i<w;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				y0 = j;
				break;
			}
			t++;
		}
		t+=ow;
		if (y0!=-1) break;

	}
	if (y0==-1) return;
	//下から探す
	y1 = -1;
	for (j = h-1;j>=0;j--){
		t = j * tw;
		for(i=0;i<w;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				y1 = j;
				break;
			}
			t++;
		}
		if (y1!=-1) break;
	}

	//まず左から探す
	x0 = -1;
	t = 0;
	for (j=0;j<w;j++){
		t = j;
		for(i=0;i<h;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				x0 = j;
				break;
			}
			t+=tw;
		}
		if (x0!=-1) break;

	}
	//まず右から探す
	x1 = -1;
	t = 0;
	for (j=w-1;j>=0;j--){
		t = j;
		for(i=0;i<h;i++){
			c = data[t];
			if (  data[t].alpha!=0) {
				x1 = j;
				break;
			}
			t+=tw;
		}
		if (x1!=-1) break;

	}
	if ( (x0<0)||(y0<0)||(x1<0)||(y1<0) ) return;

	t =0;
	for (j=y0;j<=y1;j++){
		t = j * tw + x0;
		for(i=x0;i<=x1;i++){
			c = data[t];
			if (  data[t].alpha==0) {
				data[t] = bl;
			}
			t++;
		}

	}

}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCOLOR(ID_COLOR,&infoP->color));
	ERR(ae->GetFIXEDPOINT(ID_TOPLEFT,&infoP->top_left));
	ERR(ae->GetFIXEDPOINT(ID_BOTTOMRIGHT,&infoP->bottom_right));
	ERR(ae->GetCHECKBOX(ID_CHK,&infoP->check));
	ERR(ae->GetCHECKBOX(ID_SMOOTH,&infoP->smooth));
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	A_long x0 = infoP->top_left.x >> 16;
	A_long y0 = infoP->top_left.y >> 16;
	A_long x1 = (infoP->bottom_right.x >> 16)-1;
	A_long y1 = (infoP->bottom_right.y >> 16)-1;


	ae->out->lineHeiht = 2;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		if ( infoP->check) chkW32(ae);
		ae->out->box32(x0,y0,x1,y1,CONV8TO32(infoP->color));
		break;
	case PF_PixelFormat_ARGB64:
		if ( infoP->check) chkW16(ae);
		ae->out->box16(x0,y0,x1,y1,CONV8TO16(infoP->color));
		break;
	case PF_PixelFormat_ARGB32:
		if ( infoP->check) chkW8(ae);
		ae->out->box8(x0,y0,x1,y1,infoP->color);
		break;
	}
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
