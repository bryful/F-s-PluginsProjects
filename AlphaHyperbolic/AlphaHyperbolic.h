//-----------------------------------------------------------------------------------
/*
	AlphaHyperbolic for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef AlphaHyperbolic_H
#define AlphaHyperbolic_H

#include "../FsLibrary/Fs.h"
#include "Fs_Target.h"
#include "../FsLibrary/FsAE.h"
#include "../FsLibrary/tinyfiledialogs.h"
#include "../FsLibrary/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "FsSettings.h"
#include "RotPoint.h"

using json = nlohmann::json;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_HYPERBOLIC,
	ID_NUM_PARAMS
};

#define ID_COLOR(IDX) (ID_TCOLOR1 + (IDX) * 2)
#define ID_COLOR_ENABLED(IDX) (ID_TCOLOR1_ENABLED + (IDX) * 2)

//UIの表示文字列
#define	STR_HYPERBOLIC		"hyperbolic"
//UIのパラメータ
#define COLOR_TABLE_COUNT	8
typedef struct ParamInfo {
	PF_FpLong		hyperbolic;
	PF_Boolean		invert;
} ParamInfo, *ParamInfoP, **ParamInfoH;


//-------------------------------------------------------
PF_Err HyperbolicAlpha(
	CFsAE* ae,
	ParamInfo* infoP);


// -----------------------------------------------------------------------------------


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
#endif // AlphaHyperbolic_H

