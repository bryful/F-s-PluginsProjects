//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------
#include "SelectedBlur.h"

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
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	STR_BLUR_VALUE,//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					30,				//スライダーの最大値
					0,				//デフォルトの値
					ID_BLUR_VALUE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_TARGET_RNG,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					16,				//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_TARGET_RANGE
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					TRUE,
					0,
					ID_TARGET_ENABLED0
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL0, 
					0xFF,
					0x00,
					0x00,
					ID_TARGET_COL0
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					TRUE,
					0,
					ID_TARGET_ENABLED1
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL1, 
					0x00,
					0xFF,
					0x00,
					ID_TARGET_COL1
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					FALSE,
					0,
					ID_TARGET_ENABLED2
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL2, 
					0x00,
					0x00,
					0xFF,
					ID_TARGET_COL2
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					FALSE,
					0,
					ID_TARGET_ENABLED3
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL3, 
					0xFF,
					0x00,
					0xFF,
					ID_TARGET_COL3
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					FALSE,
					0,
					ID_TARGET_ENABLED4
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL4, 
					0x00,
					0xFF,
					0xFF,
					ID_TARGET_COL4
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					FALSE,
					0,
					ID_TARGET_ENABLED5
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL5, 
					0x00,
					0x80,
					0x80,
					ID_TARGET_COL5
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					FALSE,
					0,
					ID_TARGET_ENABLED6
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL6, 
					0x80,
					0x00,
					0x80,
					ID_TARGET_COL6
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ENABLE0,
					STR_TARGET_ENABLE1,
					FALSE,
					0,
					ID_TARGET_ENABLED7
					);
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR_TARGET_COL7, 
					0x80,
					0x00,
					0x00,
					ID_TARGET_COL7
					);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
CopyImage8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	BlurInfo *	info		= reinterpret_cast<BlurInfo*>(refcon);
	if (info){
		if ( info->isTargetCopy==TRUE){
			PF_Boolean b = FALSE;
			if ( inP->alpha >=PF_MAX_CHAN8){
				for ( A_long i=0; i<info->col_count; i++){
					if (  ( F_ABS(inP->blue  - info->target_col[i].blue )<=info->col_range)
						&&( F_ABS(inP->green - info->target_col[i].green)<=info->col_range)
						&&( F_ABS(inP->red   - info->target_col[i].red  )<=info->col_range) ) {
							b = TRUE;
							break;
					}
				}
			}
			if ( b ==FALSE){
				outP->alpha =
				outP->blue =
				outP->green =
				outP->red = 0;
			}else{
				outP->alpha	= inP->alpha;
				outP->red	= inP->red;
				outP->green	= inP->green;
				outP->blue	= inP->blue;
			}
		}else{
			//透明のところのみを複写
			if ( outP->alpha ==0){
				outP->alpha	= inP->alpha;
				outP->red	= inP->red;
				outP->green	= inP->green;
				outP->blue	= inP->blue;
			}
		}
		
	}

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
CopyImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	BlurInfo *	info		= reinterpret_cast<BlurInfo*>(refcon);
	if (info){
		if ( info->isTargetCopy==TRUE){
			PF_Boolean b = FALSE;
			if ( inP->alpha >=PF_MAX_CHAN16){
				for ( A_long i=0; i<info->col_count; i++){
					PF_Pixel p;
					p.red	= C16TO8(inP->red); 
					p.green	= C16TO8(inP->green); 
					p.blue	= C16TO8(inP->blue); 
					if (  ( F_ABS(p.blue  - info->target_col[i].blue )<=info->col_range)
						&&( F_ABS(p.green - info->target_col[i].green)<=info->col_range)
						&&( F_ABS(p.red   - info->target_col[i].red  )<=info->col_range) ) {
							b = TRUE;
							break;
					}
				}
			}
			if ( b ==FALSE){
				outP->alpha =
				outP->blue =
				outP->green =
				outP->red = 0;
			}else{
				outP->alpha	= inP->alpha;
				outP->red	= inP->red;
				outP->green	= inP->green;
				outP->blue	= inP->blue;
			}
		}else{
			//透明のところのみを複写
			if ( outP->alpha ==0){
				outP->alpha	= inP->alpha;
				outP->red	= inP->red;
				outP->green	= inP->green;
				outP->blue	= inP->blue;
			}
		}
		
	}
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
CopyImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	
	BlurInfo *	info		= reinterpret_cast<BlurInfo*>(refcon);
	if (info){
		if ( info->isTargetCopy==TRUE){
			PF_Boolean b = FALSE;
			PF_Pixel p;
			p.alpha	= RoundByteLong( FS_CONVERT32TO8( inP->alpha)); 
			if ( p.alpha >=PF_MAX_CHAN8){
				for ( A_long i=0; i<info->col_count; i++){
					p.red	= RoundByteLong( FS_CONVERT32TO8(inP->red) ); 
					p.green	= RoundByteLong( FS_CONVERT32TO8(inP->green)); 
					p.blue	= RoundByteLong( FS_CONVERT32TO8(inP->blue)); 
					if (  ( F_ABS(p.blue  - info->target_col[i].blue )<=info->col_range)
						&&( F_ABS(p.green - info->target_col[i].green)<=info->col_range)
						&&( F_ABS(p.red   - info->target_col[i].red  )<=info->col_range) ) {
							b = TRUE;
							break;
					}
				}
			}
			if ( b ==FALSE){
				outP->alpha =
				outP->blue =
				outP->green =
				outP->red = 0;
			}else{
				outP->alpha	= inP->alpha;
				outP->red	= inP->red;
				outP->green	= inP->green;
				outP->blue	= inP->blue;
			}
		}else{
			//透明のところのみを複写
			if ( outP->alpha ==0){
				outP->alpha	= inP->alpha;
				outP->red	= inP->red;
				outP->green	= inP->green;
				outP->blue	= inP->blue;
			}
		}
	}
	return err;
}

//-----------------------------------------------------------------------------------
static PF_Err 
GetParams(CFsAE *ae , BlurInfo *infoP)
{
	PF_Err				err		= PF_Err_NONE;
	if(!err) err = ae->GetADD(ID_BLUR_VALUE,&infoP->blur_value);
	if(!err){
		if ( infoP->blur_value>0)
			infoP->blur_value = ae->downScale(infoP->blur_value);
	}

	if(!err) {
		PF_Fixed v;
		err = ae->GetFIXED(ID_TARGET_RANGE,&v);
		if (!err) infoP->col_range = RoundByteLong(PF_MAX_CHAN8 * v /(100L<<16));
	}
	if(!err) {
		A_long count =0;
		A_long idx = ID_TARGET_ENABLED0;
		PF_Boolean b;
		PF_Pixel c;
		for ( A_long i =0 ; i<COLOR_COUNT; i++){
			idx = ID_TARGET_ENABLED0 + i*2;
			err = ae->GetCHECKBOX(idx,&b);
			if (err) break;
			if ( b==TRUE){
				err = ae->GetCOLOR(idx+1,&c);
				if (err) break;
				infoP->target_col[count] = c;
				count++;
			}
		}
		if ( !err) infoP->col_count = count;
	}
	return err;
}
//-----------------------------------------------------------------------------------
static void 
GetBlurPrm(CFsAE *ae , BlurInfo *infoP,blurPrm *bp)
{
	bp->blur_value	= infoP->blur_value;
	bp->data		= ae->output->data;
	bp->width		= ae->output->width;
	bp->height		= ae->output->height;
	bp->in_data		= ae->in_data;
	bp->scanlineH	= NULL;
	if ( bp->width>=bp->height){
		bp->scanlineWidth = bp->width;
	}else{
		bp->scanlineWidth = bp->height;
	}
}
//-----------------------------------------------------------------------------------
static PF_Err 
GetParams(CFsAE *ae , BlurInfo *infoP,blurPrm *bp)
{
	PF_Err				err		= PF_Err_NONE;
	err = GetParams(ae,infoP);
	if (!err) GetBlurPrm(ae,infoP,bp);
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
Exec(CFsAE *ae , BlurInfo *infoP,blurPrm *bp)
{
	PF_Err				err		= PF_Err_NONE;
	if ((infoP->blur_value<=0)||(infoP->col_count<=1)) {
		ae->CopyInToOut();
		return err;
	}
	switch(ae->pixelFormat())
	{
		case PF_PixelFormat_ARGB128:
			infoP->isTargetCopy = TRUE;
			ERR( ae->iterate32((refconType)infoP,CopyImage32));
			if (!err){
				bp->widthTrue = ae->output->rowbytes / sizeof(PF_PixelFloat);
				bp->scanlineH = ae->NewHandle(bp->scanlineWidth *sizeof(PF_PixelFloat)*2);
				if ( !bp->scanlineH ) return PF_Err_INTERNAL_STRUCT_DAMAGED;
				BlurSub32(bp);
				if ( bp->scanlineH != NULL){
					ae->DisposeHandle(bp->scanlineH);
					bp->scanlineH = NULL;
				}
				infoP->isTargetCopy = FALSE;
				ERR( ae->iterate32((refconType)infoP,CopyImage32));
			}
		break;
		case PF_PixelFormat_ARGB64:
			infoP->isTargetCopy = TRUE;
			ERR( ae->iterate16((refconType)infoP,CopyImage16));
			if (!err){
				bp->widthTrue = ae->output->rowbytes / sizeof(PF_Pixel16);
				bp->scanlineH = ae->NewHandle(bp->scanlineWidth *sizeof(PF_Pixel16)*2);
				if ( !bp->scanlineH ) return PF_Err_INTERNAL_STRUCT_DAMAGED;
				BlurSub16(bp);
				if ( bp->scanlineH != NULL){
					ae->DisposeHandle(bp->scanlineH);
					bp->scanlineH = NULL;
				}
				infoP->isTargetCopy = FALSE;
				ERR( ae->iterate16((refconType)infoP,CopyImage16));
			}
		break;
		case PF_PixelFormat_ARGB32:
			infoP->isTargetCopy = TRUE;
			ERR( ae->iterate8((refconType)infoP,CopyImage8));
			if (!err){
				bp->widthTrue = ae->output->rowbytes / sizeof(PF_Pixel);
				bp->scanlineH = ae->NewHandle(bp->scanlineWidth *sizeof(PF_Pixel)*2);
				if ( !bp->scanlineH ) return PF_Err_INTERNAL_STRUCT_DAMAGED;
				BlurSub8(bp);
				if ( bp->scanlineH != NULL){
					ae->DisposeHandle(bp->scanlineH);
					bp->scanlineH = NULL;
				}
				infoP->isTargetCopy = FALSE;
				ERR( ae->iterate8((refconType)infoP,CopyImage8));
			}
		break;
	}
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err 
Render ( 
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;

	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err = ae.resultErr(); 
	if ( !err){
		BlurInfo info;
		blurPrm bp;
		ERR(GetParams(&ae, &info,&bp));
		ERR(Exec(&ae, &info,&bp));
	}
	return err;
}

//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_dataP,
	PF_PreRenderExtra	*extraP)
{
	PF_Err err = PF_Err_NONE;
	CFsAE ae(in_data,out_dataP,extraP,sizeof(BlurInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		BlurInfo *infoP = reinterpret_cast<BlurInfo*>(ae.LockPreRenderData());
		if (infoP){
			ae.SetHostPreRenderData();
			err = GetParams(&ae,infoP);
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
		BlurInfo *infoP = reinterpret_cast<BlurInfo*>(ae.LockPreRenderData());

		if (infoP){
			blurPrm bp;
			GetBlurPrm(&ae,infoP,&bp);
			ERR(Exec(&ae,infoP,&bp));
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