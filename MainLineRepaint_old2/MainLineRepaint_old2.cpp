//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "MainLineRepaint_old2.h"


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
	PF_ADD_COLOR(	STR_Main_Color, 
					0x00,
					0x00,
					0x00,
					ID_Main_Color
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_level,	//Name
						0,						//VALID_MIN
						100,						//VALID_MAX
						0,						//SLIDER_MIN
						20,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_level
						);
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
static PF_Err Exec8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err 	err 	= PF_Err_NONE;
	PF_Pixel *scanline;
	scanline = (PF_Pixel *)infoP->scanline;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();

	PF_Pixel *data;
	data = (PF_Pixel *)ae->out->data();
	//水平方向
	A_long cnt = 0;
	for ( A_long j=0; j<h; j++){
		A_long adrY = j*wt;
		for (A_long i=0; i<w; i++) scanline[i] = data[adrY + i];
		for (A_long i=1; i<w-1; i++){
			if ( compPix8Lv(infoP->Main_Color,scanline[i],infoP->lv)==TRUE){
				PF_Pixel d = scanline[i];
				A_long dv = PF_MAX_CHAN16;

				if (compPix8Lv(infoP->Main_Color,scanline[i-1],infoP->lv)==FALSE){
					d = scanline[i-1];
					dv = pxV8(d);
				}

				if (compPix8Lv(infoP->Main_Color,scanline[i+1],infoP->lv)==FALSE){
					if ( dv > pxV8(scanline[i+1])){
						d = scanline[i+1];
					}
				}
				if (compPix8Lv(infoP->Main_Color,d,infoP->lv)==FALSE){
					data[adrY + i] = d;
				}else{
					cnt++;
				}
			}
		}
		if (compPix8Lv(infoP->Main_Color,data[adrY],infoP->lv)==TRUE){
			if (compPix8Lv(infoP->Main_Color,data[adrY+1],infoP->lv)==FALSE){
				data[adrY] = data[adrY+1];
			}else{
				cnt++;
			}
		}
		if (compPix8Lv(infoP->Main_Color,data[adrY+ w-1],infoP->lv)==TRUE){
			if (compPix8Lv(infoP->Main_Color,data[adrY+w -2],infoP->lv)==FALSE){
				data[adrY+ w-1] = data[adrY+ w-2];
			}else{
				cnt++;
			}
		}
	}
	if ( cnt == 0) return err;
	cnt = 0;
	for ( A_long i=0; i<w; i++){
		for (A_long j=0; j<h; j++) scanline[j] = data[j * wt + i];
		for (A_long j=1; j<h-1; j++){
			if ( compPix8Lv(infoP->Main_Color,scanline[j],infoP->lv)==TRUE){
				PF_Pixel d = scanline[j];
				A_long dv = PF_MAX_CHAN16;

				if (compPix8Lv(infoP->Main_Color,scanline[j-1],infoP->lv)==FALSE){
					d = scanline[j-1];
					dv = pxV8(d);
				}

				if (compPix8Lv(infoP->Main_Color,scanline[j+1],infoP->lv)==FALSE){
					if ( dv > pxV8(scanline[j+1])){
						d = scanline[j+1];
					}
				}
				if (compPix8Lv(infoP->Main_Color,d,infoP->lv)==FALSE){
					data[j* wt + i] = d;
				}else{
					cnt++;
				}
			}
		}
		A_long ay = wt * (h-1);
		if (compPix8Lv(infoP->Main_Color,data[i],infoP->lv)==TRUE){
			if (compPix8Lv(infoP->Main_Color,data[i+ wt],infoP->lv)==FALSE){
				data[i] = data[i + wt];
			}else{
				cnt++;
			}
		}
		if (compPix8Lv(infoP->Main_Color,data[i + ay],infoP->lv)==TRUE){
			if (compPix8Lv(infoP->Main_Color,data[i + ay - wt],infoP->lv)==FALSE){
				data[i + ay] = data[i + ay - wt];
			}else{
				cnt++;
			}
		}
	}
	infoP->count = cnt;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec16(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err 	err 	= PF_Err_NONE;
	PF_Pixel16 *scanline;
	scanline = (PF_Pixel16 *)infoP->scanline;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();

	PF_Pixel16 *data;
	data = (PF_Pixel16 *)ae->out->data();
	//水平方向
	A_long cnt = 0;
	for ( A_long j=0; j<h; j++){
		A_long adrY = j*wt;
		for (A_long i=0; i<w; i++) scanline[i] = data[adrY + i];
		for (A_long i=1; i<w-1; i++){
			if ( compPix16_8Lv(scanline[i],infoP->Main_Color,infoP->lv)==TRUE){
				PF_Pixel16 d = scanline[i];
				A_long dv = PF_MAX_CHAN16;

				if (compPix16_8Lv(scanline[i-1],infoP->Main_Color,infoP->lv)==FALSE){
					d = scanline[i-1];
					dv = pxV16(d);
				}

				if (compPix16_8Lv(scanline[i+1],infoP->Main_Color,infoP->lv)==FALSE){
					if ( dv > pxV16(scanline[i+1])){
						d = scanline[i+1];
					}
				}
				if (compPix16_8Lv(d,infoP->Main_Color,infoP->lv)==FALSE){
					data[adrY + i] = d;
				}else{
					cnt++;
				}
			}
		}
		if (compPix16_8Lv(data[adrY],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix16_8Lv(data[adrY+1],infoP->Main_Color,infoP->lv)==FALSE){
				data[adrY] = data[adrY+1];
			}else{
				cnt++;
			}
		}
		if (compPix16_8Lv(data[adrY+ w-1],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix16_8Lv(data[adrY+w -2],infoP->Main_Color,infoP->lv)==FALSE){
				data[adrY+ w-1] = data[adrY+ w-2];
			}else{
				cnt++;
			}
		}
	}
	if ( cnt == 0) return err;
	cnt = 0;
	for ( A_long i=0; i<w; i++){
		for (A_long j=0; j<h; j++) scanline[j] = data[j * wt + i];
		for (A_long j=1; j<h-1; j++){
			if ( compPix16_8Lv(scanline[j],infoP->Main_Color,infoP->lv)==TRUE){
				PF_Pixel16 d = scanline[j];
				A_long dv = PF_MAX_CHAN16;

				if (compPix16_8Lv(scanline[j-1],infoP->Main_Color,infoP->lv)==FALSE){
					d = scanline[j-1];
					dv = pxV16(d);
				}

				if (compPix16_8Lv(scanline[j+1],infoP->Main_Color,infoP->lv)==FALSE){
					if ( dv > pxV16(scanline[j+1])){
						d = scanline[j+1];
					}
				}
				if (compPix16_8Lv(d,infoP->Main_Color,infoP->lv)==FALSE){
					data[j* wt + i] = d;
				}else{
					cnt++;
				}
			}
		}
		A_long ay = wt * (h-1);
		if (compPix16_8Lv(data[i],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix16_8Lv(data[i+ wt],infoP->Main_Color,infoP->lv)==FALSE){
				data[i] = data[i + wt];
			}else{
				cnt++;
			}
		}
		if (compPix16_8Lv(data[i + ay],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix16_8Lv(data[i + ay - wt],infoP->Main_Color,infoP->lv)==FALSE){
				data[i + ay] = data[i + ay - wt];
			}else{
				cnt++;
			}
		}
	}
	infoP->count = cnt;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec32(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err 	err 	= PF_Err_NONE;
	PF_PixelFloat *scanline;
	scanline = (PF_PixelFloat *)infoP->scanline;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();

	PF_PixelFloat *data;
	data = (PF_PixelFloat *)ae->out->data();
	//水平方向
	A_long cnt = 0;
	for ( A_long j=0; j<h; j++){
		A_long adrY = j*wt;
		for (A_long i=0; i<w; i++) scanline[i] = data[adrY + i];
		for (A_long i=1; i<w-1; i++){
			if ( compPix32_8Lv(scanline[i],infoP->Main_Color,infoP->lv)==TRUE){
				PF_PixelFloat d = scanline[i];
				A_long dv = PF_MAX_CHAN16;

				if (compPix32_8Lv(scanline[i-1],infoP->Main_Color,infoP->lv)==FALSE){
					d = scanline[i-1];
					dv = pxV32(d);
				}

				if (compPix32_8Lv(scanline[i+1],infoP->Main_Color,infoP->lv)==FALSE){
					if ( dv > pxV32(scanline[i+1])){
						d = scanline[i+1];
					}
				}
				if (compPix32_8Lv(d,infoP->Main_Color,infoP->lv)==FALSE){
					data[adrY + i] = d;
				}else{
					cnt++;
				}
			}
		}
		if (compPix32_8Lv(data[adrY],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix32_8Lv(data[adrY+1],infoP->Main_Color,infoP->lv)==FALSE){
				data[adrY] = data[adrY+1];
			}else{
				cnt++;
			}
		}
		if (compPix32_8Lv(data[adrY+ w-1],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix32_8Lv(data[adrY+w -2],infoP->Main_Color,infoP->lv)==FALSE){
				data[adrY+ w-1] = data[adrY+ w-2];
			}else{
				cnt++;
			}
		}
	}
	if ( cnt == 0) return err;
	cnt = 0;
	for ( A_long i=0; i<w; i++){
		for (A_long j=0; j<h; j++) scanline[j] = data[j * wt + i];
		for (A_long j=1; j<h-1; j++){
			if ( compPix32_8Lv(scanline[j],infoP->Main_Color,infoP->lv)==TRUE){
				PF_PixelFloat d = scanline[j];
				A_long dv = PF_MAX_CHAN16;

				if (compPix32_8Lv(scanline[j-1],infoP->Main_Color,infoP->lv)==FALSE){
					d = scanline[j-1];
					dv = pxV32(d);
				}

				if (compPix32_8Lv(scanline[j+1],infoP->Main_Color,infoP->lv)==FALSE){
					if ( dv > pxV32(scanline[j+1])){
						d = scanline[j+1];
					}
				}
				if (compPix32_8Lv(d,infoP->Main_Color,infoP->lv)==FALSE){
					data[j* wt + i] = d;
				}else{
					cnt++;
				}
			}
		}
		A_long ay = wt * (h-1);
		if (compPix32_8Lv(data[i],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix32_8Lv(data[i+ wt],infoP->Main_Color,infoP->lv)==FALSE){
				data[i] = data[i + wt];
			}else{
				cnt++;
			}
		}
		if (compPix32_8Lv(data[i + ay],infoP->Main_Color,infoP->lv)==TRUE){
			if (compPix32_8Lv(data[i + ay - wt],infoP->Main_Color,infoP->lv)==FALSE){
				data[i + ay] = data[i + ay - wt];
			}else{
				cnt++;
			}
		}
	}
	infoP->count = cnt;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCOLOR(ID_Main_Color,&infoP->Main_Color));
	PF_FpLong d;
	ERR(ae->GetFLOAT(ID_level,&d));
	infoP->lv = (A_u_char)(PF_MAX_CHAN8 * d/100);
	infoP->count = 0;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_InData *in_data;
	in_data = ae->in_data;
	PF_Err	err = PF_Err_NONE;


	A_long w = ae->out->width();
	if ( w < ae->out->height() ) w = ae->out->height();
	w *=2;

	//画面をコピー
	ERR(ae->CopyInToOut());
	

	//PF_Handle lH;
	PF_EffectWorld bw1;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->NewWorld(ae->out->width(), 8, PF_PixelFormat_ARGB128, &bw1));
			
		infoP->scanline = bw1.data;
		for ( int i=0; i<20; i++){
			Exec32(ae,infoP);
			if (infoP->count<=0) break;
		}
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->NewWorld(ae->out->width(), 8, PF_PixelFormat_ARGB64, &bw1));
		
		infoP->scanline = bw1.data;
		for ( int i=0; i<20; i++){
			Exec16(ae,infoP);
			if (infoP->count<=0) break;
		}
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->NewWorld(ae->out->width(), 8, PF_PixelFormat_ARGB32, &bw1));

		infoP->scanline = bw1.data;
		for ( int i=0; i<20; i++){
			Exec8(ae,infoP);
			if (infoP->count<=0) break;
		}
		break;
	}
	ae->DisposeWorld(&bw1);
//	PF_DISPOSE_HANDLE(lH);
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
