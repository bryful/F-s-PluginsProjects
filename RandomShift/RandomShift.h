//-----------------------------------------------------------------------------------
/*
	PluginSkeleton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef RandomShift_H
#define RandomShift_H

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


//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer 
	ID_Y,
	ID_SHIFTMAX,			//boolean
	ID_SHIFTMIN,		// 1:XLine 2:YLine 3: Line 4:Box 5:BoxFill
	ID_LENGTHMAX,		//
	ID_LENGTHMIN,		//
	ID_HEIGHTMAX,	//
	ID_HEIGHTMIN,	//
	ID_VERSHIFT_VALUE,
	ID_NUM_PARAMS
	};

#define STR_Y		"量"
#define STR_SHIFTMAX	"ずらし幅の最大値(px)"
#define STR_SHIFTMIN	"ずらし幅の最小値(px)"
#define STR_LENGTHMAX	"横幅の最大値(px)"
#define STR_LEMGTHMIN	"横幅の最小値(px)"
#define STR_HEIGHTMAX	"縦幅の最大値(px)"
#define STR_HEIGHTMIN	"縦幅の最小値(px)"
#define STR_VERSHIFT_VALUE	"上下のずらし幅(px)"


//UIのパラメータ
typedef struct ParamInfo {
	A_long					value;
	A_long					shiftMax;
	A_long					shiftMin;

	A_long					shiftYMax;
	A_long					shiftYMin;

	A_long					lengthMax;
	A_long					lengthMin;
	A_long					heightMax;
	A_long					heightMin;

	A_long					shift;
	A_long					length;
	A_long					height;

	A_long					vertualShift;
} ParamInfo, *ParamInfoP, **ParamInfoH;
	
/*
class CRandomShift : public CFsAE{
public:
	CRandomShift(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output ):CFsAE(in_data,out_data,params,output){
			getParams(in_data,out_data,params,output);
			srand(m_Frame);
		}
	//パラメータを獲得する関数
	PF_Err getParams(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output );
	inline long getDir(){
		if ( F_RAND() > (F_RAND_MAX/2) ){
			return 1;
		}else{
			return -1;
		}
	}
	inline long getShift(){
		shift		= shiftMin + (shiftMax-shiftMin) * F_RAND() / F_RAND_MAX;
		if (shift<0) { shift =0;}else if (shift>shiftMax) {shift = shiftMax;}
		return shift;
	}
	inline long getLength(){
		length	= lengthMin + (lengthMax-lengthMin) * F_RAND() / F_RAND_MAX;
		if (length<0) { length =0;}else if (length>lengthMax) {length = lengthMax;}
		return length;
	}
	inline long getHeight(){
		height	= heightMin + (heightMax-heightMin) * F_RAND() / F_RAND_MAX;
		if (height<0) { height =0;}else if (height>heightMax) {height = heightMax;}
		return height;
}
	inline void get8(long x,long y){
		long xx =x;
		long yy =y;
		long ww =length;
		long hh =height;

		PF_Pixel* b = (PF_Pixel *)buf;
		long i,j;
		
		long targetB =0;
		for (j=0;j<hh;j++){
			xx =x;
			for (i=0;i<ww;i++){
				b[targetB] = in->getPixel8_F(xx,yy);
				xx++;
				targetB++;
			}
			yy++;
		}

	}
	inline void get16(long x,long y){
		long xx =x;
		long yy =y;
		long ww =length;
		long hh =height;

		PF_Pixel16* b = (PF_Pixel16 *)buf;
		long i,j;
		
		long targetB =0;
		for (j=0;j<hh;j++){
			xx =x;
			for (i=0;i<ww;i++){
			
				b[targetB] = in->getPixel16_F(xx,yy);
				xx++;
				targetB++;
			}
			yy++;
		}

	}
	inline void put8(long x,long y){
		long xx =x;
		long yy =y;
		long ww =length;
		long hh =height;

		PF_Pixel* b = (PF_Pixel *)buf;
		long i,j;
		
		long targetB =0;
		for (j=0;j<hh;j++){
			xx = x;
			for (i=0;i<ww;i++){
				out->setPixel8(xx,yy,b[targetB]);
				xx++;
				targetB++;
			}
			yy++;
		}

	}
	inline void put16(long x,long y){
		long xx =x;
		long yy =y;
		long ww =length;
		long hh =height;

		PF_Pixel16* b = (PF_Pixel16 *)buf;
		long i,j;
		
		long targetB =0;
		for (j=0;j<hh;j++){
			xx = x;
			for (i=0;i<ww;i++){
				out->setPixel16(xx,yy,b[targetB]);
				xx++;
				targetB++;
			}
			yy++;
		}

	}
	void shift8(){
		long i,x,y,v,s;
		v	= value;
		
		for (i=0; i<v; i++){
			x = (in->width() - 1) * F_RAND()/F_RAND_MAX;
			y = (in->height() - 1) * F_RAND()/F_RAND_MAX;
			length	= getLength();
			height	= getHeight();
			s		= getShift() * getDir();
			if (s!=0){
				get8(x,y);
				put8(x+s,y);
			}
		}
	}
	void shift16(){
		long i,x,y,v,s;
		v				= value;
		
		for (i=0; i<v; i++){
			x = (in->width() - 1) * F_RAND()/F_RAND_MAX;
			y = (in->height() - 1) * F_RAND()/F_RAND_MAX;
			length	= getLength();
			height	= getHeight();
			s		= getShift() * getDir();
			if (s!=0){
				get16(x,y);
				put16(x+s,y);
			}
		}
	}
	//以下固有のパラメータ
	long					value;
	long					shiftMax;
	long					shiftMin;
	long					lengthMax;
	long					lengthMin;
	long					heightMax;
	long					heightMin;

	long					shift;
	long					length;
	long					height;
	PF_PixelPtr		buf;
	long					bufSize;

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
#endif // RandomShift_H
