//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "ColorChangeEng.h"



PF_Err (*subFunc8)(refconType	refcon, A_long xL, A_long yL,PF_Pixel8	*inP, PF_Pixel8	*outP);
PF_Err (*subFunc16)(refconType	refcon, A_long xL, A_long yL,PF_Pixel16	*inP, PF_Pixel16	*outP);
PF_Err (*subFunc32)(refconType	refcon, A_long xL, A_long yL,PF_PixelFloat	*inP, PF_PixelFloat	*outP);

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

	A_u_char		CT[8][3] = {
		{0xFF,0x00,0x00},
		{0x00,0xFF,0x00},
		{0x00,0x00,0xFF},
		{0x00,0xFF,0xFF},
		{0xFF,0x00,0xFF},
		{0xFF,0xFF,0x00},
		{0xFF,0xFF,0xFF},
		{0x00,0x00,0x00}
	};
	A_u_char		DT[8][3] = {
		{0xFF,0x00,0xFF},
		{0xFF,0xFF,0x00},
		{0x00,0x00,0x00},
		{0xFF,0x00,0x00},
		{0x00,0xFF,0x00},
		{0x00,0x00,0xFF},
		{0x00,0xFF,0xFF},
		{ 0xFF,0xFF,0xFF }
	};

	//１個目のパラメータ
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_LV,//パラメータの名前
					0, 				//数値入力する場合の最小値
					PF_MAX_CHAN8,	//数値入力する場合の最大値
					0,				//スライダーの最小値 
					15,				//スライダーの最大値
					0,				//デフォルトの値
					ID_LEVEL
					);

	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_MODE1,STR_EXEC, TRUE,0, ID_ENABLE);


	for (int i = 0; i < 8; i++) {
		std::string str = STR_TARGET0 +std::to_string(i);
		AEFX_CLR_STRUCT(def);
		PF_ADD_CHECKBOX(str.c_str(), STR_EXEC, FALSE, 0, ID_TARGET(i));
		AEFX_CLR_STRUCT(def);
		str = STR_SRCCOL0 + std::to_string(i);
		PF_ADD_COLOR(str.c_str(),
			CT[i][0],
			CT[i][1],
			CT[i][2],
			ID_SRCCOL(i)
		);
		str = STR_DSTCOL0 + std::to_string(i);
		PF_ADD_COLOR(str.c_str(),
			DT[i][0],
			DT[i][1],
			DT[i][2],
			ID_DSTCOL(i)
		);
	}
	

	out_data->num_params = ID_NUM_PARAMS;

	return err;
}

//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage8Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		for (int i=0; i<niP->count;i++){
			if ( (inP->red == niP->Src[i].red)&&(inP->green == niP->Src[i].green)&&(inP->blue == niP->Src[i].blue)) {
				outP->alpha	= inP->alpha;
				outP->red	= niP->Dst[i].red;
				outP->green	= niP->Dst[i].green;
				outP->blue	= niP->Dst[i].blue;
			}
		}
	}

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
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		for (int i=0; i<niP->count;i++){
			if ( (ABS(inP->red - niP->Src[i].red)<=niP->Lv)&&(ABS(inP->green - niP->Src[i].green)<=niP->Lv)&&(ABS(inP->blue - niP->Src[i].blue)<=niP->Lv) ) {
				outP->alpha	= inP->alpha;
				outP->red	= niP->Dst[i].red;
				outP->green	= niP->Dst[i].green;
				outP->blue	= niP->Dst[i].blue;
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage16Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Pixel p =  CONV16TO8(*inP);
		for (int i=0; i<niP->count;i++){
			if ( (p.red == niP->Src[i].red)&&(p.green == niP->Src[i].green)&&(p.blue == niP->Src[i].blue) ) {
				outP->alpha	= inP->alpha;
				outP->red	= niP->Dst16[i].red;
				outP->green	= niP->Dst16[i].green;
				outP->blue	= niP->Dst16[i].blue;
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
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
					
	if (niP){
		PF_Pixel p =  CONV16TO8(*inP);
		for (int i=0; i<niP->count;i++){
			if ( (ABS(p.red - niP->Src[i].red)<=niP->Lv)&&(ABS(p.green - niP->Src[i].green)<=niP->Lv)&&(ABS(p.blue - niP->Src[i].blue)<=niP->Lv) ) {
				outP->alpha	= inP->alpha;
				outP->red	= niP->Dst16[i].red;
				outP->green	= niP->Dst16[i].green;
				outP->blue	= niP->Dst16[i].blue;
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
FilterImage32Zero (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
					
	if (niP){
		PF_Pixel p =  CONV32TO8(*inP);
		for (int i=0; i<niP->count;i++){
			if ( (p.red == niP->Src[i].red)&&(p.green == niP->Src[i].green)&&(p.blue == niP->Src[i].blue) ) {
				outP->alpha	= inP->alpha;
				outP->red	=niP->Dst32[i].red;
				outP->green	=niP->Dst32[i].green;
				outP->blue	=niP->Dst32[i].blue;
			}
		}
	}

	return err;
}
//-----------------------------------------------------------------------------------
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
					
	if (niP){
		PF_Pixel p =  CONV32TO8(*inP);
		for (int i=0; i<niP->count;i++){
			if ( (ABS(p.red - niP->Src[i].red)<=niP->Lv)&&(ABS(p.green - niP->Src[i].green)<=niP->Lv)&&(ABS(p.blue - niP->Src[i].blue)<=niP->Lv) ) {
				outP->alpha	= inP->alpha;
				outP->red	=niP->Dst32[i].red;
				outP->green	=niP->Dst32[i].green;
				outP->blue	=niP->Dst32[i].blue;
			}
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_ENABLE,&infoP->enabled));
	if (!err)
	{
		if ( infoP->enabled == FALSE){
			infoP->count =0;
		}else{
			A_long idx;
			ERR(ae->GetADD(ID_LEVEL,&idx));
			if ( !err){
				infoP->Lv = RoundByteLong(idx);
				PF_Boolean b;
				PF_Pixel p;
				A_long cnt = 0;
				for ( A_long i=0; i<ColorChangeEngCount; i++){
					idx = ID_TARGET0 + i*3;
					ERR(ae->GetCHECKBOX(idx,&b));
					if (err) break;
					if (b==TRUE){
						ERR(ae->GetCOLOR(idx+1,&p));
						if (!err) infoP->Src[cnt] = p;
						ERR(ae->GetCOLOR(idx+2,&p));
						if (!err) {
							infoP->Dst[cnt] = p;
							infoP->Dst16[cnt] = CONV8TO16(p);
							infoP->Dst32[cnt] = CONV8TO32(p);
						}
						cnt++;
					}
				}
				infoP->count = cnt;
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
	if ((infoP->enabled ==TRUE )&&(infoP->count>0)) {
		if ( infoP->Lv ==0){
			subFunc8 = FilterImage8Zero;
			subFunc16 = FilterImage16Zero;
			subFunc32 = FilterImage32Zero;
		}else{
			subFunc8 = FilterImage8;
			subFunc16 = FilterImage16;
			subFunc32 = FilterImage32;
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
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extra)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extra,sizeof(ParamInfo),ID_NUM_PARAMS);
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
	PF_SmartRenderExtra		*extra)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extra,ID_NUM_PARAMS);
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