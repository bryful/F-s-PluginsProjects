//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------
#pragma once

#ifndef GuideFrame_H
#define GuideFrame_H


#include "Fs_Target.h"

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
	#define refconType void*
#else
	#include "PF_Suite_Helper.h"
	#define refconType A_long
#endif

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

#include "../FsLibrary/FsAE.h"
//#include "FsAE.h"


	
#define STR_COLOR			"color"
#define STR_TOPLEFT			"TopLeft"
#define STR_BOTTOMRIGHT		"BottomRight"
#define STR_CHK1			"Check"
#define STR_CHK2			"on"
#define STR_SMOOTH1			"Smooth"
#define STR_SMOOTH2			"on"


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_COLOR,	//
	ID_TOPLEFT,		//
	ID_BOTTOMRIGHT,		//
	ID_CHK,	
	ID_SMOOTH,	


	ID_NUM_PARAMS
	};
typedef struct ParamInfo {
	PF_Pixel  		color;
	PF_FixedPoint	top_left;
	PF_FixedPoint	bottom_right;
	PF_Boolean		check;
	PF_Boolean		smooth;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-----------------------------------------------------------------------------------
inline	PF_Pixel16 toPixel16(PF_Pixel s){
		PF_Pixel16 r;
		r.alpha	= (unsigned short)CONVERT8TO16(s.alpha);
		r.red		= (unsigned short)CONVERT8TO16(s.red);
		r.green	= (unsigned short)CONVERT8TO16(s.green);
		r.blue	= (unsigned short)CONVERT8TO16(s.blue);
		return r;
	}
/*
class CGuideFrame : public CFsAE{
public:
	CGuideFrame(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output ):CFsAE(in_data,out_data,params,output){
			getParams(in_data,out_data,params,output);
		}
	//パラメータを獲得する関数
	PF_Err getParams(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output );
	//以下固有のパラメータ
	PF_Pixel  		color;
	PF_FixedPoint	top_left;
	PF_FixedPoint	bottom_right;
	PF_Boolean		check;
	PF_Boolean		smooth;


	//枠を描く
	void drawRect(){
		long x0 = top_left.x >> 16;
		long y0 = top_left.y >> 16;
		long x1 = (bottom_right.x >> 16)-1;
		long y1 = (bottom_right.y >> 16)-1;
		
		out->lineHeiht = 2;
		if (is16Bit()==TRUE){
			PF_Pixel16 c16 = toPixel16(color);
			out->box16(x0,y0,x1,y1,c16); 
			x0++;
			y0++;
			x1--;
			y1--;
			out->box16(x0,y0,x1,y1,c16); 
		}else{
			out->box8(x0,y0,x1,y1,color); 
			x0++;
			y0++;
			x1--;
			y1--;
			out->box8(x0,y0,x1,y1,color); 
		}
	}
	//-------------
	void chkW8()
	{
		PF_Pixel *data;
		PF_Pixel c;
		PF_Pixel bl ={64,0,0,255};
		data = (PF_Pixel *)out->data();
		long w = out->width();
		long h = out->height();
		long ow = out->offsetWidth();
		long tw	= out->widthTrue();

		long x0,x1,y0,y1;

		long i,j,t;
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
	//-------------
	void chkW16()
	{
		PF_Pixel16 *data;
		PF_Pixel16 c;
		PF_Pixel16 bl ={PF_MAX_CHAN16/4,0,0,PF_MAX_CHAN16};
		data = (PF_Pixel16 *)out->data();
		long w = out->width();
		long h = out->height();
		long ow = out->offsetWidth();
		long tw	= out->widthTrue();

		long x0,x1,y0,y1;

		long i,j,t;
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
	void chkW()
	{
		if (is16Bit()==TRUE){
			chkW16(); 
		}else{
			chkW8(); 
		}

	}

};

*/
//-------------------------------------------------------
extern "C" {
DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP);
}

#endif // GuideFrame_H
