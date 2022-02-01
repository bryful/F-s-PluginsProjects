//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "PaperPlaneGetWeightInfo.h"

#include <stdio.h>
#include "AEDebugAngel.h"

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
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_CHECKBOX(STR_DRAW_GRAPH,
					"on",
					TRUE,
					0,
					ID_DRAW_GRAPH
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(STR_GLUE,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		25,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		15,			//スライダーの最大値
		7,				//デフォルトの値
		ID_GLUE
	);	//----------------------------------------------------------------
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
	PF_Err				err					= PF_Err_NONE;
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
	PF_Err 	err 	= PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
chkWeightTable  (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel *data = (PF_Pixel *)ae->in->data();




	A_long w = ae->in->width();
	A_long h = ae->in->height();
	A_long wt = ae->in->widthTrue();

	A_long target = 0;

	for (A_long x = 0; x < w; x++){
		target = x;
		double ww = 0;
		for (A_long y = 0; y < h; y++) {
			if (data[target].alpha == PF_MAX_CHAN8) {

				A_long c = (A_long)(data[target].blue & 0xF);
				A_long c2 = (A_long)((data[target].blue>>4) & 0xF);

				double v = (double)data[target].green + (double)data[target].red;

				if (c == c2) {
					if ((infoP->glue > 0) && (c > 1)) {
						v += infoP->glue *(c - 1);
					}
				}
				else {
					if ((infoP->glue > 0) && (c > 1)) {
						v += infoP->glue *(c - 1)*2;
					}
				}

				v /= 36;
				v = v * 180/ ((788 * 300 / 25.4)*(1091 * 300 / 25.4));
				ww += v;
			}
			target += wt;
		}
		infoP->weightTable[x] = ww;

	}

	double mm = 0;
	double all = 0;
	infoP->weight = 0;
	for (A_long x = 0; x < w; x++){
		mm += (double)x * infoP->weightTable[x];
		all += infoP->weightTable[x];
	}
	infoP->center = (A_long)(mm / all + 0.5);
	infoP->weight = all;

	return err;
}
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
static PF_Err 
drawBalance  (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	PF_InData		*in_data;
	in_data = ae->in_data;

	PF_Pixel *data = (PF_Pixel *)ae->out->data();
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long wt = ae->out->widthTrue();


	A_long ww = (A_long)(infoP->weight * 100 + 0.5);
	double ww2 = (double)ww / 100;
	char str[256] = { '\0' };
	PF_SPRINTF(str, "weight:%4.2fg", ww2);

	PF_Pixel p = { 255, 255, 255, 255 };
	DispAngel(ae->out, str, 30, 300, p);


	if (infoP->drawGraph) {
		PF_Pixel red = { 128, 255, 0, 0 };
		for (A_long x = 0; x < w; x++) {
			A_long g = (A_long)(infoP->weightTable[x]*100000 + 0.5);
			for (A_long y = 0; y < g; y++){
				PF_Pixel8 c = ae->out->getPixel8(x, y);
				ae->out->setPixel8(x, y, PixelBlend8(c, red));
			}
		}
	}
	PF_Pixel blue = { 255, 255, 0, 255 };
	ae->out->YLine8(infoP->center - 2, 0, h, blue);
	ae->out->YLine8(infoP->center-1, 0, h, blue);
	ae->out->YLine8(infoP->center, 0, h, blue);
	ae->out->YLine8(infoP->center+1, 0, h, blue);
	ae->out->YLine8(infoP->center + 2, 0, h, blue);



	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	infoP->center = 50;
	infoP->weight = 0;

	ERR(ae->GetCHECKBOX(ID_DRAW_GRAPH, &infoP->drawGraph));
	ERR(ae->GetADD(ID_GLUE, &infoP->glue));

		


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	infoP->width = ae->out->width();
	infoP->height = ae->out->height();
	infoP->widthTrue = ae->out->widthTrue();

	ae->CopyInToOut();

	infoP->tableH = ae->NewHandle(infoP->width *sizeof(double) * 2);
	if (!infoP->tableH){
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		return err;
	}
	infoP->weightTable = *(double **)infoP->tableH;


	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		break;
	case PF_PixelFormat_ARGB64:
		break;
	case PF_PixelFormat_ARGB32:

		chkWeightTable(ae, infoP);
		drawBalance(ae, infoP);
		break;
	}


	if (infoP->tableH != NULL){
		ae->DisposeHandle(infoP->tableH);
		infoP->tableH = NULL;
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
