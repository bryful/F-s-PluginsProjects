#include "TouchDrawStraght.h"



//#define TARGET_COLOR	green
#define TARGET_COLOR	blue


typedef struct {
	PF_Pixel	targetColor;
	A_u_char	lv;
	A_long	count;
} FindTargetColorParam8;
typedef struct {
	PF_Pixel	targetColor;
	A_u_short	lv;
	A_long	count;
} FindTargetColorParam16;
typedef struct {
	PF_Pixel	targetColor;
	PF_FpShort	lv;
	A_long	count;
} FindTargetColorParam32;


typedef struct {
	A_long	table[10];
	A_long	width;
	A_long	height;
	A_long	count;
} FindEdgeParam;

//*************************************************************************************************
//ターゲットと同じ色を探してredにフラグを立てる
//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
static PF_Err 
FindTargetColorSub8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FindTargetColorParam8 *	infoP		= reinterpret_cast<FindTargetColorParam8*>(refcon);
	if ( compPix8Lv(*inP,infoP->targetColor,infoP->lv)==TRUE){
		outP->red = PF_MAX_CHAN8;
		infoP->count++;
	}else{
		outP->red = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FindTargetColorSub16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FindTargetColorParam8 *	infoP		= reinterpret_cast<FindTargetColorParam8*>(refcon);
	if ( compPix8Lv(CONV16TO8( *inP),infoP->targetColor,infoP->lv)==TRUE){
		outP->red = PF_MAX_CHAN16;
		infoP->count++;
	}else{
		outP->red = 0;
	}


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FindTargetColorSub32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FindTargetColorParam8 *	infoP		= reinterpret_cast<FindTargetColorParam8*>(refcon);
	if ( compPix8Lv(CONV32TO8(*inP),infoP->targetColor,infoP->lv)==TRUE){
		outP->red = 1;
		infoP->count++;
	}else{
		outP->red = 0;
	}


	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindTargetColor8(CFsAE *ae , PF_Pixel target_color,A_long color_range ,A_long *count)
{
	PF_Err	err = PF_Err_NONE;
	FindTargetColorParam8 info;
	info.count = 0;
	info.targetColor = target_color;
	info.lv =  RoundByteLong( PF_MAX_CHAN8 * color_range >> 16);
	err = ae->iterate8(	(refconType)&info,FindTargetColorSub8);
	if ( !err){
		*count = info.count;
	}else{
		*count = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindTargetColor16(CFsAE *ae , PF_Pixel target_color,A_long color_range ,A_long *count)
{
	PF_Err	err = PF_Err_NONE;
	FindTargetColorParam8 info;
	info.count = 0;
	info.targetColor = target_color;
	info.lv =  RoundByteLong( PF_MAX_CHAN8 * color_range >> 16);
	err = ae->iterate16((refconType)&info,FindTargetColorSub16);
	if ( !err){
		*count = info.count;
	}else{
		*count = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindTargetColor32(CFsAE *ae , PF_Pixel target_color,A_long color_range ,A_long *count)
{
	PF_Err	err = PF_Err_NONE;
	FindTargetColorParam8 info;
	info.count = 0;
	info.targetColor = target_color;
	info.lv =  RoundByteLong( PF_MAX_CHAN8 * color_range >> 16);
	err = ae->iterate32(	(refconType)&info,FindTargetColorSub32);
	if ( !err){
		*count = info.count;
	}else{
		*count = 0;
	}
	return err;
}
//*************************************************************************************************
//redを見てエッジを抽出 blueにフラグ
//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
static PF_Err 
FindEdgeSub8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FindEdgeParam *	infoP		= reinterpret_cast<FindEdgeParam*>(refcon);

	A_u_char mx = PF_MAX_CHAN8;
	if ( outP->red < mx){
		outP->alpha = 0;
	}else{
		PF_Boolean r = TRUE;
		if ((xL>=1)&&(yL>=1))
			if (outP[infoP->table[0]].red < mx) r = FALSE;
		if (r==TRUE) if (yL>=1)
			if (outP[infoP->table[1]].red < mx) r = FALSE; 
		if ( r == TRUE) if ((xL<=infoP->width)&&(yL>=1))
			if (outP[infoP->table[2]].red < mx) r = FALSE;
		if ( r == TRUE) if (xL>=1)
			if (outP[infoP->table[3]].red < mx) r = FALSE;
		if ( r == TRUE) if (xL<=infoP->width)
			if (outP[infoP->table[5]].red < mx) r = FALSE;
		if ( r == TRUE) if ((xL>=1)&&(yL<=infoP->height))
			if (outP[infoP->table[6]].red < mx) r = FALSE;
		if ( r == TRUE) if (yL<=infoP->height)
			if (outP[infoP->table[7]].red != mx) r = FALSE;
		if ( r == TRUE) if ((xL<=infoP->width)&&(yL<=infoP->height))
			if (outP[infoP->table[8]].red < mx) r = FALSE;

		if( r==TRUE)
			outP->TARGET_COLOR = 0;
		else {
			outP->TARGET_COLOR = mx;
			infoP->count++;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FindEdgeSub16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FindEdgeParam *	infoP		= reinterpret_cast<FindEdgeParam*>(refcon);

	A_u_short mx = PF_MAX_CHAN16;
	if ( outP->red < mx){
		outP->alpha = 0;
	}else{
		PF_Boolean r = TRUE;
		if ((xL>=1)&&(yL>=1))
			if (outP[infoP->table[0]].red < mx) r = FALSE;
		if (r==TRUE) if (yL>=1)
			if (outP[infoP->table[1]].red < mx) r = FALSE; 
		if ( r == TRUE) if ((xL<=infoP->width)&&(yL>=1))
			if (outP[infoP->table[2]].red < mx) r = FALSE;
		if ( r == TRUE) if (xL>=1)
			if (outP[infoP->table[3]].red < mx) r = FALSE;
		if ( r == TRUE) if (xL<=infoP->width)
			if (outP[infoP->table[5]].red < mx) r = FALSE;
		if ( r == TRUE) if ((xL>=1)&&(yL<=infoP->height))
			if (outP[infoP->table[6]].red < mx) r = FALSE;
		if ( r == TRUE) if (yL<=infoP->height)
			if (outP[infoP->table[7]].red < mx) r = FALSE;
		if ( r == TRUE) if ((xL<=infoP->width)&&(yL<=infoP->height))
			if (outP[infoP->table[8]].red < mx) r = FALSE;

		if( r==TRUE)
			outP->TARGET_COLOR = 0;
		else {
			outP->TARGET_COLOR = mx;
			infoP->count++;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FindEdgeSub32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	FindEdgeParam *	infoP		= reinterpret_cast<FindEdgeParam*>(refcon);

	PF_FpShort mx = 1;
	if ( outP->red != mx){
		outP->alpha = 0;
	}else{
		PF_Boolean r = TRUE;
		if ((xL>=1)&&(yL>=1))
			if (outP[infoP->table[0]].red < mx) r = FALSE;
		if (r==TRUE) if (yL>=1)
			if (outP[infoP->table[1]].red < mx) r = FALSE; 
		if ( r == TRUE) if ((xL<=infoP->width)&&(yL>=1))
			if (outP[infoP->table[2]].red < mx) r = FALSE;
		if ( r == TRUE) if (xL>=1)
			if (outP[infoP->table[3]].red < mx) r = FALSE;
		if ( r == TRUE) if (xL<=infoP->width)
			if (outP[infoP->table[5]].red < mx) r = FALSE;
		if ( r == TRUE) if ((xL>=1)&&(yL<=infoP->height))
			if (outP[infoP->table[6]].red < mx) r = FALSE;
		if ( r == TRUE) if (yL<=infoP->height)
			if (outP[infoP->table[7]].red < mx) r = FALSE;
		if ( r == TRUE) if ((xL<=infoP->width)&&(yL<=infoP->height))
			if (outP[infoP->table[8]].red < mx) r = FALSE;

		if( r==TRUE)
			outP->TARGET_COLOR = 0;
		else {
			outP->TARGET_COLOR = mx;
			infoP->count++;
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindEdge8(CFsAE *ae ,A_long *count)
{
	PF_Err	err = PF_Err_NONE;
	FindEdgeParam info;
	info.width = ae->out->width()-2;
	info.height = ae->out->height()-2;
	info.count = 0;

	info.table[0] = -1 -ae->out->widthTrue(); 
	info.table[1] =  0 -ae->out->widthTrue(); 
	info.table[2] =  1 -ae->out->widthTrue(); 
	info.table[3] = -1 ; 
	info.table[4] =  0 ; 
	info.table[5] =  1 ; 
	info.table[6] = -1 +ae->out->widthTrue(); 
	info.table[7] =  0 +ae->out->widthTrue(); 
	info.table[8] =  1 +ae->out->widthTrue(); 
	err = ae->iterate8((refconType)	&info,FindEdgeSub8);
	if ( !err){
		*count = info.count;
	}else{
		*count = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindEdge16(CFsAE *ae , A_long *count)
{
	PF_Err	err = PF_Err_NONE;
	FindEdgeParam info;
	info.width = ae->out->width()-2;
	info.height = ae->out->height()-2;
	info.count = 0;

	info.table[0] = -1 -ae->out->widthTrue(); 
	info.table[1] =  0 -ae->out->widthTrue(); 
	info.table[2] =  1 -ae->out->widthTrue(); 
	info.table[3] = -1 ; 
	info.table[4] =  0 ; 
	info.table[5] =  1 ; 
	info.table[6] = -1 +ae->out->widthTrue(); 
	info.table[7] =  0 +ae->out->widthTrue(); 
	info.table[8] =  1 +ae->out->widthTrue(); 
	err = ae->iterate16((refconType)&info,FindEdgeSub16);
	if ( !err){
		*count = info.count;
	}else{
		*count = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FindEdge32(CFsAE *ae ,A_long *count)
{
	PF_Err	err = PF_Err_NONE;
	FindEdgeParam info;
	info.width = ae->out->width()-2;
	info.height = ae->out->height()-2;
	info.count = 0;

	info.table[0] = -1 -ae->out->widthTrue(); 
	info.table[1] =  0 -ae->out->widthTrue(); 
	info.table[2] =  1 -ae->out->widthTrue(); 
	info.table[3] = -1 ; 
	info.table[4] =  0 ; 
	info.table[5] =  1 ; 
	info.table[6] = -1 +ae->out->widthTrue(); 
	info.table[7] =  0 +ae->out->widthTrue(); 
	info.table[8] =  1 +ae->out->widthTrue(); 
	err = ae->iterate32((refconType)&info,FindEdgeSub32);
	if ( !err){
		*count = info.count;
	}else{
		*count = 0;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
PF_Err FindTarget(CFsAE *ae , PF_Pixel target_color, A_long color_range)
{
	PF_Err	err = PF_Err_NONE;
	A_long count = 0;
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		FindTargetColor32(ae,target_color,color_range,&count);
		if (count>0)
			FindEdge32(ae,&count);
		else
			ae->out->clear();
		break;
	case PF_PixelFormat_ARGB64:
		FindTargetColor16(ae,target_color,color_range,&count);
		if (count>0)
			FindEdge16(ae,&count);
		else
			ae->out->clear();
		break;
	case PF_PixelFormat_ARGB32:
		FindTargetColor8(ae,target_color,color_range,&count);
		if (count>0)
			FindEdge8(ae,&count);
		else
			ae->out->clear();
		break;
	}
	return err;
}
