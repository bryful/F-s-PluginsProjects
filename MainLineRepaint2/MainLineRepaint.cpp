//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "MainLineRepaint.h"


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
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();
	PF_Pixel *scanline;
	A_long *scanlineV;
	scanline = (PF_Pixel *)infoP->scanline;
	scanlineV = (A_long*)(scanline +w*2);

	PF_Pixel *data;
	data = (PF_Pixel *)ae->out->data();
	//水平方向
	A_long cnt = 0;
	for ( A_long j=0; j<h; j++){
		A_long adrY = j*wt;
		//元画像をスキャンラインごとに退避
		for (A_long i = 0; i < w; i++) {
			scanline[i] = data[adrY + i];
			//主線・透明と明るさのテーブル
			//　明るさ 0-255
			// 透明 256 PPTRANS
			// 主線 257 PPMAIN
			scanlineV[i] = pV8(data[adrY + i], infoP->Main_Color, infoP->lv);
		}

		for (A_long i=1; i<w-1; i++){
			if( scanlineV[i]==PPMAIN) {
				PF_Pixel dst = infoP->Main_Color;
				PF_Boolean dv = TRUE;
				PF_Pixel lp = infoP->Main_Color;
				A_long lv = PPMAIN;
				PF_Pixel rp = infoP->Main_Color;
				A_long rv = PPMAIN;

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = i -k;
					if (k2 < 0) break;
					if (scanlineV[k2] < PPMAIN)
					{
						lp = scanline[k2];
						lv = scanlineV[k2];
						break;
					}
				}
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k3 = i +k;
					if ( k3>= w) break;
					if (scanlineV[k3] < PPMAIN)
					{
						rp = scanline[k3];
						rv = scanlineV[k3];
						break;
					}
				}
				if ((lv <= PF_MAX_CHAN8) && (rv <= PF_MAX_CHAN8))
				{
					if (lv <= rv)
					{
						//dst = { 255,255,0,0 };
						dst = lp;
					}
					else {
						//dst = { 255,255,255,0 };
						dst = rp;
					}
				}
				else if ((lv == PPMAIN) && (rv <= PF_MAX_CHAN8))
				{
					//dst = { 255,0,255,0 };
					dst = rp;
				}
				else if ((lv <= PF_MAX_CHAN8) && (rv == PPMAIN))
				{
					//dst = { 255,0,255,255 };
					dst = lp;
				}
				else if ((lv == PPTRANS) && (rv == PPTRANS))
				{
					dst = {0,0,0,0};
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE){
					data[adrY + i] = dst;
				}else{
					cnt++;
				}
			}
		}
		if (scanlineV[0]>=PPMAIN) {
			if (scanlineV[1]< PPMAIN) {
				data[adrY] = scanline[1];
			}else{
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[adrY + w - 1] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	if ( cnt == 0) return err;
	// 垂直方向
	cnt = 0;
	for ( A_long i=0; i<w; i++){
		for (A_long j = 0; j < h; j++) {
			scanline[j] = data[j * wt + i];
			scanlineV[j] = pV8(data[j * wt + i], infoP->Main_Color, infoP->lv);
		}

		

		for (A_long j=1; j<h-1; j++){
			if (scanlineV[j] == PPMAIN) {
				PF_Pixel dst = infoP->Main_Color;
				PF_Boolean dv = TRUE;
				PF_Pixel lp = infoP->Main_Color;
				PF_Pixel rp = infoP->Main_Color;
				A_long lv = PPMAIN;
				A_long rv = PPMAIN;
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j-k;
					if (k2 < 0) break;
					if (scanlineV[k2] != PPMAIN)
					{
						lp = scanline[k2];
						lv = scanlineV[k2];
						break;
					}
				}

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j+ k ;
					if (k2 >= h) break;
					if (scanlineV[k2] != PPMAIN)
					{
						rp = scanline[k2];
						rv = scanlineV[k2];
						break;
					}
				}


				if ((lv <= PF_MAX_CHAN8) && (rv <= PF_MAX_CHAN8))
				{
					if (lv <= rv)
					{
						dst = lp;
					}
					else {
						dst = rp;
					}
				}
				else if ((lv == PPMAIN) && (rv <= PF_MAX_CHAN8))
				{
					dst = rp;
				}
				else if ((lv <= PF_MAX_CHAN8) && (rv == PPMAIN))
				{
					dst = lp;
				}
				else if ((rv == PPTRANS) && (lv == PPTRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[j * wt + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[i] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[wt * (h - 1) + i] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	infoP->count = cnt;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec16(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err 	err = PF_Err_NONE;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();
	PF_Pixel16* scanline;
	A_long* scanlineV;
	scanline = (PF_Pixel16*)infoP->scanline;
	scanlineV = (A_long*)(scanline + w * 2);

	PF_Pixel16* data;
	data = (PF_Pixel16*)ae->out->data();
	
	PF_Pixel16 m2 = CONV8TO16(infoP->Main_Color);
	//水平方向
	A_long cnt = 0;
	for (A_long j = 0; j < h; j++) {
		A_long adrY = j * wt;
		//元画像をスキャンラインごとに退避
		for (A_long i = 0; i < w; i++) {
			scanline[i] = data[adrY + i];
			//主線・透明と明るさのテーブル
			//　明るさ 0-255
			// 透明 256 PPTRANS
			// 主線 257 PPMAIN
			scanlineV[i] = pV16(data[adrY + i], infoP->Main_Color, infoP->lv);
		}

		for (A_long i = 1; i < w - 1; i++) {
			if (scanlineV[i] == PPMAIN) {
				PF_Pixel16 dst = m2;
				PF_Boolean dv = TRUE;
				PF_Pixel16 lp = m2;
				A_long lv = PPMAIN;
				PF_Pixel16 rp = m2;
				A_long rv = PPMAIN;

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = i - k;
					if (k2 < 0) break;
					if (scanlineV[k2] < PPMAIN)
					{
						lp = scanline[k2];
						lv = scanlineV[k2];
						break;
					}
				}
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k3 = i + k;
					if (k3 >= w) break;
					if (scanlineV[k3] < PPMAIN)
					{
						rp = scanline[k3];
						rv = scanlineV[k3];
						break;
					}
				}
				if ((lv <= PF_MAX_CHAN8) && (rv <= PF_MAX_CHAN8))
				{
					if (lv <= rv)
					{
						//dst = { 255,255,0,0 };
						dst = lp;
					}
					else {
						//dst = { 255,255,255,0 };
						dst = rp;
					}
				}
				else if ((lv == PPMAIN) && (rv <= PF_MAX_CHAN8))
				{
					//dst = { 255,0,255,0 };
					dst = rp;
				}
				else if ((lv <= PF_MAX_CHAN8) && (rv == PPMAIN))
				{
					//dst = { 255,0,255,255 };
					dst = lp;
				}
				else if ((lv == PPTRANS) && (rv == PPTRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[adrY + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[adrY] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[adrY + w - 1] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	if (cnt == 0) return err;
	// 垂直方向
	cnt = 0;
	for (A_long i = 0; i < w; i++) {
		for (A_long j = 0; j < h; j++) {
			scanline[j] = data[j * wt + i];
			scanlineV[j] = pV16(data[j * wt + i], infoP->Main_Color, infoP->lv);
		}



		for (A_long j = 1; j < h - 1; j++) {
			if (scanlineV[j] == PPMAIN) {
				PF_Pixel16 dst = m2;
				PF_Boolean dv = TRUE;
				PF_Pixel16 lp = m2;
				PF_Pixel16 rp = m2;
				A_long lv = PPMAIN;
				A_long rv = PPMAIN;
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j - k;
					if (k2 < 0) break;
					if (scanlineV[k2] != PPMAIN)
					{
						lp = scanline[k2];
						lv = scanlineV[k2];
						break;
					}
				}

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j + k;
					if (k2 >= h) break;
					if (scanlineV[k2] != PPMAIN)
					{
						rp = scanline[k2];
						rv = scanlineV[k2];
						break;
					}
				}


				if ((lv <= PF_MAX_CHAN8) && (rv <= PF_MAX_CHAN8))
				{
					if (lv <= rv)
					{
						dst = lp;
					}
					else {
						dst = rp;
					}
				}
				else if ((lv == PPMAIN) && (rv <= PF_MAX_CHAN8))
				{
					dst = rp;
				}
				else if ((lv <= PF_MAX_CHAN8) && (rv == PPMAIN))
				{
					dst = lp;
				}
				else if ((rv == PPTRANS) && (lv == PPTRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[j * wt + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[i] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[wt * (h - 1) + i] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	infoP->count = cnt;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err Exec32(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err 	err = PF_Err_NONE;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();
	PF_PixelFloat *scanline;
	A_long* scanlineV;
	scanline = (PF_PixelFloat*)infoP->scanline;
	scanlineV = (A_long*)(scanline + w * 2);

	PF_PixelFloat* data;
	data = (PF_PixelFloat*)ae->out->data();

	PF_PixelFloat m2 = CONV8TO32(infoP->Main_Color);
	//水平方向
	A_long cnt = 0;
	for (A_long j = 0; j < h; j++) {
		A_long adrY = j * wt;
		//元画像をスキャンラインごとに退避
		for (A_long i = 0; i < w; i++) {
			scanline[i] = data[adrY + i];
			//主線・透明と明るさのテーブル
			//　明るさ 0-255
			// 透明 256 PPTRANS
			// 主線 257 PPMAIN
			scanlineV[i] = pV32(data[adrY + i], infoP->Main_Color, infoP->lv);
		}

		for (A_long i = 1; i < w - 1; i++) {
			if (scanlineV[i] == PPMAIN) {
				PF_PixelFloat dst = m2;
				PF_Boolean dv = TRUE;
				PF_PixelFloat lp = m2;
				A_long lv = PPMAIN;
				PF_PixelFloat rp = m2;
				A_long rv = PPMAIN;

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = i - k;
					if (k2 < 0) break;
					if (scanlineV[k2] < PPMAIN)
					{
						lp = scanline[k2];
						lv = scanlineV[k2];
						break;
					}
				}
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k3 = i + k;
					if (k3 >= w) break;
					if (scanlineV[k3] < PPMAIN)
					{
						rp = scanline[k3];
						rv = scanlineV[k3];
						break;
					}
				}
				if ((lv <= PF_MAX_CHAN8) && (rv <= PF_MAX_CHAN8))
				{
					if (lv <= rv)
					{
						//dst = { 255,255,0,0 };
						dst = lp;
					}
					else {
						//dst = { 255,255,255,0 };
						dst = rp;
					}
				}
				else if ((lv == PPMAIN) && (rv <= PF_MAX_CHAN8))
				{
					//dst = { 255,0,255,0 };
					dst = rp;
				}
				else if ((lv <= PF_MAX_CHAN8) && (rv == PPMAIN))
				{
					//dst = { 255,0,255,255 };
					dst = lp;
				}
				else if ((lv == PPTRANS) && (rv == PPTRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[adrY + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[adrY] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[adrY + w - 1] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	if (cnt == 0) return err;
	// 垂直方向
	cnt = 0;
	for (A_long i = 0; i < w; i++) {
		for (A_long j = 0; j < h; j++) {
			scanline[j] = data[j * wt + i];
			scanlineV[j] = pV32(data[j * wt + i], infoP->Main_Color, infoP->lv);
		}



		for (A_long j = 1; j < h - 1; j++) {
			if (scanlineV[j] == PPMAIN) {
				PF_PixelFloat dst = m2;
				PF_Boolean dv = TRUE;
				PF_PixelFloat lp = m2;
				PF_PixelFloat rp = m2;
				A_long lv = PPMAIN;
				A_long rv = PPMAIN;
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j - k;
					if (k2 < 0) break;
					if (scanlineV[k2] != PPMAIN)
					{
						lp = scanline[k2];
						lv = scanlineV[k2];
						break;
					}
				}

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j + k;
					if (k2 >= h) break;
					if (scanlineV[k2] != PPMAIN)
					{
						rp = scanline[k2];
						rv = scanlineV[k2];
						break;
					}
				}


				if ((lv <= PF_MAX_CHAN8) && (rv <= PF_MAX_CHAN8))
				{
					if (lv <= rv)
					{
						dst = lp;
					}
					else {
						dst = rp;
					}
				}
				else if ((lv == PPMAIN) && (rv <= PF_MAX_CHAN8))
				{
					dst = rp;
				}
				else if ((lv <= PF_MAX_CHAN8) && (rv == PPMAIN))
				{
					dst = lp;
				}
				else if ((rv == PPTRANS) && (lv == PPTRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[j * wt + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[i] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[wt * (h - 1) + i] = scanline[w - 1];
			}
			else {
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
	infoP->scanCount = 3;
	infoP->scanLength = 4;
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
		ERR(ae->NewWorld(w, 8, PF_PixelFormat_ARGB128, &bw1));
			
		infoP->scanline = bw1.data;
		for ( int i=0; i<infoP->scanCount; i++){
			Exec32(ae,infoP);
			if (infoP->count<=0) break;
		}
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->NewWorld(w, 8, PF_PixelFormat_ARGB64, &bw1));
		
		infoP->scanline = bw1.data;
		for (int i = 0; i < infoP->scanCount; i++) {
			Exec16(ae,infoP);
			if (infoP->count<=0) break;
		}
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->NewWorld(w, 8, PF_PixelFormat_ARGB32, &bw1));

		infoP->scanline = bw1.data;
		for (int i = 0; i < infoP->scanCount; i++) {
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
