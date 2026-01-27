//-----------------------------------------------------------------------------------
/*
	InnerFlareRot for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef InnerFlareRot_H
#define InnerFlareRot_H

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
#include "AlphaCopyDD.h"
#include "AlphaCopyR.h"

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_COLOR,
	ID_ROT,
	ID_LENGTH,
	ID_OFFSET,
	ID_BLUR,
	ID_HYPERBOLIC,

	ID_NUM_PARAMS
};


//UIの表示文字列
#define	STR_BLUR			"blur"
#define	STR_MINMAX			"max"
#define	STR_HYPERBOLIC		"hyperbolic"
#define	STR_ROT				"rot"
#define	STR_LENGTH			"length"
#define	STR_OFFSET			"offset"
#define	STR_COLOR			"color"

//UIのパラメータ
typedef struct {
	A_long				blur;
	PF_FpLong			rot;
	PF_FpLong			length;
	PF_FpLong			offset;
	PF_FpLong			hyperbolic;
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
#endif // InnerFlareRot_H

