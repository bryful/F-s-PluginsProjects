//-----------------------------------------------------------------------------------
/*
	InnerFlare for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef InnerFlare_H
#define InnerFlare_H

#include "../FsLibrary/Fs.h"
#include "Fs_Target.h"
#include "../FsLibrary/FsAE.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#include "Tiny_Mult.h"
#include "Tiny_MinMax.h"
#include "Tiny_Blur.h"
#include "Tiny_AlphaHyperbolic.h"
#include "AlphaCopy.h"
#include "AlphaCopyR.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_COLOR,
	ID_MINMAX,
	ID_BLUR,
	ID_HYPERBOLIC,
	ID_REVERCE,

	ID_NUM_PARAMS
};


//UIの表示文字列
#define	STR_BLUR			"blur"
#define	STR_MINMAX			"max"
#define	STR_HYPERBOLIC		"hyperbolic"
#define	STR_REVERCE			"reverce"
#define	STR_COLOR			"color"

//UIのパラメータ
typedef struct {
	A_long				blur;
	A_long				minmax;
	PF_FpLong			hyperbolic;
	PF_Boolean			reverce;
	PF_Pixel			color;
} ParamInfo;


//-----------------------------------------------------------------------------------
extern "C" {

DllExport 
PF_Err 
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra);
}
#endif // InnerFlare_H

