
#pragma once

#ifndef FSTDPSET_H
#define FSTDPSET_H


#include "AEConfig.h"
#include "entry.h"


//#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"

#if defined(PF_AE100_PLUG_IN_VERSION)
	#include "AEFX_SuiteHelper.h"
#else
	#include "PF_Suite_Helper.h"
#endif

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#include "../FsLibrary/FsUtils.h"



typedef struct {
	PF_PixelPtr		data;
	A_long			width;
	A_long			height;
	A_long			widthTrue;
	A_long			widthOffset;
	PF_FixedPoint	pos;
	PF_Fixed		opacity;
} PsetPrm; 



//-------------------------------------------------------
inline A_u_char mixCh8(A_long c0, A_long c1)
{
	return RoundByteLong(c0 + c1 - (c0 * c1 >>8));
}
//-------------------------------------------------------
inline A_u_short mixCh16(A_long c0, A_long c1)
{
	A_long v = c0 + c1 - (c0 * c1 >>16);
	if (v<0) { v=0; }else if(v>PF_MAX_CHAN16){v = PF_MAX_CHAN16;}
	return (A_u_short)v;
}
//-------------------------------------------------------
inline PF_FpShort mixCh32(PF_FpShort c0, PF_FpShort c1)
{
	PF_FpShort v = c0 + c1 - (c0 * c1);
	if (v<0) { v=0; }
	return v;
}

//-------------------------------------------------------
/*
	アルファーへ書き込み
*/
inline PF_Boolean PsetAlpha8(PsetPrm *prm)
{
	PF_Boolean ret = FALSE;
	PF_Fixed opacity = prm->opacity;
	if (opacity<=0) opacity = 0;
	if (opacity>(1L<<16)) opacity = 1L<<16;
	A_long v = PF_MAX_CHAN8 * opacity >> 16;
	if (v<=0) return ret;

	PF_Pixel *data;
	data = (PF_Pixel *)prm->data;
	PF_FixedPoint pos = prm->pos;
	A_long w	= prm->width;
	A_long wt = prm->widthTrue;
	A_long h	= prm->height;
	A_long x1,y1,x2,y2;

	
	x1 = prm->pos.x >> 16;
	y1 = prm->pos.y >> 16;
	x2 = x1+1;
	y2 = y1+1;

	if ( (x1>=w)||(y1>=h)||(x2<0)||(y2<0) ) return ret;

	PF_FpShort xx2,yy2,xx1,yy1;
	xx2 = (PF_FpShort)(prm->pos.x & 0xFFFF)/65536;
	yy2 = (PF_FpShort)(prm->pos.y & 0xFFFF)/65536;
	xx1 = (PF_FpShort)1 - xx2;
	yy1 = (PF_FpShort)1 - yy2;

	A_long vv;
	A_long target = x1 + (y1 * wt);
	//まず、ぴったりの時
	if ( (xx2 == 0)&&(yy2 == 0) ) {
		if ( (x1>=0)&&(y1>=0) ) {
			data[target].green = mixCh8(data[target].green,v);
			ret = TRUE;
		}
	}else{
		//まず左上
		if ( ((x1>=0)&&(y1>=0)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx1 * yy1);
			data[target].green = mixCh8(data[target].green,vv);
			ret = TRUE;
		}
		//右上
		target++;
		if ( ((x2<w)&&(y1>=0)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx2 * yy1);
			data[target].green = mixCh8(data[target].green,vv);
			ret = TRUE;
		}
		//右下
		target+= wt;
		if ( ((x2<w)&&(y2<h)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx2 * yy2);
			data[target].green = mixCh8(data[target].green,vv);
			ret = TRUE;
		}
		//左下
		target--;
		if ( ((x1>=0)&&(y2<h)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx1 * yy2);
			data[target].green = mixCh8(data[target].green,vv);
			ret = TRUE;
		}
	}
	return ret;
}
//-------------------------------------------------------
/*
	アルファーへ書き込み
*/
inline PF_Boolean PsetAlpha16(PsetPrm *prm)
{
	PF_Boolean ret = FALSE;
	PF_Fixed opacity = prm->opacity;
	if (opacity<=0) opacity = 0;
	if (opacity>(1L<<16)) opacity = 1L<<16;
	A_long v = PF_MAX_CHAN16 * opacity >> 16;
	if (v<=0) return ret;

	PF_Pixel16 *data;
	data = (PF_Pixel16 *)prm->data;
	PF_FixedPoint pos = prm->pos;
	A_long w	= prm->width;
	A_long wt = prm->widthTrue;
	A_long h	= prm->height;
	A_long x1,y1,x2,y2;

	
	x1 = prm->pos.x >> 16;
	y1 = prm->pos.y >> 16;
	x2 = x1+1;
	y2 = y1+1;

	if ( (x1>=w)||(y1>=h)||(x2<0)||(y2<0) ) return ret;

	PF_FpShort xx2,yy2,xx1,yy1;
	xx2 = (PF_FpShort)(prm->pos.x & 0xFFFF)/65536;
	yy2 = (PF_FpShort)(prm->pos.y & 0xFFFF)/65536;
	xx1 = (PF_FpShort)1 - xx2;
	yy1 = (PF_FpShort)1 - yy2;

	A_long vv;
	A_long target = x1 + (y1 * wt);
	//まず、ぴったりの時
	if ( (xx2 == 0)&&(yy2 == 0) ) {
		if ( (x1>=0)&&(y1>=0) ) {
			data[target].green = mixCh16(data[target].green,v);
			ret = TRUE;
		}
	}else{
		//まず左上
		if ( ((x1>=0)&&(y1>=0)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx1 * yy1);
			data[target].green = mixCh16(data[target].green,vv);
			ret = TRUE;
		}
		//右上
		target++;
		if ( ((x2<w)&&(y1>=0)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx2 * yy1);
			data[target].green = mixCh16(data[target].green,vv);
			ret = TRUE;
		}
		//右下
		target+= wt;
		if ( ((x2<w)&&(y2<h)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx2 * yy2);
			data[target].green = mixCh16(data[target].green,vv);
			ret = TRUE;
		}
		//左下
		target--;
		if ( ((x1>=0)&&(y2<h)) == TRUE){
			vv = (A_long)( (PF_FpShort)v * xx1 * yy2);
			data[target].green = mixCh16(data[target].green,vv);
			ret = TRUE;
		}
	}
	return ret;
}
//-------------------------------------------------------
/*
	アルファーへ書き込み
*/
inline PF_Boolean PsetAlpha32(PsetPrm *prm)
{
	PF_Boolean ret = FALSE;
	PF_FpShort v = (PF_FpShort)prm->opacity/65536;
	if (v<=0) v = 0;
	if (v<=0) return ret;

	PF_PixelFloat *data;
	data = (PF_PixelFloat *)prm->data;
	PF_FixedPoint pos = prm->pos;
	A_long w	= prm->width;
	A_long wt = prm->widthTrue;
	A_long h	= prm->height;
	A_long x1,y1,x2,y2;

	PF_FpShort xx2,yy2,xx1,yy1;
	
	x1 = prm->pos.x >> 16;
	y1 = prm->pos.y >> 16;



	x2 = x1+1;
	y2 = y1+1;
	xx2 = (PF_FpShort)(prm->pos.x & 0xFFFF)/65536;
	yy2 = (PF_FpShort)(prm->pos.y & 0xFFFF)/65536;
	xx1 = 1 - xx2;
	yy1 = 1 - yy2;
	
	if ( (x1>=w)||(y1>=h)||(x2<0)||(y2<0) ) return ret;


	PF_FpShort vv;
	long target = x1 + (y1 * wt);
	//まず、ぴったりの時
	if ( (xx2 == 0)&&(yy2 == 0) ) {
		if ( (x1>=0)&&(y1>=0) ) {
			data[target].green = mixCh32(data[target].green,v);
			ret = TRUE;
		}
	}else{
		//まず左上
		if ( ((x1>=0)&&(y1>=0)) == TRUE){
			vv = v * xx1 * yy1;
			data[target].green = mixCh32(data[target].green,vv);
			ret = TRUE;
		}
		//右上
		target++;
		if ( ((x2<w)&&(y1>=0)) == TRUE){
			vv = v * xx2 * yy1;
			data[target].green = mixCh32(data[target].green,vv);
			ret = TRUE;
		}
		//右下
		target+= wt;
		if ( ((x2<w)&&(y2<h)) == TRUE){
			vv = v * xx2 * yy2;
			data[target].green = mixCh32(data[target].green,vv);
			ret = TRUE;
		}
		//左下
		target--;
		if ( ((x1>=0)&&(y2<h)) == TRUE){
			vv = v * xx1 * yy2;
			data[target].green = mixCh32(data[target].green,vv);
			ret = TRUE;
		}
	}
	return ret;
}


#endif
