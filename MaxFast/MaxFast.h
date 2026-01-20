//-----------------------------------------------------------------------------------
/*
	MaxFast for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef MaxFast_H
#define MaxFast_H

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

using json = nlohmann::json;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_MAX,
	ID_NUM_PARAMS
};


//UIの表示文字列
#define	STR_MAX				"max"


typedef struct ParamInfo {
	A_long			max;
	PF_Boolean		maxMinus;
	A_long			dir;
} ParamInfo, *ParamInfoP, **ParamInfoH;



//-------------------------------------------------------

PF_Err MaxFast_Sub(
	CFsAE* ae,
	A_long			max,
	PF_Boolean		maxMinus
);

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
#endif // MaxFast_H

